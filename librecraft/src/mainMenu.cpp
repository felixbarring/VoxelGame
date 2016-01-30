
#include "mainMenu.h"

#include <iostream>
#include <string>

#include "config/data.h"
#include "util/worldMeta.h"

using namespace std;

// ########################################################
// Constructor/Destructor #################################
// ########################################################

MainMenu::MainMenu(Game *game) :
	game(game)
{

	title.reset(new gui::Image{200, 450, 400, 100, config::gui_data::title});

	bool quit = false;
	function<void(int)> observer = [&, game](int id)
	{
		switch(id) {
			case 0: activeWidgetGroup = playWidgetGroup;break;
			case 1: activeWidgetGroup = settingsWidgetGroup;break;
			case 2: game->quitGame(); break;
			case 3: activeWidgetGroup = newWorldWidgetGroup; break;
			case 4: activeWidgetGroup = loadWorldWidgetGroup; break;
			case 5: activeWidgetGroup = mainWidgetGroup; break;
			case 6: activeWidgetGroup = gameSettingsWidgetGroup; break;
			case 7: activeWidgetGroup = inputSettingsWidgetGroup; break;
			case 8: activeWidgetGroup = graphicsSettingsWidgetGroup; break;
			case 9: activeWidgetGroup = mainWidgetGroup; break;
			case 10: activeWidgetGroup = settingsWidgetGroup; break;
			case 11: activeWidgetGroup = settingsWidgetGroup; break;
			case 12: activeWidgetGroup = settingsWidgetGroup; break;
			case 13: activeWidgetGroup = settingsWidgetGroup; break;
			case 14: break;
			case 15: {
				string name{textInput3->getString()};
				if (name.size()) {
					if (!world_meta::worldNameExists(name)){
						world_meta::addName(name);
						game->createNewWorld(name);
						activeWidgetGroup = mainWidgetGroup;
					} else
					{
						// Error: Name already taken
					}

				} else {
					// Name can not be empty
				}
				break;
			}
			case 16: activeWidgetGroup = playWidgetGroup; break;
			case 17: break;
			case 18: break;
			case 19: break;
			case 20: break;
			case 21: activeWidgetGroup = playWidgetGroup; break;
		}

	};

	function<void(int, float)> observer2 = [&](int id, float value)
	{
		//cout << value << "\n";
		switch (id) {
			case 100: {
				textInput->setString(to_string(value));
				break;
			}
			case 101: {
				textInput2->setString(to_string(value));
				break;
			}
		}
	};

	// TODO Give the variables better names

	// ################################################################################################################

	shared_ptr<widget::IWidget> label1(new widget::Label{325, 390, 150, 30, " - Main - "});
	shared_ptr<widget::IWidget> button1(new widget::Button{0, 325, 350, 150, 30, observer, "Play"});
	shared_ptr<widget::IWidget> button2(new widget::Button{1, 325, 310, 150, 30, observer, "Settings"});
	shared_ptr<widget::IWidget> button3(new widget::Button{2, 325, 270, 150, 30, observer, "Quit"});

	mainWidgetGroup.reset(new widget::WidgetGroup{0, 300, 260, 200, 130, observer});

	mainWidgetGroup->addWidget(label1);
	mainWidgetGroup->addWidget(button1);
	mainWidgetGroup->addWidget(button2);
	mainWidgetGroup->addWidget(button3);

	// ################################################################################################################

	shared_ptr<widget::IWidget> label2(new widget::Label{325, 390, 150, 30, " - Play - "});
	shared_ptr<widget::IWidget> button4(new widget::Button{3, 325, 350, 150, 30, observer, "New World"});
	shared_ptr<widget::IWidget> button5(new widget::Button{4, 325, 310, 150, 30, observer, "Load World"});
	shared_ptr<widget::IWidget> button6(new widget::Button{5, 325, 270, 150, 30, observer, "Back"});

	playWidgetGroup.reset(new widget::WidgetGroup{0, 300, 260, 200, 130, observer});

	playWidgetGroup->addWidget(label2);
	playWidgetGroup->addWidget(button4);
	playWidgetGroup->addWidget(button5);
	playWidgetGroup->addWidget(button6);

	// ################################################################################################################

	shared_ptr<widget::IWidget> label3(new widget::Label{325, 390, 150, 30, " - Settings - "});
	shared_ptr<widget::IWidget> button7(new widget::Button{6, 325, 350, 150, 30, observer, "Game"});
	shared_ptr<widget::IWidget> button8(new widget::Button{7, 325, 310, 150, 30, observer, "Input"});
	shared_ptr<widget::IWidget> button9(new widget::Button{8, 325, 270, 150, 30, observer, "Graphics"});
	shared_ptr<widget::IWidget> button10(new widget::Button{9, 325, 230, 150, 30, observer, "Back"});

	settingsWidgetGroup.reset(new widget::WidgetGroup{0, 300, 220, 200, 170, observer});

	settingsWidgetGroup->addWidget(label3);
	settingsWidgetGroup->addWidget(button7);
	settingsWidgetGroup->addWidget(button8);
	settingsWidgetGroup->addWidget(button9);
	settingsWidgetGroup->addWidget(button10);

	// ################################################################################################################

	shared_ptr<widget::IWidget> label4(new widget::Label{325, 390, 150, 30, " - Game - "});
	shared_ptr<widget::IWidget> button11(new widget::Button{10, 325, 230, 150, 30, observer, "Back"});

	gameSettingsWidgetGroup.reset(new widget::WidgetGroup{0, 300, 220, 200, 170, observer});

	gameSettingsWidgetGroup->addWidget(label4);
	gameSettingsWidgetGroup->addWidget(button11);

	// ################################################################################################################

	shared_ptr<widget::IWidget> label5(new widget::Label{325, 390, 150, 30, " - Input - "});
	shared_ptr<widget::IWidget> button12(new widget::Button{11, 325, 230, 150, 30, observer, "Back"});

	inputSettingsWidgetGroup.reset(new widget::WidgetGroup{0, 300, 220, 200, 170, observer});

	inputSettingsWidgetGroup->addWidget(label5);
	inputSettingsWidgetGroup->addWidget(button12);

	// ################################################################################################################

	shared_ptr<widget::IWidget> label6(new widget::Label{325, 390, 150, 30, " - Settings - Game "});

	shared_ptr<widget::IWidget> label7(new widget::Label{             30+100, 310, 80, 20, "FOV:"});
	shared_ptr<widget::IWidget> slider(new widget::Slider{      100,  125+100, 310, 150, 30, observer2});
	textInput.reset(new widget::TextInput{666, 285+100, 310, 100,  30});

	shared_ptr<widget::IWidget> label8(new widget::Label{              30+100, 270, 80, 20, "Render Distance:"});
	shared_ptr<widget::IWidget> slider2(new widget::Slider{      101,  125+100, 270, 150, 30, observer2});
	textInput2.reset(new widget::TextInput{666, 285+100, 270, 100, 30});

	shared_ptr<widget::IWidget> button13(new widget::Button{12, 245, 130, 150, 30, observer, "Save"});
	shared_ptr<widget::IWidget> button14(new widget::Button{13, 400, 130, 150, 30, observer, "Cancel"});

	graphicsSettingsWidgetGroup.reset(new widget::WidgetGroup{0, 100, 100, 600, 250, observer});

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

	shared_ptr<widget::IWidget> label15(new widget::Label{230, 390, 150, 50, " - Create New World - "});
	shared_ptr<widget::IWidget> label16(new widget::Label{220, 330, 150, 30, "Enter a Name:"});
	textInput3.reset(new widget::TextInput{666, 370, 330, 200, 30});
	shared_ptr<widget::IWidget> button15(new widget::Button{14, 240, 135, 100, 30, observer, "Advanced"});
	shared_ptr<widget::IWidget> button16(new widget::Button{15, 350, 135, 100, 30, observer, "Create"});
	shared_ptr<widget::IWidget> button17(new widget::Button{16, 460, 135, 100, 30, observer, "Cancel"});

	shared_ptr<widget::IWidget> button18(new widget::Button{17, 370, 290, 200, 30, observer, "Random Name"});


	newWorldWidgetGroup.reset(new widget::WidgetGroup{0, 200, 120, 400, 270, observer});

	newWorldWidgetGroup->addWidget(label15);
	newWorldWidgetGroup->addWidget(label16);
	newWorldWidgetGroup->addWidget(textInput3);
	newWorldWidgetGroup->addWidget(button15);
	newWorldWidgetGroup->addWidget(button16);
	newWorldWidgetGroup->addWidget(button17);
	newWorldWidgetGroup->addWidget(button18);


	// ################################################################################################################

	shared_ptr<widget::IWidget> label17(new widget::Label{270, 390, 150, 50, " - Load World - "});

	shared_ptr<widget::IWidget> button19(new widget::Button{18, 225, 70, 80, 30, observer, "Rename"});
	shared_ptr<widget::IWidget> button20(new widget::Button{19, 315, 70, 80, 30, observer, "Delete"});

	shared_ptr<widget::IWidget> button21(new widget::Button{20, 405, 70, 80, 30, observer, "Load"});
	shared_ptr<widget::IWidget> button22(new widget::Button{21, 495, 70, 80, 30, observer, "Cancel"});

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

void MainMenu::update(float timePassed)
{

	shared_ptr<util::Input> input = util::Input::getInstance();

	input->unlockMouse();
	input->updateValues();

	double y = input->mouseYPosition - config::graphics_data::windowHeight;
	if (y < 0) {
		y = -y;
	} else {
		y = -1;
	}

	glm::vec2 mouse = gui::adjustMouse(
			config::graphics_data::virtualWidth,
			config::graphics_data::virtualHeight,
			config::graphics_data::windowWidth,
			config::graphics_data::windowHeight,
			input->mouseXPosition, y);

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

