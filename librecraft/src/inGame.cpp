
#include "inGame.h"

#include "graphics/chunkBatcher.h"
#include "graphics/cubeBatcher.h"

#include "model/world/chunk/chunkManager.h"

#include "config/data.h"
#include "gui/mouse.h"

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

InGame::InGame(Game *game, string name) :
	m_game{game},
	m_name{name},
	skybox{graphics::Resources::getInstance().getTextureCubeMap(
			cube_map_data::cubeMap1[0],
			cube_map_data::cubeMap1[1],
			cube_map_data::cubeMap1[2],
			cube_map_data::cubeMap1[3],
			cube_map_data::cubeMap1[4],
			cube_map_data::cubeMap1[5],
			cube_map_data::cubeMap1Width,
			cube_map_data::cubeMap1Height)}
{

//	glm::mat4 matrix = gui::createVirtualToScreen(
//			1920,
//			1080,
//			static_cast<float>(config::graphics_data::windowWidth),
//			static_cast<float>(config::graphics_data::windowHeight));
//
//	m_virtualProjection = glm::ortho(0.0f,
//			static_cast<float>(config::graphics_data::windowWidth), 0.0f,
//			static_cast<float>(config::graphics_data::windowHeight), -1.0f, 1.0f) * matrix;
//	graphics::SpriteBatcher::getInstance().setProjection(m_virtualProjection);

	m_player.setLocation(80, 7, 80);

	std::function<void(int)> observer = [&, game](int id)
	{
		switch(id) {
			case 0: {
				game->changeStateToMainMenu();
				state = GameState::NoOverlay;
				chunk::ChunkManager::getInstance().saveWorld(m_name);
				break;
			}
			case 1: {
				Input::getInstance()->centerMouse();
				state = GameState::NoOverlay;
				break;
			}
		}
	};

	shared_ptr<IWidget> button1(new Button{0, 325, 350, 150, 30, observer, "Main Menu"});
	shared_ptr<IWidget> button2(new Button{1, 325, 310, 150, 30, observer, "Back To Game"});
	m_widgetGroup1.reset(new WidgetGroup{0, 300, 300, 200, 90, observer});

	m_widgetGroup1->addWidget(button1);
	m_widgetGroup1->addWidget(button2);

	m_crossHair.reset(new Sprite(380, 280, 0, 40, 40,
			Resources::getInstance().getTexture(config::gui_data::crossHair)));

}

// ########################################################
// Member Functions########################################
// ########################################################

void InGame::update(float timePassed)
{

	auto input = Input::getInstance();
	auto &mouse = Mouse::getInstance();

	if (input->escapeKeyPressed)
		state = GameState::OverlayMenu;

	if (state == GameState::NoOverlay) {
		mouse.lock();
		input->updateValues();
		mouse.update();
		m_player.update(timePassed);

		SpriteBatcher::getInstance().addBatch(m_crossHair);
	} else {
		mouse.unlock();
		input->updateValues();
		mouse.update();
		mouse.draw();
	}

	glDisable(GL_DEPTH_TEST);

	//skybox.render();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	ChunkBatcher::getInstance().draw();
	CubeBatcher::getInstance().draw();

	if (state == GameState::OverlayMenu) {

		double y = input->mouseYPosition - graphics_data::windowHeight;
		if (y < 0) {
			y = -y;
		} else {
			y = -1;
		}

		vec2 mouse = adjustMouse(graphics_data::virtualWidth,
				graphics_data::virtualHeight,
				graphics_data::windowWidth,
				graphics_data::windowHeight,
				Input::getInstance()->mouseXPosition, y);

		input->mouseVirtualAdjustedX = mouse.x;
		input->mouseVirtualAdjustedY = mouse.y;

		m_widgetGroup1->update(timePassed);
		m_widgetGroup1->draw();
	}

	SpriteBatcher::getInstance().draw();
}
