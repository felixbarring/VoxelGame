#include "mainMenu.h"

#include <iostream>
#include <random>
#include <string>

#include "config/data.h"
#include "gui/mouse.h"
#include "gui/terminal.h"
#include "util/worldMeta.h"

using namespace std;
using namespace widget;

// ########################################################
// Constructor/Destructor #################################
// ########################################################

MainMenu::MainMenu(Game *game)
	: m_game(game) {

	glm::mat4 matrix = gui::createVirtualToScreen(
			config::graphics_data::virtualWidth,
			config::graphics_data::virtualHeight,
			static_cast<float>(config::graphics_data::windowWidth),
			static_cast<float>(config::graphics_data::windowHeight));

	m_virtualProjection = glm::ortho(0.0f,
			static_cast<float>(config::graphics_data::windowWidth), 0.0f,
			static_cast<float>(config::graphics_data::windowHeight), -1.0f,
			1.0f) * matrix;

	graphics::SpriteBatcher::getInstance().setProjection(m_virtualProjection);

	m_title = make_shared<gui::Image>(200, 450, 400, 100, config::gui_data::title);

	// TODO Use constants for each ID.
	// TODO use better names for the widgets variables.

	bool quit = false;
	function<void(int)> observer = [&, game](int id)
	{
		switch(id) {

		case 0: m_activeWidgetGroup = m_playWidgetGroup;break;
		case 1: m_activeWidgetGroup = m_settingsWidgetGroup;break;
		case 2: game->quitGame(); break;
		case 3: m_activeWidgetGroup = m_newWorldWidgetGroup; break;
		case 4: m_activeWidgetGroup = m_loadWorldWidgetGroup; break;
		case 5: m_activeWidgetGroup = m_mainWidgetGroup; break;
		case 6: m_activeWidgetGroup = m_gameSettingsWidgetGroup; break;
		case 7: m_activeWidgetGroup = m_inputSettingsWidgetGroup; break;
		case 8: m_activeWidgetGroup = m_graphicsSettingsWidgetGroup; break;
		case 9: m_activeWidgetGroup = m_mainWidgetGroup; break;
		case 10: m_activeWidgetGroup = m_settingsWidgetGroup; break;
		case 11: {
			config::input_data::mouseSensitivityX =	(m_mouseSensitivitySlider->getValue() / 100);
			config::input_data::mouseSensitivityY = (m_mouseSensitivitySlider->getValue() / 100);
			m_activeWidgetGroup = m_settingsWidgetGroup; break;
		}
		case 12: m_activeWidgetGroup = m_settingsWidgetGroup; break;
		case 13: m_activeWidgetGroup = m_settingsWidgetGroup; break;
		case 14: break;
		case 15: {
			string name {m_textInput3->getString()};
			if (name.size()) {
				if (!world_meta::worldNameExists(name)) {
					world_meta::addName(name);
					game->createWorld(name);
					m_activeWidgetGroup = m_mainWidgetGroup;
					m_worldList->addListItem(name);
				} else {
					m_activeWidgetGroup = m_errorUsedName;
				}
			} else {
				m_activeWidgetGroup = m_errorEmptyName;
			}
			break;
		}
		case 16: m_activeWidgetGroup = m_playWidgetGroup; break;
		case 17: {
			m_textInput3->setString(randomName());
			break;
		}
		case 18: break;
		case 19: break;
		case 20: {
			if (m_worldList->getSelectedListItem().size()) {
				string name = m_worldList->getSelectedListItem();
				game->createWorld(m_worldList->getSelectedListItem());
				m_activeWidgetGroup = m_mainWidgetGroup;
				m_worldList->reset();
			}
			break;
		}
		case 21: m_activeWidgetGroup = m_playWidgetGroup; break;
		case 22: m_activeWidgetGroup = m_newWorldWidgetGroup; break;
		case 100: {
			m_textInput->setString(to_string(m_slider->getValue()));
			break;
		}
		case 101: {
			m_textInput2->setString(to_string(m_slider2->getValue()));
			break;
		}
		case 102: {
			m_mouseSensitivityInput->setString(to_string(m_mouseSensitivitySlider->getValue() / 100));
			break;
		}
		}
	};

	// TODO Give the variables better names

	// ########################################################################

	{
		shared_ptr<IWidget> label(new Label{325, 390, 150, 30, " - Main - "});
		shared_ptr<IWidget> button1(new Button{0, 325, 350, 150, 30, observer,	"Play"});
		shared_ptr<IWidget> button2(new Button{1, 325, 310, 150, 30, observer, "Settings"});
		shared_ptr<IWidget> button3(new Button{2, 325, 270, 150, 30, observer, "Quit"});

		m_mainWidgetGroup.reset(new WidgetGroup{0, 300, 260, 200, 130, observer});

		m_mainWidgetGroup->addWidget(label);
		m_mainWidgetGroup->addWidget(button1);
		m_mainWidgetGroup->addWidget(button2);
		m_mainWidgetGroup->addWidget(button3);
	}

	// ########################################################################

	{
		shared_ptr<IWidget> label(new Label{325, 390, 150, 30, " - Play - "});
		shared_ptr<IWidget> button1(new Button{3, 325, 350, 150, 30, observer, "New World"});
		shared_ptr<IWidget> button2(new Button{4, 325, 310, 150, 30, observer, "Load World"});
		shared_ptr<IWidget> button3(new Button{5, 325, 270, 150, 30, observer, "Back"});

		m_playWidgetGroup.reset(new WidgetGroup {0, 300, 260, 200, 130, observer});

		m_playWidgetGroup->addWidget(label);
		m_playWidgetGroup->addWidget(button1);
		m_playWidgetGroup->addWidget(button2);
		m_playWidgetGroup->addWidget(button3);
	}

	// ########################################################################

	{
		shared_ptr<IWidget> label(new Label{325, 390, 150, 30, " - Settings - "});
		shared_ptr<IWidget> button1(new Button{6, 325, 350, 150, 30, observer, "Game"});
		shared_ptr<IWidget> button2(new Button{7, 325, 310, 150, 30, observer, "Input"});
		shared_ptr<IWidget> button3(new Button{8, 325, 270, 150, 30, observer, "Graphics"});
		shared_ptr<IWidget> button4(new Button{9, 325, 230, 150, 30, observer, "Back"});

		m_settingsWidgetGroup.reset(new WidgetGroup{0, 300, 220, 200, 170, observer});

		m_settingsWidgetGroup->addWidget(label);
		m_settingsWidgetGroup->addWidget(button1);
		m_settingsWidgetGroup->addWidget(button2);
		m_settingsWidgetGroup->addWidget(button3);
		m_settingsWidgetGroup->addWidget(button4);
	}

	// ########################################################################

	{
		shared_ptr<IWidget> label(new Label{325, 390, 150, 30, " - Game - "});
		shared_ptr<IWidget> button(new Button{10, 325, 230, 150, 30, observer, "Back"});

		m_gameSettingsWidgetGroup.reset(new WidgetGroup{0, 300, 220, 200, 170, observer});

		m_gameSettingsWidgetGroup->addWidget(label);
		m_gameSettingsWidgetGroup->addWidget(button);
	}

	// ########################################################################

	{
		shared_ptr<IWidget> label(new Label{325, 390, 150, 30, " - Input - "});

		m_mouseSensitivitySlider.reset(new Slider{102, 125 + 100, 310, 150, 30, observer, 1});
		m_mouseSensitivityInput.reset(new TextInput{666, 125 + 100, 270, 150, 30, 1});

		m_inputSettingsWidgetGroup.reset(new WidgetGroup{0, 100, 100, 600, 250, observer});

		shared_ptr<IWidget> button1(new Button{11, 245, 130, 150, 30, observer, "Save", 1});
		shared_ptr<IWidget> button2(new Button{11, 400, 130, 150, 30, observer, "Cancel", 1});

		m_inputSettingsWidgetGroup->addWidget(label);
		m_inputSettingsWidgetGroup->addWidget(m_mouseSensitivityInput);
		m_inputSettingsWidgetGroup->addWidget(m_mouseSensitivitySlider);
		m_inputSettingsWidgetGroup->addWidget(button1);
		m_inputSettingsWidgetGroup->addWidget(button2);
	}

	// ########################################################################

	{
		shared_ptr<IWidget> label1(new Label{325, 390, 150, 30, " - Graphics - "});

		shared_ptr<IWidget> label2(new Label{30 + 100, 310, 80, 20, "FOV:", 1});
		m_slider.reset(new Slider{100, 125 + 100, 310, 150, 30, observer, 1});
		m_textInput.reset(new TextInput{666, 285 + 100, 310, 100, 30, 1});

		shared_ptr<IWidget> label3(new Label{30 + 100, 270, 80, 20, "Render Distance:", 1});
		m_slider2.reset(new Slider{101, 125 + 100, 270, 150, 30, observer, 1});
		m_textInput2.reset(new TextInput{666, 285 + 100, 270, 100, 30, 1});

		shared_ptr<IWidget> button1(new Button{12, 245, 130, 150, 30, observer, "Save", 1});
		shared_ptr<IWidget> button2(new Button{13, 400, 130, 150, 30, observer, "Cancel", 1});

		m_graphicsSettingsWidgetGroup.reset(new WidgetGroup{0, 100, 100, 600, 250, observer});

		m_graphicsSettingsWidgetGroup->addWidget(label1);

		m_graphicsSettingsWidgetGroup->addWidget(label2);
		m_graphicsSettingsWidgetGroup->addWidget(m_slider);
		m_graphicsSettingsWidgetGroup->addWidget(m_textInput);

		m_graphicsSettingsWidgetGroup->addWidget(label3);
		m_graphicsSettingsWidgetGroup->addWidget(m_slider2);
		m_graphicsSettingsWidgetGroup->addWidget(m_textInput2);

		m_graphicsSettingsWidgetGroup->addWidget(button1);
		m_graphicsSettingsWidgetGroup->addWidget(button2);
	}

	// ########################################################################

	{
		shared_ptr<IWidget> label1(new Label{230, 390, 150, 50, " - Create New World - "});
		shared_ptr<IWidget> label2(new Label{250, 330, 150, 30, "Enter a Name:", 1});
		m_textInput3.reset(new TextInput{666, 250, 290, 300, 30, 1});
		shared_ptr<IWidget> button1(new Button{14, 240, 135, 100, 30, observer, "Advanced", 1});
		shared_ptr<IWidget> button2(new Button{15, 350, 135, 100, 30, observer, "Create", 1});
		shared_ptr<IWidget> button3(new Button{16, 460, 135, 100, 30, observer, "Cancel", 1});
		shared_ptr<IWidget> button4(new Button{17, 250, 250, 300, 30, observer, "Random Name", 1});

		m_newWorldWidgetGroup.reset(new WidgetGroup{0, 200, 120, 400, 270, observer});

		m_newWorldWidgetGroup->addWidget(label1);
		m_newWorldWidgetGroup->addWidget(label2);
		m_newWorldWidgetGroup->addWidget(m_textInput3);
		m_newWorldWidgetGroup->addWidget(button1);
		m_newWorldWidgetGroup->addWidget(button2);
		m_newWorldWidgetGroup->addWidget(button3);
		m_newWorldWidgetGroup->addWidget(button4);
	}

	// ########################################################################

	{
		shared_ptr<IWidget> label(new Label{270, 390, 150, 50, " - Load World - "});
		shared_ptr<IWidget> button1(new Button{18, 225, 60, 80, 30, observer, "Rename", 1});
		shared_ptr<IWidget> button2(new Button{19, 315, 60, 80, 30, observer, "Delete", 1});
		shared_ptr<IWidget> button3(new Button{20, 405, 60, 80, 30, observer, "Load", 1});
		shared_ptr<IWidget> button4(new Button{21, 495, 60, 80, 30, observer, "Cancel", 1});

		m_loadWorldWidgetGroup.reset(new WidgetGroup{0, 200, 50, 400, 400, observer});

		m_worldList.reset(new SelectableList{666, 220, 100, 360, 300, observer, 2});

		for (auto s : world_meta::getAllWorldNames())
			m_worldList->addListItem(s);

		m_loadWorldWidgetGroup->addWidget(label);
		m_loadWorldWidgetGroup->addWidget(button1);
		m_loadWorldWidgetGroup->addWidget(button2);
		m_loadWorldWidgetGroup->addWidget(button3);
		m_loadWorldWidgetGroup->addWidget(button4);
		m_loadWorldWidgetGroup->addWidget(m_worldList);
	}

	// ########################################################################

	{
		shared_ptr<IWidget> label(new Label{210, 260, 150, 25, "Error: Name can not be empty", 4});
		shared_ptr<IWidget> button(new Button{22, 510, 260, 80, 30, observer, "OK", 4});
		m_errorEmptyName.reset(new WidgetGroup{0, 200, 250, 400, 45, observer, 3});
		m_errorEmptyName->addWidget(label);
		m_errorEmptyName->addWidget(button);
	}

	// ########################################################################

	{
		shared_ptr<IWidget> label(new Label{210, 260, 150, 25, "Error: Name is already used", 4});
		shared_ptr<IWidget> button(new Button{22, 510, 260, 80, 30, observer, "OK", 4});
		m_errorUsedName.reset(new WidgetGroup{0, 200, 250, 400, 45, observer, 3});
		m_errorUsedName->addWidget(label);
		m_errorUsedName->addWidget(button);
	}

	// ########################################################################

	m_activeWidgetGroup = m_mainWidgetGroup;

}

// ########################################################
// Member Functions########################################
// ########################################################

void MainMenu::update(float timePassed) {
	shared_ptr<util::Input> input = util::Input::getInstance();

	Mouse::getInstance().unlock();
	input->updateValues();
	Mouse::getInstance().update();
	Mouse::getInstance().draw();

	m_activeWidgetGroup->update(timePassed);

	m_title->draw();
	m_activeWidgetGroup->draw();

	graphics::SpriteBatcher::getInstance().draw();
}

std::string MainMenu::randomName() {

	static vector<string> names {"Dank World", "Bloxel", "Sees", "Soos", "Satan", "Shrek", "Memus", "Adventure",
		"Swoosh", "Blool"};
	static int last{-1};

	std::random_device randomDevice;
	std::mt19937 randomNumber(randomDevice());
	std::uniform_int_distribution<int> uni(0, names.size() - 1);

	int value;
	do {
		value = uni(randomNumber);
	} while (value == last);
	last = value;

	return names[value];
}
