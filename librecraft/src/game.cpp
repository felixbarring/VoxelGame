#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <thread>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "util/input.h"

#include "graphics/chunkBatcher.h"
#include "graphics/cubeBatcher.h"

#include "inGame.h"
#include "mainMenu.h"
#include "model/world/chunk/chunkManager.h"
#include "gui/guiUtil.h"
#include "util/soundPlayer.h"

#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <future>

#include "detail/type_vec.hpp"
#include "graphics/fontMeshBuilder.h"
#include "graphics/sprite.h"
#include "graphics/resources.h"
#include "util/checkSystem.h"
#include "util/fpsManager.h"

using graphics::ChunkBatcher;
using graphics::CubeBatcher;
using graphics::FontMeshBuilder;
using graphics::Resources;
using graphics::Sprite;
using sf::Window;
using util::FPSManager;

using namespace std;

// ########################################################
// Constructor/Destructor #################################
// ########################################################

// ########################################################
// Helper Class ###########################################
// ########################################################

class LoadingScreen {
public:

    LoadingScreen(FPSManager &fpsManager, sf::Window *window)
        : m_fpsManager(fpsManager),
          m_window(window)
    {
        auto &res = Resources::getInstance();
        FontMeshBuilder &fontMeshBuilder = res.getFontMeshBuilder(
                config::font_data::fontLayout,
                config::font_data::fontAtlasWidth,
                config::font_data::fontAtlasHeight);

        m_sprites.push_back(shared_ptr<Sprite> {new Sprite(300, 300, 10,
                            fontMeshBuilder.buldMeshForString("Loading", 80),
                            res.getTexture(config::font_data::font))});
        m_sprites.push_back(shared_ptr<Sprite> {new Sprite(300, 300, 10,
                            fontMeshBuilder.buldMeshForString("Loading.", 80),
                            res.getTexture(config::font_data::font))});
        m_sprites.push_back(shared_ptr<Sprite> {new Sprite(300, 300, 10,
                            fontMeshBuilder.buldMeshForString("Loading..", 80),
                            res.getTexture(config::font_data::font))});
        m_sprites.push_back(shared_ptr<Sprite> {new Sprite(300, 300, 10,
                            fontMeshBuilder.buldMeshForString("Loading...", 80),
                            res.getTexture(config::font_data::font))});
    }

    void update()
    {
        m_fpsManager.frameStart();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (m_frameTime > m_timePerFrame) {
            m_frameTime = 0.0;
            ++m_spriteCounter;

            if (m_spriteCounter >= m_sprites.size())
                m_spriteCounter = 0;
        }

        graphics::SpriteBatcher::getInstance().addBatch(m_sprites[m_spriteCounter]);
        graphics::SpriteBatcher::getInstance().draw();

        m_fpsManager.sync();
        m_frameTime += m_fpsManager.frameTime();
        m_window->display();
    }

private:

    vector<shared_ptr<Sprite>> m_sprites {};
    FPSManager &m_fpsManager;
    sf::Window *m_window;

    unsigned m_spriteCounter{0};
    const double m_timePerFrame{0.3};
    double m_frameTime{0.0};

};


// ########################################################
// Member Functions########################################
// ########################################################

void Game::run() {

    util::check_system::checkStuff();

    util::SoundPlayer::getInstance().playMusic(config::music::menuMusic);

    int WIDTH = config::graphics_data::windowWidth;
    int HEIGHT = config::graphics_data::windowHeight;

    util::Input::createInstance(WIDTH / 2.0, HEIGHT / 2.0);

    // create the window
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 1;

    window = new sf::Window{sf::VideoMode(WIDTH, HEIGHT), "Voxel Game", sf::Style::Default, settings};

    window->setMouseCursorVisible(false);

//    window->setVerticalSyncEnabled(true);
//    window->setFramerateLimit(300);

    util::Input::getInstance()->setWindow(window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK)
        cout << "Failed to initialize GLEW\n";

    glm::vec3 skyColor = config::graphics_data::skyColor;
    glClearColor(skyColor.x, skyColor.y, skyColor.z, 1.0f);
    glViewport(0, 0, WIDTH, HEIGHT);

    m_mainMenu.reset(new MainMenu(this));
    m_currentState = m_mainMenu;

    // Load all graphics batchers somewhere else
    // Done here on the main thread to avoid thread issues.
    ChunkBatcher::getInstance();

    // run the main loop
    while (!m_quit && window->isOpen()) {

        m_fpsManager.frameStart();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_currentState->update(m_fpsManager.frameTime());
        window->display();
        m_fpsManager.sync();
    }

}

void Game::createWorld(string name) {

    //util::SoundPlayer::getInstance().stopMusic();

    m_inGame.reset(new InGame(this, name));
    auto future = m_threadPool.enqueue([name]
    {
        chunk::ChunkManager::getInstance().createWorld(name);
    }
    );

    LoadingScreen loadingScreen(m_fpsManager, window);

    std::chrono::milliseconds span{0};
    while (future.wait_for(span) != future_status::ready)
        loadingScreen.update();

    m_currentState = m_inGame;
}

void Game::changeStateToMainMenu() {
    m_currentState = m_mainMenu;
}

void Game::quitGame() {
	m_quit = true;
}

