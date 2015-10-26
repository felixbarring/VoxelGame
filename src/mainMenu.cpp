
#include "mainMenu.h"

#include <iostream>

#include "config/data.h"

// ########################################################
// Constructor/Destructor #################################
// ########################################################

MainMenu::MainMenu(Game *game) :
	game(game)
{

	bool quit = false;
	std::function<void(int)> observer = [&, game](int id)
	{
		switch(id) {
			case 0: {
				state = MainMenuState::Play;
				break;
			}
			case 1: {
				state = MainMenuState::Settings;
				break;
			}
			case 2: {
				game->quitGame();
				break;
			}
			case 3: {
				game->changeStateToIngame();
				state = MainMenuState::MainMenu;
				break;
			}
			case 5: {
				state = MainMenuState::MainMenu;
				break;
			}
			case 6: {
				state = MainMenuState::GameSettings;
				break;
			}
			case 9: {
				state = MainMenuState::MainMenu;
				break;
			}
			case 10: {
				state = MainMenuState::Settings;
				break;
			}
		}

	};

	std::function<void(int, float)> observer2 = [&](int id, float value)
	{
		std::cout << value << "\n";
	};

	// ################################################################################################################

	std::shared_ptr<widget::IWidget> label1(new widget::Label{325, 390, 150, 30, " - Main - "});
	std::shared_ptr<widget::IWidget> button1(new widget::Button{0, 325, 350, 150, 30, observer, "Play"});
	std::shared_ptr<widget::IWidget> button2(new widget::Button{1, 325, 310, 150, 30, observer, "Settings"});
	std::shared_ptr<widget::IWidget> button3(new widget::Button{2, 325, 270, 150, 30, observer, "Quit"});

	mainWidgetGroup.reset(new widget::WidgetGroup{0, 300, 260, 200, 130, observer});

	mainWidgetGroup->addWidget(label1);
	mainWidgetGroup->addWidget(button1);
	mainWidgetGroup->addWidget(button2);
	mainWidgetGroup->addWidget(button3);

	// ################################################################################################################

	std::shared_ptr<widget::IWidget> label2(new widget::Label{325, 390, 150, 30, " - Play - "});
	std::shared_ptr<widget::IWidget> button4(new widget::Button{3, 325, 350, 150, 30, observer, "New World"});
	std::shared_ptr<widget::IWidget> button5(new widget::Button{4, 325, 310, 150, 30, observer, "Load World"});
	std::shared_ptr<widget::IWidget> button6(new widget::Button{5, 325, 270, 150, 30, observer, "Back"});

	playWidgetGroup.reset(new widget::WidgetGroup{0, 300, 260, 200, 130, observer});

	playWidgetGroup->addWidget(label2);
	playWidgetGroup->addWidget(button4);
	playWidgetGroup->addWidget(button5);
	playWidgetGroup->addWidget(button6);

	// ################################################################################################################

	std::shared_ptr<widget::IWidget> label3(new widget::Label{325, 390, 150, 30, " - Settings - "});
	std::shared_ptr<widget::IWidget> button7(new widget::Button{6, 325, 350, 150, 30, observer, "Game"});
	std::shared_ptr<widget::IWidget> button8(new widget::Button{7, 325, 310, 150, 30, observer, "Input"});
	std::shared_ptr<widget::IWidget> button9(new widget::Button{8, 325, 270, 150, 30, observer, "Graphics"});
	std::shared_ptr<widget::IWidget> button10(new widget::Button{9, 325, 230, 150, 30, observer, "Back"});

	settingsWidgetGroup.reset(new widget::WidgetGroup{0, 300, 220, 200, 170, observer});

	settingsWidgetGroup->addWidget(label3);
	settingsWidgetGroup->addWidget(button7);
	settingsWidgetGroup->addWidget(button8);
	settingsWidgetGroup->addWidget(button9);
	settingsWidgetGroup->addWidget(button10);

	// ################################################################################################################

	std::shared_ptr<widget::IWidget> label4(new widget::Label{325, 390, 150, 30, " - Settings - Game "});

	std::shared_ptr<widget::IWidget> label5(new widget::Label{             30, 310, 80,  20, "FOV:"});
	std::shared_ptr<widget::IWidget> slider(new widget::Slider{      666,  125, 310, 150, 30, observer2});
	std::shared_ptr<widget::IWidget> textInput(new widget::TextInput{666, 285, 310, 60,  30});

	std::shared_ptr<widget::IWidget> label6(new widget::Label{              30, 270, 80, 20, "Render Distance:"});
	std::shared_ptr<widget::IWidget> slider2(new widget::Slider{      666,  125, 270, 150, 30, observer2});
	std::shared_ptr<widget::IWidget> textInput2(new widget::TextInput{666, 285, 270, 60, 30});



	std::shared_ptr<widget::IWidget> button11(new widget::Button{10, 325, 230, 150, 30, observer, "Back"});

	gameSettingsWidgetGroup.reset(new widget::WidgetGroup{0, 0, 120, 800, 270, observer});

	gameSettingsWidgetGroup->addWidget(label4);

	gameSettingsWidgetGroup->addWidget(label5);
	gameSettingsWidgetGroup->addWidget(slider);
	gameSettingsWidgetGroup->addWidget(textInput);

	gameSettingsWidgetGroup->addWidget(label6);
	gameSettingsWidgetGroup->addWidget(slider2);
	gameSettingsWidgetGroup->addWidget(textInput2);

	gameSettingsWidgetGroup->addWidget(button11);

	// ################################################################################################################

	activeWidgetGroup = mainWidgetGroup;

}

// ########################################################
// Member Functions########################################
// ########################################################

void MainMenu::update()
{

	std::shared_ptr<util::Input> input = util::Input::getInstance();

	input->unlockMouse();
	input->updateValues();

	double y = input->mouseYPosition - config::graphics_data::windowHeight;
	if (y < 0) {
		y = -y;
	} else {
		y = -1;
	}

	glm::vec2 mouse = gui::adjustMouse(config::graphics_data::virtualWidth, config::graphics_data::virtualHeight,
			config::graphics_data::windowWidth, config::graphics_data::windowHeight, input->mouseXPosition, y);

	switch (state) {
		case MainMenuState::MainMenu: {
			activeWidgetGroup = mainWidgetGroup;
			break;
		}
		case MainMenuState::Play: {
			activeWidgetGroup = playWidgetGroup;
			break;
		}
		case MainMenuState::Settings: {
			activeWidgetGroup = settingsWidgetGroup;
			break;
		}
		case MainMenuState::GameSettings: {
			activeWidgetGroup = gameSettingsWidgetGroup;
		}
	}

	activeWidgetGroup->update();
	activeWidgetGroup->mouseMoved(mouse.x, mouse.y);

	if (input->keyWasTyped)
		activeWidgetGroup->keyTyped(input->keyTyped);

	if (input->action1Pressed)
		activeWidgetGroup->mouseClicked(0, mouse.x, mouse.y);

	activeWidgetGroup->draw();


	graphics::SpriteBatcher::getInstance().draw();
}

