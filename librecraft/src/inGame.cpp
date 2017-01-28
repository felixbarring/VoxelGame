#include "inGame.h"

#include <emmintrin.h>
#include "graphics/chunkBatcher.h"
#include "graphics/cubeBatcher.h"
#include "graphics/cubeMap.h"

#include "model/world/chunk/chunkManager.h"

#include "config/data.h"
#include "graphics/graphicsManager.h"
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

InGame::InGame(Game *game)
    : m_game{game}
{

    m_player.setLocation(chunk_data::NUMBER_OF_CHUNKS_FROM_MIDDLE_TO_BORDER * chunk_data::CHUNK_WIDTH_AND_DEPTH, 40.1,
                         chunk_data::NUMBER_OF_CHUNKS_FROM_MIDDLE_TO_BORDER * chunk_data::CHUNK_WIDTH_AND_DEPTH);

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
    string printDebugInfo = "printDebugInfo";
    vector<string> commands{close, flyMode, gravityMode, turnOfMusic, setMouseSensitivity, loadChunks, setTime,
                            stopTime, resumeTime, printDebugInfo};

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

            double x, y;
            try {
                x = stod(arguments[1].c_str());
                y = stod(arguments[2].c_str());
            } catch (invalid_argument &e) {
                m_terminal->addLine("Invalid arguments!");
                return;
            }
            config::input_data::mouseSensitivityX = x;
            config::input_data::mouseSensitivityY = y;

        } else if (command == loadChunks) {
            if (!(arguments.size() >= 2)) {
                m_terminal->addLine("To few arguments");
                return;
            }
            if (arguments[1] == "0")
                ChunkManager::getInstance().loadWorldWhenDecentered(false);
            else
                ChunkManager::getInstance().loadWorldWhenDecentered();
        } else if (command == setTime) {
            if (arguments.size() < 1) {
                m_terminal->addLine("To few arguments");
                return;
            }

            double time;
            try {
                time = stod(arguments[1].c_str());
            } catch (invalid_argument &e) {
                m_terminal->addLine("Invalid arguments!");
                return;
            }
            m_timeCycle.setTime(time);

        } else if (command == stopTime) {
            m_timeCycle.stopCycle();
        } else if (command == resumeTime) {
            m_timeCycle.resumeCycle();
        } else if (command == printDebugInfo) {
            m_displayDebugInfo = !arguments.empty();
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

    if (input->openTerminalPressed)
        m_state = GameState::Terminal;

    if (input->escapeKeyPressed) {
        if (m_state == GameState::Terminal)
            m_state = GameState::NoOverlay;
        else
            m_state = GameState::OverlayMenu;
    }

    if (m_state == GameState::NoOverlay) {
        m_mouse.lock();
        input->updateValues();
        m_mouse.update();
        m_player.update(timePassed);
        m_timeCycle.update(timePassed);

        graphics::GraphicsManager::getInstance().setSunStrenght(m_timeCycle.getSunStrenght());

        GraphicsManager::getInstance().getSpriteBatcher().addBatch(m_crossHair);

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

            GraphicsManager::getInstance().getSpriteBatcher().addBatch(m_direction);
            GraphicsManager::getInstance().getSpriteBatcher().addBatch(m_fps);
            GraphicsManager::getInstance().getSpriteBatcher().addBatch(m_lastSelecteCube);
        }

        vec3 ses = m_player.getLastSelectedCube();
        string soos = "Last Selected: " + to_string(ses.x) + ", " + to_string(ses.y) + ", " + to_string(ses.z);
        m_lastSelecteCube.reset(new Sprite(0, 70, 10, fontMeshBuilder.buldMeshForString(soos, 20),
                    res.getTexture(config::font_data::font)));

        GraphicsManager::getInstance().getSpriteBatcher().addBatch(m_selectedCubeThumbnails[m_player.getBuildingCube()]);

    } else {
        m_mouse.unlock();
        input->updateValues();
        m_mouse.update();
        m_mouse.draw();
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

    if (m_timeCycle.getStarStrenght() > 0.0) {
        float valModifier = 0.01; // Changes so that the rotation dose not get to fast.
        GraphicsManager::getInstance().getSkyMap().setRotationValue(valModifier * m_timeCycle.getTime());
        GraphicsManager::getInstance().getSkyMap().draw(m_timeCycle.getStarStrenght());
    }

    GraphicsManager::getInstance().getChunkBatcher().draw();
    GraphicsManager::getInstance().getCubeBatcher().draw();

    GraphicsManager::getInstance().getSpriteBatcher().draw();

}

