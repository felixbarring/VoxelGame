
#include "inGame.h"

#include "graphics/chunkBatcher.h"
#include "graphics/cubeBatcher.h"

#include "config/data.h"

// ########################################################
// Constructor/Destructor #################################
// ########################################################

InGame::InGame(Game *game, util::Input &in) :
	input(in),
	player(in),
	game(game),
	skybox{graphics::Resources::getInstance().getTextureCubeMap(
			config::cube_map_data::cubeMap1[0],
			config::cube_map_data::cubeMap1[1],
			config::cube_map_data::cubeMap1[2],
			config::cube_map_data::cubeMap1[3],
			config::cube_map_data::cubeMap1[4],
			config::cube_map_data::cubeMap1[5],
			config::cube_map_data::cubeMap1Width,
			config::cube_map_data::cubeMap1Height)}
{
	player.setLocation(7, 7, 7);

	std::function<void(int)> observer = [&, game](int id)
	{
		switch(id) {
			case 0: {
				game->changeStateToMainMenu();
				state = GameState::NoOverlay;
				break;
			}
			case 1: {
				input.centerMouse();
				state = GameState::NoOverlay;
				break;
			}
		}
		std::cout << "A button with id: " << id << " was pressed\n";
	};

	std::shared_ptr<widget::IWidget> button1(new widget::Button{0, 325, 350, 150, 30, observer, "Main Menu"});
	std::shared_ptr<widget::IWidget> button2(new widget::Button{1, 325, 310, 150, 30, observer, "Back To Game"});
	widgetGroup1.reset(new widget::WidgetGroup{0, 300, 300, 200, 90, observer});

	widgetGroup1->addWidget(button1);
	widgetGroup1->addWidget(button2);

}

// ########################################################
// Member Functions########################################
// ########################################################

void InGame::update()
{

	if (input.escapeKeyPressed) {
		state = GameState::OverlayMenu;
	}

	if (state == GameState::NoOverlay) {
		input.lockMouse();
		input.updateValues();
		player.update(0.0f);
	} else {
		input.unlockMouse();
		input.updateValues();
	}

	glDisable(GL_DEPTH_TEST);

	skybox.render();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	graphics::ChunkBatcher::getInstance().draw();
	graphics::CubeBatcher::getInstance().draw();

	if (state == GameState::OverlayMenu) {

		double y = input.mouseYPosition - config::graphics_data::windowHeight;
		if (y < 0) {
			y = -y;
		} else {
			y = -1;
		}

		glm::vec2 mouse = gui::adjustMouse(config::graphics_data::virtualWidth, config::graphics_data::virtualHeight,
				config::graphics_data::windowWidth, config::graphics_data::windowHeight, input.mouseXPosition, y);

		widgetGroup1->mouseMoved(mouse.x, mouse.y);
		if (input.action1Pressed) {
			widgetGroup1->mouseClicked(0, mouse.x, mouse.y);
		}

		widgetGroup1->draw();

		graphics::SpriteBatcher::getInstance().draw();
	}
}