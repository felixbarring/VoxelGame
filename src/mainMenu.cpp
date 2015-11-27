
#include "mainMenu.h"

#include <iostream>

#include "config/data.h"

// ########################################################
// Constructor/Destructor #################################
// ########################################################

MainMenu::MainMenu(Game *game) :
	game(game)
{

	title.reset(new gui::Image{200, 450, 400, 100, config::gui_data::title});

	bool quit = false;
	std::function<void(int)> observer = [&, game](int id)
	{
		switch(id) {
			case 0: state = MainMenuState::Play; break;
			case 1: state = MainMenuState::Settings; break;
			case 2: game->quitGame(); break;
			case 3: state = MainMenuState::NewWorld; break;// ;
			case 4: state = MainMenuState::LoadWorld; break;
			case 5: state = MainMenuState::MainMenu; break;
			case 6: state = MainMenuState::GameSettings; break;
			case 7: state = MainMenuState::InputSettings; break;
			case 8: state = MainMenuState::GraphicsSettings; break;
			case 9: state = MainMenuState::MainMenu; break;
			case 10: state = MainMenuState::Settings; break;
			case 11: state = MainMenuState::Settings; break;
			case 12: state = MainMenuState::Settings; break;
			case 13: state = MainMenuState::Settings; break;
			case 14: break; //game->changeStateToIngame(); state = MainMenuState::MainMenu; break;
			case 15: game->changeStateToIngame(); state = MainMenuState::MainMenu; break;
			case 16: state = MainMenuState::Play; break;
			case 17: break;
			case 18: break;
			case 19: break;
			case 20: break;
			case 21: state = MainMenuState::Play; break;
		}

	};

	std::function<void(int, float)> observer2 = [&](int id, float value)
	{
		//std::cout << value << "\n";
		switch (id) {
			case 100: {
				textInput->setString(std::to_string(value));
				break;
			}
			case 101: {
				textInput2->setString(std::to_string(value));
				break;
			}
		}
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

	std::shared_ptr<widget::IWidget> label4(new widget::Label{325, 390, 150, 30, " - Game - "});
	std::shared_ptr<widget::IWidget> button11(new widget::Button{10, 325, 230, 150, 30, observer, "Back"});

	gameSettingsWidgetGroup.reset(new widget::WidgetGroup{0, 300, 220, 200, 170, observer});

	gameSettingsWidgetGroup->addWidget(label4);
	gameSettingsWidgetGroup->addWidget(button11);

	// ################################################################################################################

	std::shared_ptr<widget::IWidget> label5(new widget::Label{325, 390, 150, 30, " - Input - "});
	std::shared_ptr<widget::IWidget> button12(new widget::Button{11, 325, 230, 150, 30, observer, "Back"});

	inputSettingsWidgetGroup.reset(new widget::WidgetGroup{0, 300, 220, 200, 170, observer});

	inputSettingsWidgetGroup->addWidget(label5);
	inputSettingsWidgetGroup->addWidget(button12);

	// ################################################################################################################

	std::shared_ptr<widget::IWidget> label6(new widget::Label{325, 390, 150, 30, " - Settings - Game "});

	std::shared_ptr<widget::IWidget> label7(new widget::Label{             30, 310, 80,  20, "FOV:"});
	std::shared_ptr<widget::IWidget> slider(new widget::Slider{      100,  125, 310, 150, 30, observer2});
	textInput.reset(new widget::TextInput{666, 285, 310, 60,  30});

	std::shared_ptr<widget::IWidget> label8(new widget::Label{              30, 270, 80, 20, "Render Distance:"});
	std::shared_ptr<widget::IWidget> slider2(new widget::Slider{      101,  125, 270, 150, 30, observer2});
	textInput2.reset(new widget::TextInput{666, 285, 270, 60, 30});

	std::shared_ptr<widget::IWidget> button13(new widget::Button{12, 245, 130, 150, 30, observer, "Save"});
	std::shared_ptr<widget::IWidget> button14(new widget::Button{13, 400, 130, 150, 30, observer, "Cancel"});

	graphicsSettingsWidgetGroup.reset(new widget::WidgetGroup{0, 0, 120, 800, 270, observer});

	graphicsSettingsWidgetGroup->addWidget(label6);

	graphicsSettingsWidgetGroup->addWidget(label7);
	graphicsSettingsWidgetGroup->addWidget(slider);
	graphicsSettingsWidgetGroup->addWidget(textInput);

	graphicsSettingsWidgetGroup->addWidget(label8);
	graphicsSettingsWidgetGroup->addWidget(slider2);
	graphicsSettingsWidgetGroup->addWidget(textInput2);

	graphicsSettingsWidgetGroup->addWidget(button13);
	graphicsSettingsWidgetGroup->addWidget(button14);

	// ################################################################################################################

	std::shared_ptr<widget::IWidget> label15(new widget::Label{230, 390, 150, 50, " - Create New World - "});
	std::shared_ptr<widget::IWidget> label16(new widget::Label{220, 330, 150, 30, "Enter a Name:"});
	std::shared_ptr<widget::IWidget> textInput3(new widget::TextInput{666, 370, 330, 200, 30});
	std::shared_ptr<widget::IWidget> button15(new widget::Button{14, 240, 135, 100, 30, observer, "Advanced"});
	std::shared_ptr<widget::IWidget> button16(new widget::Button{15, 350, 135, 100, 30, observer, "Create"});
	std::shared_ptr<widget::IWidget> button17(new widget::Button{16, 460, 135, 100, 30, observer, "Cancel"});

	std::shared_ptr<widget::IWidget> button18(new widget::Button{17, 370, 290, 200, 30, observer, "Random Name"});


	newWorldWidgetGroup.reset(new widget::WidgetGroup{0, 200, 120, 400, 270, observer});

	newWorldWidgetGroup->addWidget(label15);
	newWorldWidgetGroup->addWidget(label16);
	newWorldWidgetGroup->addWidget(textInput3);
	newWorldWidgetGroup->addWidget(button15);
	newWorldWidgetGroup->addWidget(button16);
	newWorldWidgetGroup->addWidget(button17);
	newWorldWidgetGroup->addWidget(button18);


	// ################################################################################################################

	std::shared_ptr<widget::IWidget> label17(new widget::Label{270, 390, 150, 50, " - Load World - "});

	std::shared_ptr<widget::IWidget> button19(new widget::Button{18, 225, 70, 80, 30, observer, "Rename"});
	std::shared_ptr<widget::IWidget> button20(new widget::Button{19, 315, 70, 80, 30, observer, "Delete"});

	std::shared_ptr<widget::IWidget> button21(new widget::Button{20, 405, 70, 80, 30, observer, "Load"});
	std::shared_ptr<widget::IWidget> button22(new widget::Button{21, 495, 70, 80, 30, observer, "Cancel"});

	loadWorldWidgetGroup.reset(new widget::WidgetGroup{0, 200, 50, 400, 400, observer});

	loadWorldWidgetGroup->addWidget(label17);
	loadWorldWidgetGroup->addWidget(button19);
	loadWorldWidgetGroup->addWidget(button20);
	loadWorldWidgetGroup->addWidget(button21);
	loadWorldWidgetGroup->addWidget(button22);

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
		case MainMenuState::NewWorld: {
			activeWidgetGroup = newWorldWidgetGroup;
			break;
		}
		case MainMenuState::LoadWorld: {
			activeWidgetGroup = loadWorldWidgetGroup;
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
			break;
		}
		case MainMenuState::InputSettings: {
			activeWidgetGroup = inputSettingsWidgetGroup;
			break;
		}
		case MainMenuState::GraphicsSettings: {
			activeWidgetGroup = graphicsSettingsWidgetGroup;
		}
	}

	activeWidgetGroup->update();
	activeWidgetGroup->mouseMoved(mouse.x, mouse.y);

	if (input->keyWasTyped)
		activeWidgetGroup->keyTyped(input->keyTyped);

	if (input->action1Pressed)
		activeWidgetGroup->mouseClicked(0, mouse.x, mouse.y);

	title->draw();
	activeWidgetGroup->draw();


	graphics::SpriteBatcher::getInstance().draw();
}

