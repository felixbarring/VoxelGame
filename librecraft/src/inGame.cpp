#include "inGame.h"

#include "graphics/chunkBatcher.h"
#include "graphics/cubeBatcher.h"

#include "model/world/chunk/chunkManager.h"

#include "config/data.h"
#include "gui/mouse.h"
#include "util/fpsManager.h"

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
		: m_game {game}, m_name {name}, skybox {
				graphics::Resources::getInstance().getTextureCubeMap(
						cube_map_data::cubeMap1[0], cube_map_data::cubeMap1[1],
						cube_map_data::cubeMap1[2], cube_map_data::cubeMap1[3],
						cube_map_data::cubeMap1[4], cube_map_data::cubeMap1[5],
						cube_map_data::cubeMap1Width,
						cube_map_data::cubeMap1Height)} {

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

	m_player.setLocation(80.1, 25.1, 80.1);

	std::function<void(int)> observer = [&, game](int id)
	{
		switch(id) {
			case 0: {
				game->changeStateToMainMenu();
				m_state = GameState::NoOverlay;
				chunk::ChunkManager::getInstance().saveWorld(m_name);
				break;
			}
			case 1: {
				Input::getInstance()->centerMouse();
				m_state = GameState::NoOverlay;
				break;
			}
		}
	};

	shared_ptr < IWidget > button1(new Button {0, 325, 350, 150, 30, observer,
			"Main Menu"});
	shared_ptr < IWidget > button2(new Button {1, 325, 310, 150, 30, observer,
			"Back To Game"});
	m_widgetGroup1.reset(new WidgetGroup {0, 300, 300, 200, 90, observer});

	m_widgetGroup1->addWidget(button1);
	m_widgetGroup1->addWidget(button2);

	m_crossHair.reset(
			new Sprite(390, 290, 0, 20, 20,
					Resources::getInstance().getTexture(
							config::gui_data::crossHair)));

	for (int i = 0; i <= config::cube_data::LAST_CUBE; ++i) {
		m_selectedCubeThumbnails.push_back(make_shared<Sprite>(
				Sprite(380, 5, 2, 40, 40,
						Resources::getInstance().getTexture(
								config::cube_data::thumbnails[i]))));
	}

}

// ########################################################
// Member Functions########################################
// ########################################################

void InGame::update(float timePassed) {

	auto input = Input::getInstance();
	auto &mouse = Mouse::getInstance();

	if (input->escapeKeyPressed)
		m_state = GameState::OverlayMenu;

	if (m_state == GameState::NoOverlay) {
		mouse.lock();
		input->updateValues();
		mouse.update();
		m_player.update(timePassed);

		SpriteBatcher::getInstance().addBatch(m_crossHair);

		auto &res = Resources::getInstance();
		FontMeshBuilder &fontMeshBuilder = res.getFontMeshBuilder(
				config::font_data::fontLayout, config::font_data::fontAtlasWidth,
				config::font_data::fontAtlasHeight);

		vec3 dir = m_player.getViewingDirection();
		string derp =
				"Vied direction: " +
				to_string(dir.x) + ", " +
				to_string(dir.y) + ", "	+
				to_string(dir.z);

		m_direction.reset(
				new Sprite(0, 20, 10,
						fontMeshBuilder.buldMeshForString(derp, 20),
						res.getTexture(config::font_data::font)));

		m_fps.reset(
			new Sprite(0, 45, 10,
					fontMeshBuilder.buldMeshForString(
							"FPS: " + to_string(util::FPSManager::getFps()), 20),
					res.getTexture(config::font_data::font)));


		vec3 ses = m_player.getLastSelectedCube();
		string soos = "Last Selected: " + to_string(ses.x) +
				", " + to_string(ses.y) + ", " + to_string(ses.z);
		m_lastSelecteCube.reset(
			new Sprite(0, 70, 10,
					fontMeshBuilder.buldMeshForString(
							soos, 20),
					res.getTexture(config::font_data::font)));

		SpriteBatcher::getInstance().addBatch(m_direction);
		SpriteBatcher::getInstance().addBatch(m_fps);
		SpriteBatcher::getInstance().addBatch(m_lastSelecteCube);

		SpriteBatcher::getInstance().addBatch(
				m_selectedCubeThumbnails[m_player.getSelectedCube()]);


	} else {
		mouse.unlock();
		input->updateValues();
		mouse.update();
		mouse.draw();
	}

	glDisable (GL_DEPTH_TEST);

	//skybox.render();

	if (m_state == GameState::OverlayMenu) {
		m_widgetGroup1->update(timePassed);
		m_widgetGroup1->draw();
	}

	// TODO Should be in the batchers?!?
	glEnable (GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	ChunkBatcher::getInstance().draw();
	CubeBatcher::getInstance().draw();
	SpriteBatcher::getInstance().draw();
}
