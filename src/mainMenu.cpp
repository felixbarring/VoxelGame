
#include "mainMenu.h"

#include "config/data.h"

// ########################################################
// Constructor/Destructor #################################
// ########################################################

MainMenu::MainMenu(Game *game, util::Input &in) :
	input(in),
	game(game)
{

	bool quit = false;
	std::function<void(int)> observer = [game](int id)
	{
		switch(id) {
			case 0: {
				game->changeStateToIngame();
				break;
			}
			case 2: {
				game->quitGame();
				break;
			}
		}
		std::cout << "A button with id: " << id << " was pressed\n";
	};

	std::shared_ptr<widget::IWidget> button1(new widget::Button{0, 325, 350, 150, 30, observer, "Single Player"});
	std::shared_ptr<widget::IWidget> button2(new widget::Button{1, 325, 310, 150, 30, observer, "Settings"});
	std::shared_ptr<widget::IWidget> button3(new widget::Button{2, 325, 270, 150, 30, observer, "Quit"});

	widgetGroup1.reset(new widget::WidgetGroup{0, 300, 260, 200, 130, observer});

	widgetGroup1->addWidget(button1);
	widgetGroup1->addWidget(button2);
	widgetGroup1->addWidget(button3);


}

// ########################################################
// Member Functions########################################
// ########################################################

void MainMenu::update()
{
	input.unlockMouse();
	input.updateValues();

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

