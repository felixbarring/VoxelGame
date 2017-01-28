#include "mainMenu.h"

#include <iostream>
#include <random>
#include <string>

#include "config/data.h"
#include "graphics/graphicsManager.h"
#include "gui/terminal.h"
#include "gui/widget/checkButton.h"
#include "util/worldMeta.h"

using namespace std;
using namespace widget;
using namespace gui;

// ########################################################
// Constructor/Destructor #################################
// ########################################################

MainMenu::MainMenu(Game *game)
    : m_game(game)
{

    glm::mat4 matrix = gui::createVirtualToScreen(
        config::graphics_data::virtualWidth,
        config::graphics_data::virtualHeight,
        static_cast<float>(config::graphics_data::windowWidth),
        static_cast<float>(config::graphics_data::windowHeight));

    m_virtualProjection = glm::ortho(0.0f,
        static_cast<float>(config::graphics_data::windowWidth), 0.0f,
        static_cast<float>(config::graphics_data::windowHeight), -1.0f,
        1.0f) * matrix;

    graphics::GraphicsManager::getInstance().getSpriteBatcher().setProjection(m_virtualProjection);

    m_title = make_unique<gui::Image>(200, 450, 400, 100, config::gui_data::title);

    bool quit = false;

    // ########################################################################

    {
        function<void(int)> observer = [this, game](int id)
        {
            switch(id) {
            case 0: m_activeWidgetGroup = m_playWidgetGroup; break;
            case 1: m_activeWidgetGroup = m_settingsWidgetGroup; break;
            case 2: game->quitGame(); break;
            }
        };

        auto label = make_shared<Label>(325, 390, 150, 30, " - Main - ");
        auto button1 = make_shared<Button>(0, 325, 350, 150, 30, observer,	"Play");
        auto button2 = make_shared<Button>(1, 325, 310, 150, 30, observer, "Settings");
        auto button3 = make_shared<Button>(2, 325, 270, 150, 30, observer, "Quit");

        m_mainWidgetGroup = make_shared<WidgetGroup>(-1, 300, 260, 200, 130, observer);

        m_mainWidgetGroup->addWidget({label, button1, button2, button3});
    }

    // ########################################################################

    {
        function<void(int)> observer = [this, game](int id)
        {
            switch(id) {
            case 0: m_activeWidgetGroup = m_newWorldWidgetGroup; break;
            case 1: m_activeWidgetGroup = m_loadWorldWidgetGroup; break;
            case 2: m_activeWidgetGroup = m_mainWidgetGroup; break;
            }
        };

        auto label = make_shared<Label>(325, 390, 150, 30, " - Play - ");
        auto button1 = make_shared<Button>(0, 325, 350, 150, 30, observer, "New World");
        auto button2 = make_shared<Button>(1, 325, 310, 150, 30, observer, "Load World");
        auto button3 = make_shared<Button>(2, 325, 270, 150, 30, observer, "Back");

        m_playWidgetGroup = make_shared<WidgetGroup>(-1, 300, 260, 200, 130, observer);

        m_playWidgetGroup->addWidget({label, button1, button2, button3});
    }

    // ########################################################################

    {
        function<void(int)> observer = [this, game](int id)
        {
            switch(id) {
            case 0: m_activeWidgetGroup = m_gameSettingsWidgetGroup; break;
            case 1: m_activeWidgetGroup = m_inputSettingsWidgetGroup; break;
            case 2: m_activeWidgetGroup = m_graphicsSettingsWidgetGroup; break;
            case 3: m_activeWidgetGroup = m_mainWidgetGroup; break;
            }
        };

        auto label = make_shared<Label>(325, 390, 150, 30, " - Settings - ");
        auto button1 = make_shared<Button>(0, 325, 350, 150, 30, observer, "Game");
        auto button2 = make_shared<Button>(1, 325, 310, 150, 30, observer, "Input");
        auto button3 = make_shared<Button>(2, 325, 270, 150, 30, observer, "Graphics");
        auto button4 = make_shared<Button>(3, 325, 230, 150, 30, observer, "Back");

        m_settingsWidgetGroup = make_shared<WidgetGroup>(0, 300, 220, 200, 170, observer);

        m_settingsWidgetGroup->addWidget({label, button1, button2, button3, button4});
    }

    // ########################################################################

    {
        function<void(int)> observer = [this, game](int id)
        {
            switch(id) {
            case 0: m_activeWidgetGroup = m_settingsWidgetGroup; break;
            }
        };

        auto label = make_shared<Label>(325, 390, 150, 30, " - Game - ");
        auto button = make_shared<Button>(0, 325, 230, 150, 30, observer, "Back");

        m_gameSettingsWidgetGroup = make_shared<WidgetGroup>(0, 300, 220, 200, 170, observer);

        m_gameSettingsWidgetGroup->addWidget(label);
        m_gameSettingsWidgetGroup->addWidget(button);
    }

    // ########################################################################

    {
        function<void(int)> observer = [this, game](int id)
        {
            switch(id) {
            case 0: {
                config::input_data::mouseSensitivityX = (m_mouseSensitivitySlider->getValue() / 100);
                config::input_data::mouseSensitivityY = (m_mouseSensitivitySlider->getValue() / 100);
                m_activeWidgetGroup = m_settingsWidgetGroup; break;
            }
            case 1: {
                m_mouseSensitivityInput->setString(to_string(m_mouseSensitivitySlider->getValue() / 100));
                break;
            }
            }
        };

        shared_ptr<IWidget> label(new Label{325, 390, 150, 30, " - Input - "});

        m_mouseSensitivitySlider = make_shared<Slider>(1, 125 + 100, 310, 150, 30, observer, 1);
        m_mouseSensitivityInput = make_shared<TextInput>(666, 125 + 100, 270, 150, 30, 1);

        auto button1 = make_shared<Button>(0, 245, 130, 150, 30, observer, "Save", 1);
        auto button2 = make_shared<Button>(0, 400, 130, 150, 30, observer, "Cancel", 1);

        m_inputSettingsWidgetGroup = make_shared<WidgetGroup>(0, 100, 100, 600, 250, observer);

        m_inputSettingsWidgetGroup-> addWidget({label, m_mouseSensitivityInput, m_mouseSensitivitySlider, button1,
            button2});
    }

    // ########################################################################

    {
        function<void(int)> observer = [this, game](int id)
        {
            switch(id) {
            case 0: m_activeWidgetGroup = m_settingsWidgetGroup; break;
            case 1: m_activeWidgetGroup = m_settingsWidgetGroup; break;
            case 2: {
                m_textInput->setString(to_string(m_slider->getValue()));
                break;
            }
            case 3: {
                m_textInput2->setString(to_string(m_slider2->getValue()));
                break;
            }
            }
        };

        auto label1 = make_shared<Label>(325, 390, 150, 30, " - Graphics - ");
        auto label2 = make_shared<Label>(30 + 100, 310, 80, 20, "FOV:", 1);

        m_slider = make_shared<Slider>(2, 125 + 100, 310, 150, 30, observer, 1);
        m_textInput = make_shared<TextInput>(666, 285 + 100, 310, 100, 30, 1);

        auto label3 = make_shared<Label>(30 + 100, 270, 80, 20, "Render Distance:", 1);

        m_slider2 = make_shared<Slider>(3, 125 + 100, 270, 150, 30, observer, 1);
        m_textInput2 = make_shared<TextInput>(-1, 285 + 100, 270, 100, 30, 1);

        auto button1 = make_shared<Button>(0, 245, 130, 150, 30, observer, "Save", 1);
        auto button2 = make_shared<Button>(1, 400, 130, 150, 30, observer, "Cancel", 1);

        m_graphicsSettingsWidgetGroup = make_shared<WidgetGroup>(0, 100, 100, 600, 250, observer);

        m_graphicsSettingsWidgetGroup->addWidget({label1, label2, m_slider, m_textInput, label3, m_slider2,
            m_textInput2, button1, button2});
    }

    // ########################################################################

    {
        function<void(int)> observer = [this, game](int id)
        {
            switch(id) {
            case 0: {
                m_activeWidgetGroup = m_newWorldWidgetGroupAdvanced;
                break;
            }
            case 1: {
                string name{m_textInput3->getString()};
                if (name.size()) {
                    if (!world_meta::worldNameExists(name)) {
                        world_meta::addName(name);
                        m_options.setName(name);
                        game->createWorld(m_options);
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
            case 2: m_activeWidgetGroup = m_playWidgetGroup; break;
            case 3: {
                m_textInput3->setString(randomName());
                break;
            }
            }
        };

        auto label1 = make_shared<Label>(230, 390, 150, 50, " - Create New World - ");
        auto label2 = make_shared<Label>(250, 330, 150, 30, "Enter a Name:", 1);

        m_textInput3 = make_shared<TextInput>(666, 250, 290, 300, 30, 1);

        auto button1 = make_shared<Button>(0, 240, 135, 100, 30, observer, "Advanced", 1);
        auto button2 = make_shared<Button>(1, 350, 135, 100, 30, observer, "Create", 1);
        auto button3 = make_shared<Button>(2, 460, 135, 100, 30, observer, "Cancel", 1);
        auto button4 = make_shared<Button>(3, 250, 250, 300, 30, observer, "Random Name", 1);

        m_newWorldWidgetGroup = make_shared<WidgetGroup>(0, 200, 120, 400, 270, observer);

        m_newWorldWidgetGroup->addWidget({label1, label2, m_textInput3, button1, button2, button3, button4});
    }

    // ########################################################################

    {
        function<void(int)> observer = [this, game](int id)
        {
            switch(id) {
            case 0: {
                m_options.setFlat(true);
                button1->setToggled();
                button2->setUntoggled();
                break;
            }
            case 1: {
                m_options.setFlat(false);
                button1->setUntoggled();
                button2->setToggled();
                break;
            }
            case 2: {
                m_options.setDifferencBubesForEachChunk(true);
                button3->setToggled();
                button4->setUntoggled();
                break;
            }
            case 3: {
                m_options.setDifferencBubesForEachChunk(false);
                button3->setUntoggled();
                button4->setToggled();
                break;
            }
            case 4: break; // TODO
            case 5: m_activeWidgetGroup = m_newWorldWidgetGroup; break;
            }
        };

        auto label1 = make_shared<Label>(230, 390, 150, 50, " - Advanced - ");

        button1 = make_shared<ToggleButton>(0, 220, 350, 30, 30, observer, "Flat", 2,
            ToggleButton::Skin::ReadioButton);
        button2 = make_shared<ToggleButton>(1, 220, 310, 30, 30, observer, "Height Map", 2,
            ToggleButton::Skin::ReadioButton);
        button2->toggle();

        button3 = make_shared<ToggleButton>(2, 420, 350, 30, 30, observer, "Same", 2,
            ToggleButton::Skin::ReadioButton);
        button4 = make_shared<ToggleButton>(3, 420, 310, 30, 30, observer, "Different", 2,
            ToggleButton::Skin::ReadioButton);
        button4->toggle();

        auto button5 = make_shared<Button>(4, 350, 135, 100, 30, observer, "Reset", 1);
        auto button6 = make_shared<Button>(5, 460, 135, 100, 30, observer, "Back", 1);

        m_newWorldWidgetGroupAdvanced = make_shared<WidgetGroup>(0, 200, 120, 400, 270, observer);

        m_newWorldWidgetGroupAdvanced->addWidget({label1, button1, button2, button3, button4, button5, button6});
    }

    // ########################################################################

    {
        function<void(int)> observer = [this, game](int id)
        {
            switch(id) {
            case 0: break;
            case 1: break;
            case 2: {
                if (m_worldList->getSelectedListItem().size()) {
                    string name = m_worldList->getSelectedListItem();
                    m_options.setName(name);

                    // TODO The options need to be the same as when the map was created.
                    // TODO Implement so its possible to save and load options for a map.

                    game->createWorld(m_options);
                    m_activeWidgetGroup = m_mainWidgetGroup;
                    m_worldList->reset();
                }
                break;
            }
            case 3: m_activeWidgetGroup = m_playWidgetGroup; break;
            }
        };

        auto label = make_shared<Label>(270, 390, 150, 50, " - Load World - ");
        auto button1 = make_shared<Button>(0, 225, 60, 80, 30, observer, "Rename", 1);
        auto button2 = make_shared<Button>(1, 315, 60, 80, 30, observer, "Delete", 1);
        auto button3 = make_shared<Button>(2, 405, 60, 80, 30, observer, "Load", 1);
        auto button4 = make_shared<Button>(3, 495, 60, 80, 30, observer, "Cancel", 1);

        m_loadWorldWidgetGroup = make_shared<WidgetGroup>(0, 200, 50, 400, 400, observer);

        m_worldList = make_shared<SelectableList>(666, 220, 100, 360, 300, observer, 2);

        for (auto s : world_meta::getAllWorldNames())
            m_worldList->addListItem(s);

        m_loadWorldWidgetGroup->addWidget({label, button1, button2, button3, button4, m_worldList});
    }

    // ########################################################################

    {
        function<void(int)> observer = [this, game](int id)
        {
            switch(id) {
            case 1: m_activeWidgetGroup = m_newWorldWidgetGroup; break;
            case 0: m_activeWidgetGroup = m_playWidgetGroup; break;
            }
        };

        auto label = make_shared<Label>(210, 260, 150, 25, "Error: Name can not be empty", 4);
        auto button = make_shared<Button>(1, 510, 260, 80, 30, observer, "OK", 4);

        m_errorEmptyName = make_shared<WidgetGroup>(0, 200, 250, 400, 45, observer, 3);

        m_errorEmptyName->addWidget({label, button});
    }

    // ########################################################################

    {
        function<void(int)> observer = [this, game](int id)
        {
            switch(id) {
            case 1: m_activeWidgetGroup = m_newWorldWidgetGroup; break;
            case 0: m_activeWidgetGroup = m_playWidgetGroup; break;
            }
        };

        auto label = make_shared<Label>(210, 260, 150, 25, "Error: Name is already used", 4);
        auto button = make_shared<Button>(1, 510, 260, 80, 30, observer, "OK", 4);

        m_errorUsedName = make_shared<WidgetGroup>(0, 200, 250, 400, 45, observer, 3);

        m_errorUsedName->addWidget({label, button});
    }

    // ########################################################################

    m_activeWidgetGroup = m_mainWidgetGroup;
}

// ########################################################
// Member Functions########################################
// ########################################################

void MainMenu::update(float timePassed) {
    shared_ptr<util::Input> input = util::Input::getInstance();

    m_mouse.unlock();
    input->updateValues();
    m_mouse.update();
    m_mouse.draw();

    m_activeWidgetGroup->update(timePassed);

    m_title->draw();
    m_activeWidgetGroup->draw();

    glm::vec3 skyColor = config::graphics_data::skyColor;
    glClearColor(skyColor.x, skyColor.y, skyColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    graphics::GraphicsManager::getInstance().getSpriteBatcher().draw();
}

string randomName() {

    static vector<string> names {"Dank World", "Bloxel", "Sees", "Soos", "Satan", "Shrek", "Memus", "Adventure",
        "Swoosh", "Blool"};
    static int last{-1};

    random_device randomDevice;
    mt19937 randomNumber(randomDevice());
    uniform_int_distribution<int> uni(0, names.size() - 1);

    int value;
    do {
        value = uni(randomNumber);
    } while (value == last);
    last = value;

    return names[value];
}
