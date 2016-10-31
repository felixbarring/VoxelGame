#include "inGame.h"

#include <emmintrin.h>
#include "graphics/chunkBatcher.h"
#include "graphics/cubeBatcher.h"
#include "graphics/cubeMap.h"

#include "model/world/chunk/chunkManager.h"

#include "config/data.h"
#include "gui/mouse.h"
#include "gui/terminal.h"
#include "util/fpsManager.h"
#include "util/soundPlayer.h"

using chunk::ChunkManager;

using namespace std;
using namespace glm;

using namespace graphics;
using namespace config;
using namespace widget;
using namespace util;
using namespace gui;

// ########################################################
// Constructor/Destructor #################################
// ########################################################

InGame::InGame(Game *game, string name)
    : m_game{game},
      m_name{name} {

    m_player.setLocation(chunk_data::NUMBER_OF_CHUNKS_FROM_MIDDLE_TO_BORDER * chunk_data::CHUNK_WIDTH_AND_DEPTH, 40.1,
                         chunk_data::NUMBER_OF_CHUNKS_FROM_MIDDLE_TO_BORDER * chunk_data::CHUNK_WIDTH_AND_DEPTH);

    texture::TextureCubeMap &texture =
            graphics::Resources::getInstance().getTextureCubeMap(
                    config::cube_map_data::cubeMap1[0],
                    config::cube_map_data::cubeMap1[1],
                    config::cube_map_data::cubeMap1[2],
                    config::cube_map_data::cubeMap1[3],
                    config::cube_map_data::cubeMap1[4],
                    config::cube_map_data::cubeMap1[5],
                    config::cube_map_data::cubeMap1Width,
                    config::cube_map_data::cubeMap1Height);

    m_skyBox = std::make_shared<graphics::CubeMap>(texture);

    auto observer = [this, game](int id)
    {
        switch(id) {
        case 0: {
            game->changeStateToMainMenu();
            m_state = GameState::NoOverlay;
            chunk::ChunkManager::getInstance().saveWorld();
            chunk::ChunkManager::getInstance().clearWorld();
            break;
        }
        case 1: {
            Input::getInstance()->centerMouse();
            m_state = GameState::NoOverlay;
            break;
        }
    }
    };

    shared_ptr<IWidget> button1(new Button{0, 325, 350, 150, 30, observer, "Main Menu"});
    shared_ptr<IWidget> button2(new Button{1, 325, 310, 150, 30, observer, "Back To Game"});
    m_widgetGroup1.reset(new WidgetGroup{0, 300, 300, 200, 90, observer});

    m_widgetGroup1->addWidget(button1);
    m_widgetGroup1->addWidget(button2);

    m_crossHair.reset(new Sprite(390, 290, 0, 20, 20, Resources::getInstance().getTexture(
            config::gui_data::crossHair)));

    for (int i = 0; i <= config::cube_data::LAST_CUBE_USED_FOR_BUILDING_; ++i) {
        m_selectedCubeThumbnails.push_back(make_shared<Sprite>(
                380, 5, 2, 40, 40, Resources::getInstance().getTexture(config::cube_data::thumbnails[i])));
    }

    string close = "close";
    string flyMode = "flyMode";
    string gravityMode = "gravityMode";
    string turnOfMusic = "turnOfMusic";
    string setMouseSensitivity = "setMouseSensitivity";
    string loadChunks = "loadChunks";
    string setTime = "setTime";
    string stopTime = "stopTime";
    string resumeTime = "resumeTime";
    string setDayLenght = "setDayLenght";
    string printDebugInfo = "printDebugInfo";
    vector<string> commands{close, flyMode, gravityMode, turnOfMusic, setMouseSensitivity, loadChunks, setTime,
                            stopTime, resumeTime, setDayLenght, printDebugInfo};

    // The first string in the vector should be the command, followed by arguments.
    auto func = [=](vector<string> arguments)
    {
        if (arguments.empty())
            return;

        auto command = arguments[0];

        if (command == close) {
            Input::getInstance()->centerMouse();
            m_state = GameState::NoOverlay;
        } else if (command == flyMode) {
            m_player.turnGravityOff();
        } else if (command == gravityMode) {
            m_player.turnGravityOff(false);
        } else if (command == turnOfMusic) {
            util::SoundPlayer::getInstance().stopMusic();
        } else if (command == setMouseSensitivity) {
            if (!(arguments.size() >= 3)) {
                m_terminal->addLine("To few arguments");
                return;
            }
            // TODO Error handeling here...
            config::input_data::mouseSensitivityX = std::atof(arguments[1].c_str());
            config::input_data::mouseSensitivityY = std::atof(arguments[2].c_str());
        } else if (command == loadChunks) {

            // TODO Error handeling here...
            if (!(arguments.size() >= 2)) {
                m_terminal->addLine("To few arguments");
                return;
            }
            if (arguments[1] == "0")
                ChunkManager::getInstance().loadWorldWhenDecentered(false);
            else
                ChunkManager::getInstance().loadWorldWhenDecentered();
        } else if (command == setTime) {
            // TODO Error handeling here...
            float time = std::atof(arguments[1].c_str());
            m_timeCycle.setTime(time);
        } else if (command == stopTime) {
            m_timeCycle.stopCycle();
        } else if (command == resumeTime) {
            m_timeCycle.resumeCycle();
        } else if (command == setDayLenght) {
            // TODO Error handeling here...
            float dayLenght = std::atof(arguments[1].c_str());
            m_timeCycle.setDayLenght(dayLenght);
        } else if (command == printDebugInfo) {
            // TODO Error handeling here...
            float argumentValue = std::atof(arguments[1].c_str());
                m_displayDebugInfo = argumentValue > 0;
        } else {
            m_terminal->addLine("Unknown command: " + command);
        }

    };

    m_terminal = make_shared<gui::Terminal>(move(commands), func);

    auto &res = Resources::getInstance();
    FontMeshBuilder &fontMeshBuilder = res.getFontMeshBuilder(
            font_data::fontLayout,
            font_data::fontAtlasWidth,
            font_data::fontAtlasHeight);

    m_fps.reset(new Sprite(0, 45, 10, fontMeshBuilder.buldMeshForString("FPS: " + 0, 20),
                            res.getTexture(config::font_data::font)));

}

// ########################################################
// Member Functions########################################
// ########################################################

void InGame::update(float timePassed) {

    auto input = Input::getInstance();
    auto &mouse = Mouse::getInstance();

    if (input->openTerminalPressed)
        m_state = GameState::Terminal;

    if (input->escapeKeyPressed) {
        if (m_state == GameState::Terminal)
            m_state = GameState::NoOverlay;
        else
            m_state = GameState::OverlayMenu;
    }

    if (m_state == GameState::NoOverlay) {
        mouse.lock();
        input->updateValues();
        mouse.update();
        m_player.update(timePassed);
        m_timeCycle.update(timePassed);

        float sunStrenght{m_timeCycle.getSunStrenght()};
        ChunkBatcher::getInstance().setSunStrenght(sunStrenght);
        CubeBatcher::getInstance().setSunStrenght(sunStrenght);
        SpriteBatcher::getInstance().addBatch(m_crossHair);

        auto &res = Resources::getInstance();
        FontMeshBuilder &fontMeshBuilder = res.getFontMeshBuilder(font_data::fontLayout, font_data::fontAtlasWidth,
                font_data::fontAtlasHeight);

        if (m_displayDebugInfo) {

            vec3 dir = m_player.getViewingDirection();
            string derp = "View direction: " + to_string(dir.x) + ", " + to_string(dir.y) + ", " + to_string(dir.z);

            m_direction.reset(new Sprite(0, 20, 10, fontMeshBuilder.buldMeshForString(derp, 20),
                            res.getTexture(config::font_data::font)));

            // Updating the fps every frame makes it unreadable
            m_fpsDisplayCounter += timePassed;
            if (m_fpsDisplayCounter > m_fpsDisplayDelay) {
                m_fps.reset(new Sprite(0, 45, 10, fontMeshBuilder.buldMeshForString("FPS: " +
                        to_string(util::FPSManager::getFps()) + " Frame Time = " + to_string(
                                util::FPSManager::frameTime()), 20),
                                res.getTexture(config::font_data::font)));

                m_fpsDisplayCounter = 0;
            }

            SpriteBatcher::getInstance().addBatch(m_direction);
            SpriteBatcher::getInstance().addBatch(m_fps);
            SpriteBatcher::getInstance().addBatch(m_lastSelecteCube);
        }

        vec3 ses = m_player.getLastSelectedCube();
        string soos = "Last Selected: " + to_string(ses.x) + ", " + to_string(ses.y) + ", " + to_string(ses.z);
        m_lastSelecteCube.reset(new Sprite(0, 70, 10, fontMeshBuilder.buldMeshForString(soos, 20),
                    res.getTexture(config::font_data::font)));

        SpriteBatcher::getInstance().addBatch(m_selectedCubeThumbnails[m_player.getBuildingCube()]);

    } else {
        mouse.unlock();
        input->updateValues();
        mouse.update();
        mouse.draw();
    }

    if (m_state == GameState::Terminal) {
        m_terminal->update(timePassed);
        m_terminal->draw();
    } else 	if (m_state == GameState::OverlayMenu) {
        m_widgetGroup1->update(timePassed);
        m_widgetGroup1->draw();
    }

    chunk::ChunkManager::getInstance().update();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_skyBox->render();

    ChunkBatcher::getInstance().draw();
    CubeBatcher::getInstance().draw();
    SpriteBatcher::getInstance().draw();


}

