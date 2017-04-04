#include "mainMenu.h"

#include <iostream>
#include <random>
#include <string>

#include "config/data.h"
#include "graphics/graphicsManager.h"
#include "gui/terminal.h"
#include "util/soundPlayer.h"
#include "util/worldMeta.h"

using util::SoundPlayer;

using namespace std;
using namespace widget;
using namespace gui;

MainMenu::MainMenu(Game *game, SoundPlayer &soundPlayer)
  : m_game(game)
  , m_soundPlayer(soundPlayer)
  , m_settings{m_activeWidgetGroup, m_mainWidgetGroup, m_soundPlayer}
{

  glm::mat4 matrix = gui::createVirtualToScreen(
      config::graphics_data::virtualWidth, config::graphics_data::virtualHeight,
      static_cast<float>(config::graphics_data::windowWidth),
      static_cast<float>(config::graphics_data::windowHeight));

  m_virtualProjection = glm::ortho(0.0f,
      static_cast<float>(config::graphics_data::windowWidth), 0.0f,
      static_cast<float>(config::graphics_data::windowHeight), -1.0f, 1.0f)
      * matrix;

  graphics::GraphicsManager::getInstance().getSpriteBatcher().setProjection(
      m_virtualProjection);

  m_title = make_unique<gui::Image>(200, 450, 400, 100,
      config::gui_data::title);

  // ########################################################################

  {
    auto observer = [this](int id)
    {
      m_soundPlayer.playSound(config::souds::buttonPressed);
      switch(id) {
        case 0: m_activeWidgetGroup = m_playWidgetGroup; break;
        case 1: m_activeWidgetGroup = m_settings.getMainWidgetGroup(); break;
        case 2: m_game->quitGame(); break;
      }
    };

    auto label = make_shared<Label>(325, 390, 150, 30, " - Main - ");
    auto button1 = make_shared<Button>(0, 325, 350, 150, 30, observer, "Play");
    auto button2 = make_shared<Button>(1, 325, 310, 150, 30, observer,
        "Settings");
    auto button3 = make_shared<Button>(2, 325, 270, 150, 30, observer, "Quit");

    m_mainWidgetGroup = make_shared<WidgetGroup>(-1, 300, 260, 200, 130);
    m_mainWidgetGroup->addWidget({label, button1, button2, button3});
  }

  // ########################################################################

  {
    auto observer = [this](int id)
    {
      switch(id) {
        case 0: m_activeWidgetGroup = m_newWorldWidgetGroup; break;
        case 1: m_activeWidgetGroup = m_loadWorldWidgetGroup; break;
        case 2: m_activeWidgetGroup = m_mainWidgetGroup; break;
      }
    };

    auto label = make_shared<Label>(325, 390, 150, 30, " - Play - ");
    auto button1 = make_shared<Button>(0, 325, 350, 150, 30, observer,
        "New World");
    auto button2 = make_shared<Button>(1, 325, 310, 150, 30, observer,
        "Load World");
    auto button3 = make_shared<Button>(2, 325, 270, 150, 30, observer, "Back");

    m_playWidgetGroup = make_shared<WidgetGroup>(-1, 300, 260, 200, 130);

    m_playWidgetGroup->addWidget({label, button1, button2, button3});
  }

  // ########################################################################

  {
    auto observer = [this](int id)
    {
      switch(id) {
        case 0: {
          m_activeWidgetGroup = m_newWorldWidgetGroupAdvanced;
          break;
        }
        case 1: {
          string name {m_textInput3->getString()};
          if (name.size()) {
            if (!world_meta::worldNameExists(name)) {
              world_meta::addName(name);
              m_options.setName(name);
              m_game->createWorld(m_options);

              m_soundPlayer.stopMusic();

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

    auto label1 = make_shared<Label>(230, 390, 150, 50,
        " - Create New World - ");
    auto label2 = make_shared<Label>(250, 330, 150, 30, "Enter a Name:", 1);

    m_textInput3 = make_shared<TextInput>(666, 250, 290, 300, 30, 1);

    auto button1 = make_shared<Button>(0, 240, 135, 100, 30, observer,
        "Advanced", 1);
    auto button2 = make_shared<Button>(1, 350, 135, 100, 30, observer, "Create",
        1);
    auto button3 = make_shared<Button>(2, 460, 135, 100, 30, observer, "Cancel",
        1);
    auto button4 = make_shared<Button>(3, 250, 250, 300, 30, observer,
        "Random Name", 1);

    m_newWorldWidgetGroup = make_shared<WidgetGroup>(0, 200, 120, 400, 270);

    m_newWorldWidgetGroup->addWidget({label1, label2, m_textInput3, button1,
        button2, button3, button4});
  }

  // ########################################################################

  {
    auto observer = [this](int id)
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

    button1 = make_shared<ToggleButton>(0, 220, 350, 30, 30, observer, "Flat",
        2, ToggleButton::Skin::ReadioButton);
    button2 = make_shared<ToggleButton>(1, 220, 310, 30, 30, observer,
        "Height Map", 2, ToggleButton::Skin::ReadioButton);
    button2->toggle();

    button3 = make_shared<ToggleButton>(2, 420, 350, 30, 30, observer, "Same",
        2, ToggleButton::Skin::ReadioButton);
    button4 = make_shared<ToggleButton>(3, 420, 310, 30, 30, observer,
        "Different", 2, ToggleButton::Skin::ReadioButton);
    button4->toggle();

    auto button5 = make_shared<Button>(4, 350, 135, 100, 30, observer, "Reset",
        1);
    auto button6 = make_shared<Button>(5, 460, 135, 100, 30, observer, "Back",
        1);

    m_newWorldWidgetGroupAdvanced = make_shared<WidgetGroup>(0, 200, 120, 400,
        270);

    m_newWorldWidgetGroupAdvanced->addWidget({label1, button1, button2, button3,
        button4, button5, button6});
  }

  // ########################################################################

  {
    auto observer = [this](int id)
    {
      switch(id) {
        case 0: {
          break;
        }
        case 1: {
          break;
        }
        case 2: {
          if (m_worldList->getSelectedListItem().size()) {
            string name = m_worldList->getSelectedListItem();
            m_options.setName(name);

            // TODO The options need to be the same as when the map was created.
            // TODO Implement so its possible to save and load options for a map.

            m_game->createWorld(m_options);
            m_soundPlayer.stopMusic();
            m_activeWidgetGroup = m_mainWidgetGroup;
            m_worldList->reset();
          }
        break;
        }
        case 3: m_activeWidgetGroup = m_playWidgetGroup; break;
      }
    };

    auto label = make_shared<Label>(270, 390, 150, 50, " - Load World - ");
    auto button1 = make_shared<Button>(0, 225, 60, 80, 30, observer, "Rename",
        1);
    auto button2 = make_shared<Button>(1, 315, 60, 80, 30, observer, "Delete",
        1);
    auto button3 = make_shared<Button>(2, 405, 60, 80, 30, observer, "Load", 1);
    auto button4 = make_shared<Button>(3, 495, 60, 80, 30, observer, "Cancel",
        1);

    m_loadWorldWidgetGroup = make_shared<WidgetGroup>(0, 200, 50, 400, 400);

    m_worldList = make_shared<SelectableList>(666, 220, 100, 360, 300, observer,
        2);

    for (auto s : world_meta::getAllWorldNames())
      m_worldList->addListItem(s);

    m_loadWorldWidgetGroup->addWidget({label, button1, button2, button3, button4,
      m_worldList});
  }

  // ########################################################################

  {
    auto observer = [this](int id)
    {
      switch(id) {
        case 1: m_activeWidgetGroup = m_newWorldWidgetGroup; break;
        case 0: m_activeWidgetGroup = m_playWidgetGroup; break;
      }
    };

    auto label = make_shared<Label>(210, 260, 150, 25,
        "Error: Name can not be empty", 4);
    auto button = make_shared<Button>(1, 510, 260, 80, 30, observer, "OK", 4);

    m_errorEmptyName = make_shared<WidgetGroup>(0, 200, 250, 400, 45, 3);

    m_errorEmptyName->addWidget({label, button});
  }

  // ########################################################################

  {
    auto observer = [this](int id)
    {
      switch(id) {
        case 1: m_activeWidgetGroup = m_newWorldWidgetGroup; break;
        case 0: m_activeWidgetGroup = m_playWidgetGroup; break;
      }
    };

    auto label = make_shared<Label>(210, 260, 150, 25,
        "Error: Name is already used", 4);
    auto button = make_shared<Button>(1, 510, 260, 80, 30, observer, "OK", 4);

    m_errorUsedName = make_shared<WidgetGroup>(0, 200, 250, 400, 45, 3);

    m_errorUsedName->addWidget({label, button});
  }

  // ########################################################################

  m_activeWidgetGroup = m_mainWidgetGroup;

}

void MainMenu::update(double timePassed) {
  shared_ptr<util::Input> input = util::Input::getInstance();

  m_mouse.unlock();
  input->updateValues();
  m_mouse.update();
  m_mouse.draw();

  m_title->draw();

  m_activeWidgetGroup->update(timePassed);
  m_activeWidgetGroup->draw();

  graphics::GraphicsManager::getInstance().clearScreen();
  graphics::GraphicsManager::getInstance().getSpriteBatcher().draw();
}

string randomName() {

  static vector<string> names{"Dank World", "Bloxel", "Sees", "Soos", "Satan",
      "Shrek", "Memus", "Adventure", "Swoosh", "Blool"};
  static int last{-1};

  static random_device randomDevice;
  static mt19937 randomNumber(randomDevice());
  static uniform_int_distribution<int> uni(0, names.size() - 1);

  int value{};
  do {
    value = uni(randomNumber);
  } while (value == last);
  last = value;

  return names[value];
}
