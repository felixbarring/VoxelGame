#include "inGame.h"

#include "graphics/chunkBatcher.h"
#include "graphics/cubeBatcher.h"
#include "graphics/cubeMap.h"
#include <emmintrin.h>

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

InGame::InGame(Game& game,
               chunk::ChunkManager&& chunkManager,
               util::SoundPlayer& soundPlayer,
               graphics::GraphicsManager& graphicsManager,
               util::FPSManager& fpsManager)
  : m_game{ game }
  , m_chunkManager(move(chunkManager))
  , m_player{ m_chunkManager, soundPlayer, graphicsManager }
  , m_soundPlayer(soundPlayer)
  , m_graphicsManager{ graphicsManager }
  , m_fpsManager{ fpsManager }
  , m_mouse{ m_graphicsManager }
  , m_settings{ m_activeWidgetGroup,
                m_mainWidgetGroup,
                m_soundPlayer,
                graphicsManager } {

  // TODO Should be possible to save and load the player location.
  double playerYLocation{ chunk_data::CHUNK_HEIGHT - 10.1 };
  m_player.setLocation(chunk_data::NUMBER_OF_CHUNKS_FROM_MIDDLE_TO_BORDER *
                         chunk_data::CHUNK_WIDTH_AND_DEPTH,
                       playerYLocation,
                       chunk_data::NUMBER_OF_CHUNKS_FROM_MIDDLE_TO_BORDER *
                         chunk_data::CHUNK_WIDTH_AND_DEPTH);

  static const int startTime{ 100 };
  m_timeCycle.setTime(startTime);

  auto observer = [this](int id) {
    switch (id) {
      case 0: {
        m_game.changeStateToMainMenu();
        m_state = GameState::NoOverlay;
        m_chunkManager.saveWorld();
        m_chunkManager.clearWorld();
        break;
      }
      case 1: {
        m_activeWidgetGroup = m_settings.getMainWidgetGroup();
        break;
      }
      case 2: {
        Input::getInstance()->centerMouse();
        m_state = GameState::NoOverlay;
        break;
      }
    }
  };

  shared_ptr<IWidget> button1(new Button{
    0, 325, 350, 150, 30, m_graphicsManager, observer, "Main Menu" });
  shared_ptr<IWidget> button2(new Button{
    1, 325, 310, 150, 30, m_graphicsManager, observer, "Settings" });
  shared_ptr<IWidget> button3(new Button{
    2, 325, 270, 150, 30, m_graphicsManager, observer, "Back To Game" });

  m_mainWidgetGroup =
    make_shared<WidgetGroup>(0, 300, 260, 200, 130, m_graphicsManager);

  m_mainWidgetGroup->addWidget(button1);
  m_mainWidgetGroup->addWidget(button2);
  m_mainWidgetGroup->addWidget(button3);

  m_activeWidgetGroup = m_mainWidgetGroup;

  m_crossHair.reset(new Sprite(
    390,
    290,
    0,
    20,
    20,
    Resources::getInstance().getTexture(config::gui_data::crossHair)));

  for (int i = 0; i <= config::cube_data::LAST_CUBE_USED_FOR_BUILDING; ++i) {
    m_selectedCubeThumbnails.push_back(make_shared<Sprite>(
      380,
      5,
      2,
      40,
      40,
      Resources::getInstance().getTexture(config::cube_data::thumbnails[i])));
  }

  // TODO This whole console system should be reworked.
  // TODO Refactor

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
  string setFOV = "setFOV";
  string setTimeSpeed = "setTimeSpeed";
  string setPlayerSpeed = "setPlayerSpeed";

  vector<string> commands{
    close,      flyMode,      gravityMode,   turnOfMusic, setMouseSensitivity,
    loadChunks, setTime,      stopTime,      resumeTime,  printDebugInfo,
    setFOV,     setTimeSpeed, setPlayerSpeed
  };

  // TODO Does not handle all errors... fix this!

  // The first string in the vector should be the command, followed by
  // arguments.
  auto func = [=](vector<string> arguments) {
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
      m_soundPlayer.stopMusic();
    } else if (command == setMouseSensitivity) {
      if (!(arguments.size() >= 3)) {
        m_terminal->addLine("Too few arguments");
        return;
      }

      double x, y;
      try {
        x = stod(arguments[1].c_str());
        y = stod(arguments[2].c_str());
      } catch (invalid_argument& e) {
        m_terminal->addLine("Invalid arguments!");
        return;
      }
      config::input_data::mouseSensitivityX = x;
      config::input_data::mouseSensitivityY = y;

    } else if (command == loadChunks) {
      if (!(arguments.size() >= 2)) {
        m_terminal->addLine("Too few arguments");
        return;
      }
      if (arguments[1] == "0")
        m_chunkManager.loadWorldWhenDecentered(false);
      else
        m_chunkManager.loadWorldWhenDecentered();
    } else if (command == setTime) {
      if (arguments.size() < 1) {
        m_terminal->addLine("Too few arguments");
        return;
      }

      double time;
      try {
        time = stod(arguments[1].c_str());
      } catch (invalid_argument& e) {
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
    } else if (command == setFOV) {
      if (arguments.size() < 2) {
        m_terminal->addLine("Too few arguments!");
        return;
      }
      float fov;
      try {
        fov = stod(arguments[1].c_str());
      } catch (exception& e) {
        m_terminal->addLine("Invalid arguments!");
        return;
      }
      m_graphicsManager.getPlayerCamera().setFov(fov);
    } else if (command == setTimeSpeed) {
      if (arguments.size() < 2) {
        m_terminal->addLine("Too few arguments!");
        return;
      }
      double time;
      try {
        time = stod(arguments[1].c_str());
      } catch (invalid_argument& e) {
        m_terminal->addLine("Invalid arguments!");
        return;
      }
      m_timeCycle.setTimeSpeed(time);
    } else if (command == setPlayerSpeed) {
      try {
        double speed = stod(arguments[1].c_str());
        m_player.setSpeed(speed);
      } catch (invalid_argument& e) {
        m_terminal->addLine("Invalid arguments!");
        return;
      }
    } else {
      m_terminal->addLine("Unknown command: " + command);
    }
  };

  m_terminal =
    make_shared<gui::Terminal>(move(commands), m_graphicsManager, func);

  auto& res = Resources::getInstance();
  FontMeshBuilder& fontMeshBuilder =
    res.getFontMeshBuilder(font_data::fontLayout,
                           font_data::fontAtlasWidth,
                           font_data::fontAtlasHeight);

  m_fps.reset(new Sprite(0,
                         45,
                         10,
                         fontMeshBuilder.buldMeshForString("FPS: " + 0, 20),
                         res.getTexture(config::font_data::font)));
}

void
InGame::update(double timePassed) {

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

    m_graphicsManager.setSunStrength(m_timeCycle.getSunStrenght());

    m_graphicsManager.getSpriteBatcher().addBatch(*m_crossHair);

    auto& res = Resources::getInstance();
    FontMeshBuilder& fontMeshBuilder =
      res.getFontMeshBuilder(font_data::fontLayout,
                             font_data::fontAtlasWidth,
                             font_data::fontAtlasHeight);

    if (m_displayDebugInfo) {

      // TODO Refactor this to its own class or something.

      vec3 dir = m_player.getViewingDirection();
      string derp = "View direction: " + to_string(dir.x) + ", " +
                    to_string(dir.y) + ", " + to_string(dir.z);

      m_direction.reset(new Sprite(0,
                                   20,
                                   10,
                                   fontMeshBuilder.buldMeshForString(derp, 20),
                                   res.getTexture(config::font_data::font)));

      // Updating the fps every frame makes it unreadable
      m_fpsDisplayCounter += timePassed;
      if (m_fpsDisplayCounter > m_fpsDisplayDelay) {
        m_fps.reset(
          new Sprite(0,
                     45,
                     10,
                     fontMeshBuilder.buldMeshForString(
                       "FPS: " + to_string(m_fpsManager.getFps()) +
                         " Frame Time = " + to_string(m_fpsManager.frameTime()),
                       20),
                     res.getTexture(config::font_data::font)));

        m_fpsDisplayCounter = 0;
      }

      vec3 ses = m_player.getLastSelectedCube();
      string soos = "Last Selected: " + to_string(ses.x) + ", " +
                    to_string(ses.y) + ", " + to_string(ses.z);
      m_lastSelecteCube.reset(
        new Sprite(0,
                   70,
                   10,
                   fontMeshBuilder.buldMeshForString(soos, 20),
                   res.getTexture(config::font_data::font)));

      // This seems to rely on that it was previously shared_ptr ...
      m_graphicsManager.getSpriteBatcher().addBatch(*m_direction);
      m_graphicsManager.getSpriteBatcher().addBatch(*m_fps);
      m_graphicsManager.getSpriteBatcher().addBatch(*m_lastSelecteCube);
    }

    m_graphicsManager.getSpriteBatcher().addBatch(
      *m_selectedCubeThumbnails[m_player.getBuildingCube()]);

  } else {
    m_mouse.unlock();
    input->updateValues();
    m_mouse.update();
    m_mouse.draw();
  }

  if (m_state == GameState::Terminal) {
    m_terminal->update(timePassed);
    m_terminal->draw();
  } else if (m_state == GameState::OverlayMenu) {
    m_activeWidgetGroup->update(timePassed);
    m_activeWidgetGroup->draw();
  }

  m_chunkManager.update();

  m_graphicsManager.clearScreenSunDependent();

  if (m_timeCycle.getStarStrenght() > 0.0) {
    // Changes so that the rotation dose not get to fast.
    static const double valModifier{ 0.015 };
    m_graphicsManager.getSkyMap().setRotationValue(valModifier *
                                                   m_timeCycle.getTime());
    m_graphicsManager.getSkyMap().draw(m_timeCycle.getStarStrenght());
  }

  m_graphicsManager.getChunkBatcher().draw();
  m_graphicsManager.getCubeBatcher().draw();
  m_graphicsManager.getSpriteBatcher().draw();
}
