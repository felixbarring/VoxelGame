#include "mainMenu.h"

#include <iostream>
#include <limits>
#include <random>
#include <string>

#include "config/data.h"
#include "graphics/graphicsManager.h"
#include "gui/terminal.h"
#include "model/world/chunk/creationOptions.h"
#include "util/soundPlayer.h"
#include "util/worldMeta.h"

using chunk::CreationOptions;
using util::SoundPlayer;

using namespace std;
using namespace widget;
using namespace gui;

namespace {

int
randomInt() {
  static random_device randomDevice;
  static mt19937 randomNumber(randomDevice());
  static uniform_int_distribution<int> uni(0, std::numeric_limits<int>::max());

  return uni(randomNumber);
}

string
randomName() {

  static vector<string> names{"Dank World",
                              "Bloxel",
                              "Sees",
                              "Soos",
                              "Satan",
                              "Shrek",
                              "Memus",
                              "Adventure",
                              "Swoosh",
                              "Blool",
                              "Middle Earth",
                              "FunCraft",
                              "C++ is the best",
                              "Spooky",
                              "KeK",
                              "I accidently Minecraft",
                              "Bicycle",
                              "Dirtbike",
                              "My name is Jeff",
                              "Don't cutt",
                              "Smort"};
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
}

MainMenu::MainMenu(Game& game,
                   SoundPlayer& soundPlayer,
                   graphics::GraphicsManager& graphicsManager)
  : m_game{game}
  , m_soundPlayer{soundPlayer}
  , m_graphicsManager{graphicsManager}
  , m_mouse{m_graphicsManager}
  , m_settings{m_activeWidgetGroup,
               m_mainWidgetGroupRaw,
               m_soundPlayer,
               m_graphicsManager} {

  glm::mat4 matrix = gui::createVirtualToScreen(
    config::graphics_data::virtualWidth,
    config::graphics_data::virtualHeight,
    static_cast<float>(config::graphics_data::windowWidth),
    static_cast<float>(config::graphics_data::windowHeight));

  m_virtualProjection =
    glm::ortho(0.0f,
               static_cast<float>(config::graphics_data::windowWidth),
               0.0f,
               static_cast<float>(config::graphics_data::windowHeight),
               -1.0f,
               1.0f) *
    matrix;

  m_graphicsManager.getSpriteBatcher().setProjection(m_virtualProjection);

  m_title = make_unique<gui::Image>(
    200, 450, 400, 100, config::gui_data::title, m_graphicsManager);

  // ########################################################################

  {
    auto observer = [this](int id) {
      m_soundPlayer.playSound(config::audio::buttonPressed);
      switch (id) {
        case 0:
          m_activeWidgetGroup = m_playWidgetGroup.get();
          break;
        case 1:
          m_activeWidgetGroup = m_settings.getMainWidgetGroup();
          break;
        case 2:
          m_game.quitGame();
          break;
      }
    };

    auto label =
      make_unique<Label>(325, 390, 150, 30, " - Main - ", m_graphicsManager);
    auto button1 = make_unique<Button>(
      0, 325, 350, 150, 30, m_graphicsManager, observer, "Play");
    auto button2 = make_unique<Button>(
      1, 325, 310, 150, 30, m_graphicsManager, observer, "Settings");
    auto button3 = make_shared<Button>(
      2, 325, 270, 150, 30, m_graphicsManager, observer, "Quit");

    m_mainWidgetGroup =
      make_unique<WidgetGroup>(-1, 300, 260, 200, 130, m_graphicsManager);
    m_mainWidgetGroup->addWidget(
      {label.get(), button1.get(), button2.get(), button3.get()});

    m_widgets.push_back(move(label));
    m_widgets.push_back(move(button1));
    m_widgets.push_back(move(button2));
    m_widgets.push_back(move(button3));
  }

  // ########################################################################

  {
    auto observer = [this](int id) {
      switch (id) {
        case 0:
          m_activeWidgetGroup = m_newWorldWidgetGroup.get();
          break;
        case 1:
          m_activeWidgetGroup = m_loadWorldWidgetGroup.get();
          break;
        case 2:
          m_activeWidgetGroup = m_mainWidgetGroup.get();
          break;
      }
    };

    auto label =
      make_unique<Label>(325, 390, 150, 30, " - Play - ", m_graphicsManager);
    auto button1 = make_unique<Button>(
      0, 325, 350, 150, 30, m_graphicsManager, observer, "New World");
    auto button2 = make_unique<Button>(
      1, 325, 310, 150, 30, m_graphicsManager, observer, "Load World");
    auto button3 = make_unique<Button>(
      2, 325, 270, 150, 30, m_graphicsManager, observer, "Back");

    m_playWidgetGroup =
      make_unique<WidgetGroup>(-1, 300, 260, 200, 130, m_graphicsManager);

    m_playWidgetGroup->addWidget(
      {label.get(), button1.get(), button2.get(), button3.get()});

    m_widgets.push_back(move(label));
    m_widgets.push_back(move(button1));
    m_widgets.push_back(move(button2));
    m_widgets.push_back(move(button3));
  }

  // ########################################################################

  {
    auto observer = [this](int id) {
      switch (id) {
        case 0: {
          m_activeWidgetGroup = m_newWorldWidgetGroupAdvanced.get();
          break;
        }
        case 1: {
          string name{m_textInput3->getString()};
          if (name.size()) {
            if (!world_meta::worldNameExists(name)) {
              world_meta::addName(name);

              m_game.createWorld(
                CreationOptions{name,
                                stoi(m_textInput4->getString()),
                                button1->isToggled(),
                                button3->isToggled()});

              m_soundPlayer.stopMusic();

              m_activeWidgetGroup = m_mainWidgetGroup.get();
              m_worldList->addListItem(name);
            } else {
              m_activeWidgetGroup = m_errorUsedName.get();
            }
          } else {
            m_activeWidgetGroup = m_errorEmptyName.get();
          }
          break;
        }
        case 2:
          m_activeWidgetGroup = m_playWidgetGroup.get();
          break;
        case 3: {
          m_textInput3->setString(randomName());
          break;
        }
      }
    };

    auto label1 = make_unique<Label>(
      230, 390, 150, 50, " - Create New World - ", m_graphicsManager);
    auto label2 = make_unique<Label>(
      250, 330, 150, 30, "Enter a Name:", m_graphicsManager, 1);

    m_textInput3 =
      make_unique<TextInput>(666, 250, 290, 300, 30, m_graphicsManager, 1);

    auto button1 = make_unique<Button>(
      0, 240, 135, 100, 30, m_graphicsManager, observer, "Advanced", 1);
    auto button2 = make_unique<Button>(
      1, 350, 135, 100, 30, m_graphicsManager, observer, "Create", 1);
    auto button3 = make_unique<Button>(
      2, 460, 135, 100, 30, m_graphicsManager, observer, "Cancel", 1);
    auto button4 = make_unique<Button>(
      3, 250, 250, 300, 30, m_graphicsManager, observer, "Random Name", 1);

    m_newWorldWidgetGroup =
      make_unique<WidgetGroup>(0, 200, 120, 400, 270, m_graphicsManager);

    m_newWorldWidgetGroup->addWidget({label1.get(),
                                      label2.get(),
                                      m_textInput3.get(),
                                      button1.get(),
                                      button2.get(),
                                      button3.get(),
                                      button4.get()});

    m_widgets.push_back(move(label1));
    m_widgets.push_back(move(label2));
    m_widgets.push_back(move(button1));
    m_widgets.push_back(move(button2));
    m_widgets.push_back(move(button3));
    m_widgets.push_back(move(button4));
  }

  // ########################################################################

  {
    auto observer = [this](int id) {
      switch (id) {
        case 0: {
          button1->setToggled();
          button2->setUntoggled();
          break;
        }
        case 1: {
          button1->setUntoggled();
          button2->setToggled();
          break;
        }
        case 2: {
          button3->setToggled();
          button4->setUntoggled();
          break;
        }
        case 3: {
          button3->setUntoggled();
          button4->setToggled();
          break;
        }
        case 4:
          break; // TODO
        case 5:
          m_activeWidgetGroup = m_newWorldWidgetGroup.get();
          break;
        case 6: {

          m_textInput4->setString(to_string(randomInt()));
          break;
        }
      }
    };

    auto label1 = make_unique<Label>(
      230, 390, 150, 50, " - Advanced - ", m_graphicsManager);

    button1 = make_unique<ToggleButton>(0,
                                        220,
                                        350,
                                        30,
                                        30,
                                        m_graphicsManager,
                                        observer,
                                        "Flat",
                                        2,
                                        ToggleButton::Skin::RadioButton);
    button2 = make_unique<ToggleButton>(1,
                                        220,
                                        310,
                                        30,
                                        30,
                                        m_graphicsManager,
                                        observer,
                                        "Height Map",
                                        2,
                                        ToggleButton::Skin::RadioButton);
    button2->toggle();

    button3 = make_unique<ToggleButton>(2,
                                        420,
                                        350,
                                        30,
                                        30,
                                        m_graphicsManager,
                                        observer,
                                        "Same",
                                        2,
                                        ToggleButton::Skin::RadioButton);
    button4 = make_unique<ToggleButton>(3,
                                        420,
                                        310,
                                        30,
                                        30,
                                        m_graphicsManager,
                                        observer,
                                        "Different",
                                        2,
                                        ToggleButton::Skin::RadioButton);
    button4->toggle();

    auto button5 = make_unique<Button>(
      4, 350, 135, 100, 30, m_graphicsManager, observer, "Reset", 1);
    auto button6 = make_unique<Button>(
      5, 460, 135, 100, 30, m_graphicsManager, observer, "Back", 1);

    auto seedLabel =
      make_unique<Label>(230, 210, 50, 30, "Seed", m_graphicsManager);

    m_textInput4 =
      make_unique<TextInput>(666, 280, 210, 150, 30, m_graphicsManager, 1);

    m_textInput4->setString(to_string(randomInt()));

    auto seedButton = make_unique<Button>(
      6, 440, 210, 100, 30, m_graphicsManager, observer, "Randomize", 1);

    m_newWorldWidgetGroupAdvanced =
      make_unique<WidgetGroup>(0, 200, 120, 400, 270, m_graphicsManager);

    m_newWorldWidgetGroupAdvanced->addWidget({label1.get(),
                                              button1.get(),
                                              button2.get(),
                                              button3.get(),
                                              button4.get(),
                                              button5.get(),
                                              button6.get(),
                                              seedLabel.get(),
                                              m_textInput4.get(),
                                              seedButton.get()});

    m_widgets.push_back(move(label1));
    m_widgets.push_back(move(button5));
    m_widgets.push_back(move(button6));
    m_widgets.push_back(move(seedLabel));
    m_widgets.push_back(move(seedButton));
  }

  // ########################################################################

  {
    auto observer = [this](int id) {
      switch (id) {
        case 0: {
          break;
        }
        case 1: {
          break;
        }
        case 2: {
          if (m_worldList->getSelectedListItem().size()) {
            string name{m_worldList->getSelectedListItem()};
            m_game.createWorld(CreationOptions{name});
            m_soundPlayer.stopMusic();
            m_activeWidgetGroup = m_mainWidgetGroup.get();
            m_worldList->reset();
          }
          break;
        }
        case 3: {
          m_activeWidgetGroup = m_playWidgetGroup.get();
          break;
        }
      }
    };

    auto label = make_unique<Label>(
      270, 390, 150, 50, " - Load World - ", m_graphicsManager);
    auto button1 = make_unique<Button>(
      0, 225, 60, 80, 30, m_graphicsManager, observer, "Rename", 1);
    auto button2 = make_unique<Button>(
      1, 315, 60, 80, 30, m_graphicsManager, observer, "Delete", 1);
    auto button3 = make_unique<Button>(
      2, 405, 60, 80, 30, m_graphicsManager, observer, "Load", 1);
    auto button4 = make_unique<Button>(
      3, 495, 60, 80, 30, m_graphicsManager, observer, "Cancel", 1);

    m_loadWorldWidgetGroup =
      make_unique<WidgetGroup>(0, 200, 50, 400, 400, m_graphicsManager);

    m_worldList = make_unique<SelectableList>(
      666, 220, 100, 360, 300, m_graphicsManager, observer, 2);

    for (auto s : world_meta::getAllWorldNames())
      m_worldList->addListItem(s);

    m_loadWorldWidgetGroup->addWidget({label.get(),
                                       button1.get(),
                                       button2.get(),
                                       button3.get(),
                                       button4.get(),
                                       m_worldList.get()});

    m_widgets.push_back(move(label));
    m_widgets.push_back(move(button1));
    m_widgets.push_back(move(button2));
    m_widgets.push_back(move(button3));
    m_widgets.push_back(move(button4));
  }

  // ########################################################################

  {
    auto observer = [this](int id) {
      switch (id) {
        case 1:
          m_activeWidgetGroup = m_newWorldWidgetGroup.get();
          break;
        case 0:
          m_activeWidgetGroup = m_playWidgetGroup.get();
          break;
      }
    };

    auto label = make_unique<Label>(
      210, 260, 150, 25, "Error: Name can not be empty", m_graphicsManager, 4);
    auto button = make_unique<Button>(
      1, 510, 260, 80, 30, m_graphicsManager, observer, "OK", 4);

    m_errorEmptyName =
      make_unique<WidgetGroup>(0, 200, 250, 400, 45, m_graphicsManager, 3);

    m_errorEmptyName->addWidget({label.get(), button.get()});

    m_widgets.push_back(move(label));
    m_widgets.push_back(move(button));
  }

  // ########################################################################

  {
    auto observer = [this](int id) {
      switch (id) {
        case 1:
          m_activeWidgetGroup = m_newWorldWidgetGroup.get();
          break;
        case 0:
          m_activeWidgetGroup = m_playWidgetGroup.get();
          break;
      }
    };

    auto label = make_unique<Label>(
      210, 260, 150, 25, "Error: Name is already used", m_graphicsManager, 4);
    auto button = make_unique<Button>(
      1, 510, 260, 80, 30, m_graphicsManager, observer, "OK", 4);

    m_errorUsedName =
      make_unique<WidgetGroup>(0, 200, 250, 400, 45, m_graphicsManager, 3);

    m_errorUsedName->addWidget({label.get(), button.get()});

    m_widgets.push_back(move(label));
    m_widgets.push_back(move(button));
  }

  // ########################################################################

  m_activeWidgetGroup = m_mainWidgetGroup.get();
  m_mainWidgetGroupRaw = m_mainWidgetGroup.get();
}

void
MainMenu::update(double timePassed) {
  shared_ptr<util::Input> input = util::Input::getInstance();

  m_mouse.unlock();
  m_mouse.update();
  m_mouse.draw();

  m_title->draw();

  m_activeWidgetGroup->update(timePassed);
  m_activeWidgetGroup->draw();

  m_graphicsManager.clearScreen();
  m_graphicsManager.getSpriteBatcher().draw();
}
