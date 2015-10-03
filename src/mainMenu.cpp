
#include "mainMenu.h"

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

	widgetGroup1.reset(new widget::WidgetGroup{0, 0, 0, 800, 600, observer});

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

	double y = input.mouseYPosition - 1080;
	if (y < 0) {
		y = -y;
	} else {
		y = -1;
	}

	glm::vec2 mouse = gui::adjustMouse(800, 600, 1920, 1080, input.mouseXPosition, y);

	widgetGroup1->mouseMoved(mouse.x, mouse.y);
	if (input.action1Pressed) {
		widgetGroup1->mouseClicked(0, mouse.x, mouse.y);
	}
	widgetGroup1->draw();

	graphics::SpriteBatcher::getInstance().draw();
}

