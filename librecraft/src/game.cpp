#include "game.h"

#include <chrono>
#include <future>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

#include "detail/type_vec.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "ThreadPool.h"

#include "gui/guiUtil.h"
#include "inGame.h"
#include "mainMenu.h"
#include "util/soundPlayer.h"

#include "graphics/chunkBatcher.h"
#include "graphics/cubeBatcher.h"
#include "graphics/fontMeshBuilder.h"
#include "graphics/graphicsManager.h"
#include "graphics/resources.h"
#include "graphics/sprite.h"

#include "util/checkSystem.h"
#include "util/fpsManager.h"
#include "util/globalResources.h"
#include "util/input.h"
#include "util/settingsPersistence.h"

using graphics::ChunkBatcher;
using graphics::CubeBatcher;
using graphics::FontMeshBuilder;
using graphics::Resources;
using graphics::Sprite;
using graphics::GraphicsManager;
using sf::Window;
using util::FPSManager;

using namespace std;
using namespace util;

class LoadingScreen {
public:
  LoadingScreen(FPSManager& fpsManager,
                sf::Window* window,
                graphics::GraphicsManager& graphicsManager)
    : m_fpsManager(fpsManager)
    , m_window(window)
    , m_graphicsManager{graphicsManager} {
    auto& res = Resources::getInstance();
    FontMeshBuilder& fontMeshBuilder =
      res.getFontMeshBuilder(config::font_data::fontLayout,
                             config::font_data::fontAtlasWidth,
                             config::font_data::fontAtlasHeight);

    const unsigned numberOfDots{3};
    string dots{""};
    for (unsigned i{0}; i <= numberOfDots; ++i) {
      m_sprites.push_back(make_shared<Sprite>(
        300,
        300,
        10,
        fontMeshBuilder.buldMeshForString("Loading" + dots, 80),
        res.getTexture(config::font_data::font)));
      dots += ".";
    }
  }

  void update() {
    m_fpsManager.frameStart();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_frameTime > m_timePerFrame) {
      m_frameTime = 0.0;
      ++m_spriteCounter;

      if (m_spriteCounter >= m_sprites.size())
        m_spriteCounter = 0;
    }

    m_graphicsManager.getSpriteBatcher().addBatch(*m_sprites[m_spriteCounter]);
    m_graphicsManager.getSpriteBatcher().draw();

    m_fpsManager.sync();
    m_frameTime += m_fpsManager.frameTime();
    m_window->display();
  }

private:
  vector<shared_ptr<Sprite>> m_sprites{};
  FPSManager& m_fpsManager;
  sf::Window* m_window;

  graphics::GraphicsManager& m_graphicsManager;

  unsigned m_spriteCounter{0};
  const double m_timePerFrame{0.2};
  double m_frameTime{0.0};
};

void
Game::run() {
  check_system::checkStuff();

  {
    SettingsPersistence settingsPersistence{};
    settingsPersistence.loadSettings();
  }

  bool useFullscreen{false};

  if (useFullscreen) {
    config::graphics_data::windowWidth = sf::VideoMode::getDesktopMode().width;
    config::graphics_data::windowHeight =
      sf::VideoMode::getDesktopMode().height;
  }

  const int width = config::graphics_data::windowWidth;
  const int height = config::graphics_data::windowHeight;

  Input::createInstance(width / 2.0, height / 2.0);

  // create the window
  sf::ContextSettings settings;
  settings.depthBits = 24;
  settings.stencilBits = 8;
  settings.antialiasingLevel = 4;
  settings.majorVersion = 3;
  settings.minorVersion = 1;

  string windowTitle = "Voxel Game";

  if (useFullscreen) {
    window = new sf::Window{sf::VideoMode::getDesktopMode(),
                            windowTitle,
                            sf::Style::Default,
                            settings};
  } else {
    window = new sf::Window{
      sf::VideoMode(width, height), windowTitle, sf::Style::Default, settings};
  }

  window->setMouseCursorVisible(false);

  Input::getInstance()->setWindow(window);

  glewExperimental = true;
  if (glewInit() != GLEW_OK)
    cout << "Failed to initialize GLEW\n";

  // TODO No opengl outside graphics!
  glViewport(0, 0, width, height);

  m_graphicsmanager = make_unique<graphics::GraphicsManager>();

  m_mainMenu.reset(new MainMenu(*this, m_soundPlayer, *m_graphicsmanager));
  changeStateToMainMenu();

  // Run the main loop
  while (!m_quit && window->isOpen()) {
    m_fpsManager.frameStart();

    shared_ptr<util::Input> input = util::Input::getInstance();
    input->updateValues();

    auto frameTime = m_fpsManager.frameTime();
    m_soundPlayer.update(frameTime);

    m_currentState->update(frameTime);
    window->display();
    m_fpsManager.sync();
  }

  SettingsPersistence settingsPersistence{};
  settingsPersistence.storeSettings();
}

void
Game::createWorld(chunk::CreationOptions options) {
  chunk::ChunkManager chunkManager{options, m_soundPlayer, *m_graphicsmanager};

  auto future = globalResources::g_threadPool.enqueue(
    [options, &chunkManager] { chunkManager.createWorld(); });

  LoadingScreen loadingScreen(m_fpsManager, window, *m_graphicsmanager);

  std::chrono::milliseconds span{0};
  while (future.wait_for(span) != future_status::ready) {
    loadingScreen.update();
    // Done to prevent the window to be grayed out like if the application
    // has stopped responding even though it works correctly. Seems like
    // there is some kind of watch dog on Ubuntu checking this anyway.
    shared_ptr<util::Input> input{util::Input::getInstance()};
    input->updateValues();
  }

  m_inGame = make_shared<InGame>(*this,
                            move(chunkManager),
                            m_soundPlayer,
                            *m_graphicsmanager,
                            m_fpsManager);

  m_currentState = m_inGame;
  m_soundPlayer.stopMusic();
}

void
Game::changeStateToMainMenu() {
  m_currentState = m_mainMenu;
  m_soundPlayer.playMusic(config::audio::menuMusic);
}

void
Game::quitGame() {
  m_quit = true;
}
