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

#include "graphics/fontMeshBuilder.h"
#include "graphics/sprite.h"
#include "graphics/resources.h"
#include "util/fpsManager.h"

using graphics::ChunkBatcher;
using graphics::CubeBatcher;
using graphics::FontMeshBuilder;
using graphics::Resources;
using graphics::Sprite;
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
		  window(window)
	{
		auto &res = Resources::getInstance();
		FontMeshBuilder &fontMeshBuilder = res.getFontMeshBuilder(
				config::font_data::fontLayout, config::font_data::fontAtlasWidth,
				config::font_data::fontAtlasHeight);

		sprites.push_back(shared_ptr<Sprite> {new Sprite(300, 300, 10,
							fontMeshBuilder.buldMeshForString("Loading", 80),
							res.getTexture(config::font_data::font))});
		sprites.push_back(shared_ptr<Sprite> {new Sprite(300, 300, 10,
							fontMeshBuilder.buldMeshForString("Loading.", 80),
							res.getTexture(config::font_data::font))});
		sprites.push_back(shared_ptr<Sprite> {new Sprite(300, 300, 10,
							fontMeshBuilder.buldMeshForString("Loading..", 80),
							res.getTexture(config::font_data::font))});
		sprites.push_back(shared_ptr<Sprite> {new Sprite(300, 300, 10,
							fontMeshBuilder.buldMeshForString("Loading...", 80),
							res.getTexture(config::font_data::font))});
	}

	void update()
	{
		m_fpsManager.frameStart();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (frameTime > timePerFrame) {
			frameTime = 0.0;
			++spriteCounter;

			if (spriteCounter >= sprites.size())
				spriteCounter = 0;
		}

		graphics::SpriteBatcher::getInstance().addBatch(sprites[spriteCounter]);
		graphics::SpriteBatcher::getInstance().draw();

		m_fpsManager.sync();
		frameTime += m_fpsManager.frameTime();
		window->display();
	}

private:

	vector<shared_ptr<Sprite>> sprites {};
	FPSManager &m_fpsManager;
	sf::Window *window;

	unsigned spriteCounter{0};
	double timePerFrame{0.3};
	double frameTime{0.0};

};


// ########################################################
// Member Functions########################################
// ########################################################

void Game::run() {

	util::SoundPlayer::getInstance().playMusic(config::music::menuMusic);

	int WIDTH = config::graphics_data::windowWidth;
	int HEIGHT = config::graphics_data::windowHeight;

	util::Input::createInstance(WIDTH / 2.0, HEIGHT / 2.0);

//	config::graphics_data::windowWidth = WIDTH;
//	config::graphics_data::windowHeight = HEIGHT;

    // create the window
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 1;

	window = new sf::Window{sf::VideoMode(WIDTH, HEIGHT), "Voxel Game",
				sf::Style::Default, settings};

	window->setMouseCursorVisible(false);

//	window.setVerticalSyncEnabled(true);
//	window.setFramerateLimit(300);

	util::Input::getInstance()->setWindow(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
		cout << "Failed to initialize GLEW\n";

	glClearColor(0.47f, 0.76f, 0.93f, 1.0f);
	glViewport(0, 0, WIDTH, HEIGHT);

	m_mainMenu.reset(new MainMenu(this));
	m_currentState = m_mainMenu;

	// Load all graphics batchers some where else !1!
	ChunkBatcher::getInstance();

    // run the main loop
    while (!m_quit && window->isOpen()) {

		m_fpsManager.frameStart();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_currentState->update(m_fpsManager.frameTime());

		m_fpsManager.sync();
        window->display();
    }

}

void Game::createWorld(string name) {

	util::SoundPlayer::getInstance().stopMusic();

	// TODO Do ingame really need the world name?!?
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
