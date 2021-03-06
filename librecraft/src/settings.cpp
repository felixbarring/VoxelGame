
#include "settings.h"

#include "gui/widget/button.h"
#include "gui/widget/iWidget.h"
#include "gui/widget/label.h"
#include "gui/widget/slider.h"
#include "gui/widget/textInput.h"
#include "gui/widget/widgetGroup.h"
#include "util/soundPlayer.h"

using util::SoundPlayer;

using namespace widget;
using namespace std;

Settings::Settings(widget::WidgetGroup*& active,
                   widget::WidgetGroup*& parent,
                   SoundPlayer& soundPlayer,
                   graphics::GraphicsManager& graphicsManager)
  : m_soundPlayer(soundPlayer)
  , m_graphicsManager(graphicsManager)
  , m_activeWidgetGroup{active}
  , m_parentWidgetGroup{parent} {
  // ########################################################################

  {
    auto observer = [this](int id) {
      switch (id) {
        case 0:
          m_activeWidgetGroup = m_gameSettingsWidgetGroup.get();
          break;
        case 1:
          m_activeWidgetGroup = m_inputSettingsWidgetGroup.get();
          break;
        case 2:
          m_activeWidgetGroup = m_graphicsSettingsWidgetGroup.get();
          break;
        case 3:
          m_activeWidgetGroup = m_audioSettingsWidgetGroup.get();
          break;
        case 4:
          m_activeWidgetGroup = m_parentWidgetGroup;
          break;
      }
    };

    unique_ptr<Label> label = make_unique<Label>(
      325, 390, 150, 30, " - Settings - ", m_graphicsManager);
    unique_ptr<Button> button1 = make_unique<Button>(
      0, 325, 350, 150, 30, m_graphicsManager, observer, "Game");
    unique_ptr<Button> button2 = make_unique<Button>(
      1, 325, 310, 150, 30, m_graphicsManager, observer, "Input");
    unique_ptr<Button> button3 = make_unique<Button>(
      2, 325, 270, 150, 30, m_graphicsManager, observer, "Graphics");
    unique_ptr<Button> button4 = make_unique<Button>(
      3, 325, 230, 150, 30, m_graphicsManager, observer, "Audio");
    unique_ptr<Button> button5 = make_unique<Button>(
      4, 325, 190, 150, 30, m_graphicsManager, observer, "Back");

    m_settingsWidgetGroup =
      make_unique<WidgetGroup>(0, 300, 180, 200, 210, m_graphicsManager);

    m_settingsWidgetGroup->addWidget({label.get(),
                                      button1.get(),
                                      button2.get(),
                                      button3.get(),
                                      button4.get(),
                                      button5.get()});

    m_widgets.push_back(move(label));
    m_widgets.push_back(move(button1));
    m_widgets.push_back(move(button2));
    m_widgets.push_back(move(button3));
    m_widgets.push_back(move(button4));
    m_widgets.push_back(move(button5));
  }

  // ########################################################################

  {
    auto observer = [this](int id) {
      switch (id) {
        case 0:
          m_activeWidgetGroup = m_settingsWidgetGroup.get();
          break;
      }
    };

    unique_ptr<Label> label =
      make_unique<Label>(325, 390, 150, 30, " - Game - ", m_graphicsManager);
    unique_ptr<Button> button = make_unique<Button>(
      0, 325, 230, 150, 30, m_graphicsManager, observer, "Back");

    m_gameSettingsWidgetGroup =
      make_unique<WidgetGroup>(0, 300, 220, 200, 170, m_graphicsManager);

    m_gameSettingsWidgetGroup->addWidget(*label);
    m_gameSettingsWidgetGroup->addWidget(*button);

    m_widgets.push_back(move(label));
    m_widgets.push_back(move(button));
  }

  // ########################################################################

  {
    auto observer = [this](int id) {
      switch (id) {
        case 0: {
          config::input_data::mouseSensitivityX =
            (m_mouseSensitivitySlider->getValue() / 100);
          config::input_data::mouseSensitivityY =
            (m_mouseSensitivitySlider->getValue() / 100);
          m_activeWidgetGroup = m_settingsWidgetGroup.get();
          break;
        }
        case 1: {
          m_mouseSensitivityInput->setString(
            to_string(m_mouseSensitivitySlider->getValue() / 100));
          break;
        }
      }
    };

    // TODO Refactor?
    unique_ptr<IWidget> label =
      make_unique<Label>(325, 390, 150, 30, " - Input - ", m_graphicsManager);

    m_mouseSensitivitySlider = make_unique<Slider>(
      1, 125 + 100, 310, 150, 30, m_graphicsManager, observer, 1);
    m_mouseSensitivityInput = make_unique<TextInput>(
      666, 125 + 100, 270, 150, 30, m_graphicsManager, 1);

    unique_ptr<Button> button1 = make_unique<Button>(
      0, 245, 130, 150, 30, m_graphicsManager, observer, "Save", 1);
    unique_ptr<Button> button2 = make_unique<Button>(
      0, 400, 130, 150, 30, m_graphicsManager, observer, "Cancel", 1);

    m_inputSettingsWidgetGroup =
      make_unique<WidgetGroup>(0, 100, 100, 600, 250, m_graphicsManager);

    m_inputSettingsWidgetGroup->addWidget({label.get(),
                                           m_mouseSensitivityInput.get(),
                                           m_mouseSensitivitySlider.get(),
                                           button1.get(),
                                           button2.get()});

    m_widgets.push_back(move(label));
    m_widgets.push_back(move(button1));
    m_widgets.push_back(move(button2));
  }

  // ########################################################################

  {
    auto observer = [this](int id) {
      switch (id) {
        case 0:
          m_activeWidgetGroup = m_settingsWidgetGroup.get();
          break;
        case 1:
          m_activeWidgetGroup = m_settingsWidgetGroup.get();
          break;
        case 2: {
          m_fovTextInput->setString(to_string(m_fovSlider->getValue()));
          break;
        }
        case 3: {
          m_renderDistanceInput->setString(
            to_string(m_renderDistanceSlider->getValue()));
          break;
        }
      }
    };

    unique_ptr<Label> label1 = make_unique<Label>(
      325, 390, 150, 30, " - Graphics - ", m_graphicsManager);
    unique_ptr<Label> label2 =
      make_unique<Label>(30 + 100, 310, 80, 20, "FOV:", m_graphicsManager, 1);

    m_fovSlider = make_unique<Slider>(
      2, 125 + 100, 310, 150, 30, m_graphicsManager, observer, 1);
    m_fovTextInput = make_unique<TextInput>(
      666, 285 + 100, 310, 100, 30, m_graphicsManager, 1);

    unique_ptr<Label> label3 = make_unique<Label>(
      30 + 100, 270, 80, 20, "Render Distance:", m_graphicsManager, 1);

    m_renderDistanceSlider = make_unique<Slider>(
      3, 125 + 100, 270, 150, 30, m_graphicsManager, observer, 1);
    m_renderDistanceInput =
      make_unique<TextInput>(-1, 285 + 100, 270, 100, 30, m_graphicsManager, 1);

    unique_ptr<Button> button1 = make_unique<Button>(
      0, 245, 130, 150, 30, m_graphicsManager, observer, "Save", 1);
    unique_ptr<Button> button2 = make_unique<Button>(
      1, 400, 130, 150, 30, m_graphicsManager, observer, "Cancel", 1);

    m_graphicsSettingsWidgetGroup =
      make_unique<WidgetGroup>(0, 100, 100, 600, 250, m_graphicsManager);

    m_graphicsSettingsWidgetGroup->addWidget({label1.get(),
                                              label2.get(),
                                              m_fovSlider.get(),
                                              m_fovTextInput.get(),
                                              label3.get(),
                                              m_renderDistanceSlider.get(),
                                              m_renderDistanceInput.get(),
                                              button1.get(),
                                              button2.get()});

    m_widgets.push_back(move(label1));
    m_widgets.push_back(move(label2));
    m_widgets.push_back(move(label3));
    m_widgets.push_back(move(button1));
    m_widgets.push_back(move(button2));
  }

  // ########################################################################

  {

    auto observer = [this](int id) {
      switch (id) {
        case 0: {

          using namespace config::audio;
          maserVolume = m_masterVolumeSlider->getValue();
          musicVolume = m_musicVolumeSlider->getValue();
          soundVolume = m_soundVolumeSlider->getValue();

          m_soundPlayer.setMasterVolume(maserVolume);
          m_soundPlayer.setMusicVolume(musicVolume);
          m_soundPlayer.setSoundVolume(soundVolume);
          break;
        }
        case 1: {
          m_activeWidgetGroup = m_settingsWidgetGroup.get();
          break;
        }
      }
    };

    unique_ptr<Label> label1 =
      make_unique<Label>(325, 390, 150, 30, " - Audio - ", m_graphicsManager);

    unique_ptr<Label> label2 = make_unique<Label>(
      230, 310, 80, 20, "Master Volume:", m_graphicsManager, 1);
    m_masterVolumeSlider =
      make_unique<Slider>(2, 325, 310, 150, 30, m_graphicsManager, observer, 1);
    m_masterVolumeSlider->setValue(m_soundPlayer.getMasterVolume());
    m_masterVolumeInput =
      make_unique<TextInput>(666, 485, 310, 50, 30, m_graphicsManager, 1);

    unique_ptr<Label> label3 = make_unique<Label>(
      230, 270, 80, 20, "Music Volume:", m_graphicsManager, 1);
    m_musicVolumeSlider =
      make_unique<Slider>(2, 325, 270, 150, 30, m_graphicsManager, observer, 1);
    m_musicVolumeSlider->setValue(m_soundPlayer.getMusicVolume());
    m_musicVolumeInput =
      make_unique<TextInput>(666, 485, 270, 50, 30, m_graphicsManager, 1);

    unique_ptr<Label> label4 = make_unique<Label>(
      230, 230, 80, 20, "Sound Volume:", m_graphicsManager, 1);
    m_soundVolumeSlider =
      make_unique<Slider>(2, 325, 230, 150, 30, m_graphicsManager, observer, 1);
    m_soundVolumeSlider->setValue(m_soundPlayer.getSoundVolume());
    m_soundVolumeInput =
      make_unique<TextInput>(666, 485, 230, 50, 30, m_graphicsManager, 1);

    unique_ptr<Button> button1 = make_unique<Button>(
      0, 245, 130, 150, 30, m_graphicsManager, observer, "Save", 1);
    unique_ptr<Button> button2 = make_unique<Button>(
      1, 400, 130, 150, 30, m_graphicsManager, observer, "Cancel", 1);

    m_audioSettingsWidgetGroup =
      make_unique<WidgetGroup>(0, 200, 120, 400, 230, m_graphicsManager);
    m_audioSettingsWidgetGroup->addWidget({label1.get(),
                                           button1.get(),
                                           button2.get(),
                                           m_masterVolumeSlider.get(),
                                           m_masterVolumeInput.get(),
                                           m_musicVolumeSlider.get(),
                                           m_musicVolumeInput.get(),
                                           m_soundVolumeSlider.get(),
                                           m_soundVolumeInput.get(),
                                           label2.get(),
                                           label3.get(),
                                           label4.get()});
    m_widgets.push_back(move(label1));
    m_widgets.push_back(move(button1));
    m_widgets.push_back(move(button2));
    m_widgets.push_back(move(label2));
    m_widgets.push_back(move(label3));
    m_widgets.push_back(move(label4));
  }
}

WidgetGroup*
Settings::getMainWidgetGroup() const {
  return m_settingsWidgetGroup.get();
}
