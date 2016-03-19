#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "util/input.h"
#include "util/fpsManager.h"
#include "config/data.h"

#include "graphics/chunkBatcher.h"
#include "graphics/cubeBatcher.h"

#include "inGame.h"
#include "mainMenu.h"

#include "model/world/chunk/chunkManager.h"

#include "gui/guiUtil.h"

#include <SFML/Audio.hpp>


#include "util/soundPlayer.h"

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

// ########################################################
// Constructor/Destructor #################################
// ########################################################

// ########################################################
// Member Functions########################################
// ########################################################

void Game::run() {

//	util::SoundPlayer::getInstance().playMusic(config::music::menuMusic);
	util::FPSManager fpsManager(1000); // config::graphics_data::fps);

	int WIDTH = 800;
	int HEIGHT = 600;

	util::Input::createInstance(nullptr, WIDTH / 2.0, HEIGHT / 2.0);

	config::graphics_data::windowWidth = WIDTH;
	config::graphics_data::windowHeight = HEIGHT;

    // create the window
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 1;

	sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, settings);
	window.setMouseCursorVisible(false);

//	window.setVerticalSyncEnabled(true);
//	window.setFramerateLimit(300);

	util::Input::getInstance()->setWindow(&window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
		cout << "Failed to initialize GLEW\n";

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	m_mainMenu.reset(new MainMenu(this));
	m_currentState = m_mainMenu;

    // run the main loop
    bool running = true;
    while (!m_quit)
    {
		fpsManager.frameStart();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_currentState->update(fpsManager.frameTime());

		cout << fpsManager.getFps() << "\n";

		fpsManager.sync();
        window.display();
    }

}

void Game::createNewWorld(string name) {
	m_inGame.reset(new InGame(this, name));
	chunk::ChunkManager::getInstance().createNewWorld();
	m_currentState = m_inGame;
}

void Game::loadExistingWorld(string name) {
	m_inGame.reset(new InGame(this, name));
	chunk::ChunkManager::getInstance().loadWorld(name);
	m_currentState = m_inGame;
}

void Game::changeStateToMainMenu() {
	m_currentState = m_mainMenu;
}

void Game::quitGame() {
	m_quit = true;
}
