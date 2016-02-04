
#include "mainMenu.h"

#include <iostream>
#include <string>

#include "config/data.h"
#include "util/worldMeta.h"

using namespace std;
using namespace widget;

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
						m_worldList->addListItem(name);
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
			case 20: {
				if (m_worldList->getSelectedListItem().size()) {
					string name =  m_worldList->getSelectedListItem();
					game->loadExistingWorld(
							m_worldList->getSelectedListItem());
					activeWidgetGroup = mainWidgetGroup;
					m_worldList->reset();
				}
				break;
			}
			case 21: activeWidgetGroup = playWidgetGroup; break;

			case 100: {
				textInput->setString(to_string(slider->getValue()));
				break;
			}
			case 101: {
				textInput2->setString(to_string(slider2->getValue()));
				break;
			}
		}

	};

	// TODO Give the variables better names

	// ########################################################################

	shared_ptr<IWidget> label1(
			new Label{325, 390, 150, 30, " - Main - "});
	shared_ptr<IWidget> button1(
			new Button{0, 325, 350, 150, 30, observer, "Play"});
	shared_ptr<IWidget> button2(
			new Button{1, 325, 310, 150, 30, observer, "Settings"});
	shared_ptr<IWidget> button3(
			new Button{2, 325, 270, 150, 30, observer, "Quit"});

	mainWidgetGroup.reset(new WidgetGroup{0, 300, 260, 200, 130, observer});

	mainWidgetGroup->addWidget(label1);
	mainWidgetGroup->addWidget(button1);
	mainWidgetGroup->addWidget(button2);
	mainWidgetGroup->addWidget(button3);

	// ########################################################################

	shared_ptr<IWidget> label2(
			new Label{325, 390, 150, 30, " - Play - "});
	shared_ptr<IWidget> button4(
			new Button{3, 325, 350, 150, 30, observer, "New World"});
	shared_ptr<IWidget> button5(
			new Button{4, 325, 310, 150, 30, observer, "Load World"});
	shared_ptr<IWidget> button6(
			new Button{5, 325, 270, 150, 30, observer, "Back"});

	playWidgetGroup.reset(new WidgetGroup{0, 300, 260, 200, 130, observer});

	playWidgetGroup->addWidget(label2);
	playWidgetGroup->addWidget(button4);
	playWidgetGroup->addWidget(button5);
	playWidgetGroup->addWidget(button6);

	// ########################################################################

	shared_ptr<IWidget> label3(
			new Label{325, 390, 150, 30, " - Settings - "});
	shared_ptr<IWidget> button7(
			new Button{6, 325, 350, 150, 30, observer, "Game"});
	shared_ptr<IWidget> button8(
			new Button{7, 325, 310, 150, 30, observer, "Input"});
	shared_ptr<IWidget> button9(
			new Button{8, 325, 270, 150, 30, observer, "Graphics"});
	shared_ptr<IWidget> button10(
			new Button{9, 325, 230, 150, 30, observer, "Back"});

	settingsWidgetGroup.reset(new WidgetGroup{0, 300, 220, 200, 170, observer});

	settingsWidgetGroup->addWidget(label3);
	settingsWidgetGroup->addWidget(button7);
	settingsWidgetGroup->addWidget(button8);
	settingsWidgetGroup->addWidget(button9);
	settingsWidgetGroup->addWidget(button10);

	// ########################################################################

	shared_ptr<IWidget> label4(
			new Label{325, 390, 150, 30, " - Game - "});
	shared_ptr<IWidget> button11(
			new Button{10, 325, 230, 150, 30, observer, "Back"});

	gameSettingsWidgetGroup.reset(
			new WidgetGroup{0, 300, 220, 200, 170, observer});

	gameSettingsWidgetGroup->addWidget(label4);
	gameSettingsWidgetGroup->addWidget(button11);

	// ########################################################################

	shared_ptr<IWidget> label5(
			new Label{325, 390, 150, 30, " - Input - "});
	shared_ptr<IWidget> button12(
			new Button{11, 325, 230, 150, 30, observer, "Back"});

	inputSettingsWidgetGroup.reset(
			new WidgetGroup{0, 300, 220, 200, 170, observer});

	inputSettingsWidgetGroup->addWidget(label5);
	inputSettingsWidgetGroup->addWidget(button12);

	// ########################################################################

	shared_ptr<IWidget> label6(
			new Label{325, 390, 150, 30, " - Settings - Game "});

	shared_ptr<IWidget> label7(
			new Label{30 + 100, 310, 80, 20, "FOV:"});
	slider.reset(new Slider{100, 125 + 100, 310, 150, 30, observer});
	textInput.reset(new TextInput{666,285 + 100, 310, 100,  30});

	shared_ptr<IWidget> label8(
			new Label{30 + 100, 270, 80, 20, "Render Distance:"});
	slider2.reset(
			new Slider{101, 125 + 100, 270, 150, 30, observer});
	textInput2.reset(new TextInput{666, 285 + 100, 270, 100, 30});

	shared_ptr<IWidget> button13(
			new Button{12, 245, 130, 150, 30, observer, "Save"});
	shared_ptr<IWidget> button14(
			new Button{13, 400, 130, 150, 30, observer, "Cancel"});

	graphicsSettingsWidgetGroup.reset(
			new WidgetGroup{0, 100, 100, 600, 250, observer});

	graphicsSettingsWidgetGroup->addWidget(label6);

	graphicsSettingsWidgetGroup->addWidget(label7);
	graphicsSettingsWidgetGroup->addWidget(slider);
	graphicsSettingsWidgetGroup->addWidget(textInput);

	graphicsSettingsWidgetGroup->addWidget(label8);
	graphicsSettingsWidgetGroup->addWidget(slider2);
	graphicsSettingsWidgetGroup->addWidget(textInput2);

	graphicsSettingsWidgetGroup->addWidget(button13);
	graphicsSettingsWidgetGroup->addWidget(button14);

	// ########################################################################

	shared_ptr<IWidget> label15(
			new Label{230, 390, 150, 50, " - Create New World - "});
	shared_ptr<IWidget> label16(
			new Label{250, 330, 150, 30, "Enter a Name:"});
	textInput3.reset(new TextInput{666, 250, 290, 300, 30});
	shared_ptr<IWidget> button15(
			new Button{14, 240, 135, 100, 30, observer, "Advanced"});
	shared_ptr<IWidget> button16(
			new Button{15, 350, 135, 100, 30, observer, "Create"});
	shared_ptr<IWidget> button17(
			new Button{16, 460, 135, 100, 30, observer, "Cancel"});

	shared_ptr<IWidget> button18(
			new Button{17, 250, 250, 300, 30, observer, "Random Name"});


	newWorldWidgetGroup.reset(new WidgetGroup{0, 200, 120, 400, 270, observer});

	newWorldWidgetGroup->addWidget(label15);
	newWorldWidgetGroup->addWidget(label16);
	newWorldWidgetGroup->addWidget(textInput3);
	newWorldWidgetGroup->addWidget(button15);
	newWorldWidgetGroup->addWidget(button16);
	newWorldWidgetGroup->addWidget(button17);
	newWorldWidgetGroup->addWidget(button18);


	// ########################################################################

	shared_ptr<IWidget> label17(
			new Label{270, 390, 150, 50, " - Load World - "});

	shared_ptr<IWidget> button19(
			new Button{18, 225, 70, 80, 30, observer, "Rename"});
	shared_ptr<IWidget> button20(
			new Button{19, 315, 70, 80, 30, observer, "Delete"});

	shared_ptr<IWidget> button21(
			new Button{20, 405, 70, 80, 30, observer, "Load"});
	shared_ptr<IWidget> button22(
			new Button{21, 495, 70, 80, 30, observer, "Cancel"});

	loadWorldWidgetGroup.reset(
			new WidgetGroup{0, 200, 50, 400, 400, observer});

	m_worldList.reset(
			new SelectableList{666, 220, 100, 360, 300, observer, 2});

	for (auto s : world_meta::getAllWorldNames())
		m_worldList->addListItem(s);

	loadWorldWidgetGroup->addWidget(label17);
	loadWorldWidgetGroup->addWidget(button19);
	loadWorldWidgetGroup->addWidget(button20);
	loadWorldWidgetGroup->addWidget(button21);
	loadWorldWidgetGroup->addWidget(button22);
	loadWorldWidgetGroup->addWidget(m_worldList);

	// ########################################################################

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
	if (y < 0)
		y = -y;
	else
		y = -1;

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

