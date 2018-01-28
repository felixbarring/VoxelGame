
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

Settings::Settings(std::shared_ptr<widget::WidgetGroup>& active,
                   std::shared_ptr<widget::WidgetGroup>& parent,
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
          m_activeWidgetGroup = m_gameSettingsWidgetGroup;
          break;
        case 1:
          m_activeWidgetGroup = m_inputSettingsWidgetGroup;
          break;
        case 2:
          m_activeWidgetGroup = m_graphicsSettingsWidgetGroup;
          break;
        case 3:
          m_activeWidgetGroup = m_audioSettingsWidgetGroup;
          break;
        case 4:
          m_activeWidgetGroup = m_parentWidgetGroup;
          break;
      }
    };

    shared_ptr<Label> label = make_shared<Label>(
      325, 390, 150, 30, " - Settings - ", m_graphicsManager);
    shared_ptr<Button> button1 = make_shared<Button>(
      0, 325, 350, 150, 30, m_graphicsManager, observer, "Game");
    shared_ptr<Button> button2 = make_shared<Button>(
      1, 325, 310, 150, 30, m_graphicsManager, observer, "Input");
    shared_ptr<Button> button3 = make_shared<Button>(
      2, 325, 270, 150, 30, m_graphicsManager, observer, "Graphics");
    shared_ptr<Button> button4 = make_shared<Button>(
      3, 325, 230, 150, 30, m_graphicsManager, observer, "Audio");
    shared_ptr<Button> button5 = make_shared<Button>(
      4, 325, 190, 150, 30, m_graphicsManager, observer, "Back");

    m_settingsWidgetGroup =
      make_shared<WidgetGroup>(0, 300, 180, 200, 210, m_graphicsManager);

    m_settingsWidgetGroup->addWidget(
      {label, button1, button2, button3, button4, button5});
  }

  // ########################################################################

  {
    auto observer = [this](int id) {
      switch (id) {
        case 0:
          m_activeWidgetGroup = m_settingsWidgetGroup;
          break;
      }
    };

    shared_ptr<Label> label =
      make_shared<Label>(325, 390, 150, 30, " - Game - ", m_graphicsManager);
    shared_ptr<Button> button = make_shared<Button>(
      0, 325, 230, 150, 30, m_graphicsManager, observer, "Back");

    m_gameSettingsWidgetGroup =
      make_shared<WidgetGroup>(0, 300, 220, 200, 170, m_graphicsManager);

    m_gameSettingsWidgetGroup->addWidget(label);
    m_gameSettingsWidgetGroup->addWidget(button);
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
          m_activeWidgetGroup = m_settingsWidgetGroup;
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
    shared_ptr<IWidget> label(
      new Label{325, 390, 150, 30, " - Input - ", m_graphicsManager});

    m_mouseSensitivitySlider = make_shared<Slider>(
      1, 125 + 100, 310, 150, 30, m_graphicsManager, observer, 1);
    m_mouseSensitivityInput = make_shared<TextInput>(
      666, 125 + 100, 270, 150, 30, m_graphicsManager, 1);

    shared_ptr<Button> button1 = make_shared<Button>(
      0, 245, 130, 150, 30, m_graphicsManager, observer, "Save", 1);
    shared_ptr<Button> button2 = make_shared<Button>(
      0, 400, 130, 150, 30, m_graphicsManager, observer, "Cancel", 1);

    m_inputSettingsWidgetGroup =
      make_shared<WidgetGroup>(0, 100, 100, 600, 250, m_graphicsManager);

    m_inputSettingsWidgetGroup->addWidget({label,
                                           m_mouseSensitivityInput,
                                           m_mouseSensitivitySlider,
                                           button1,
                                           button2});
  }

  // ########################################################################

  {
    auto observer = [this](int id) {
      switch (id) {
        case 0:
          m_activeWidgetGroup = m_settingsWidgetGroup;
          break;
        case 1:
          m_activeWidgetGroup = m_settingsWidgetGroup;
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

    shared_ptr<Label> label1 = make_shared<Label>(
      325, 390, 150, 30, " - Graphics - ", m_graphicsManager);
    shared_ptr<Label> label2 =
      make_shared<Label>(30 + 100, 310, 80, 20, "FOV:", m_graphicsManager, 1);

    m_fovSlider = make_shared<Slider>(
      2, 125 + 100, 310, 150, 30, m_graphicsManager, observer, 1);
    m_fovTextInput = make_shared<TextInput>(
      666, 285 + 100, 310, 100, 30, m_graphicsManager, 1);

    shared_ptr<Label> label3 = make_shared<Label>(
      30 + 100, 270, 80, 20, "Render Distance:", m_graphicsManager, 1);

    m_renderDistanceSlider = make_shared<Slider>(
      3, 125 + 100, 270, 150, 30, m_graphicsManager, observer, 1);
    m_renderDistanceInput =
      make_shared<TextInput>(-1, 285 + 100, 270, 100, 30, m_graphicsManager, 1);

    shared_ptr<Button> button1 = make_shared<Button>(
      0, 245, 130, 150, 30, m_graphicsManager, observer, "Save", 1);
    shared_ptr<Button> button2 = make_shared<Button>(
      1, 400, 130, 150, 30, m_graphicsManager, observer, "Cancel", 1);

    m_graphicsSettingsWidgetGroup =
      make_shared<WidgetGroup>(0, 100, 100, 600, 250, m_graphicsManager);

    m_graphicsSettingsWidgetGroup->addWidget({label1,
                                              label2,
                                              m_fovSlider,
                                              m_fovTextInput,
                                              label3,
                                              m_renderDistanceSlider,
                                              m_renderDistanceInput,
                                              button1,
                                              button2});
  }

  // ########################################################################

  {

    auto observer = [this](int id) {
      switch (id) {
        case 0: {
          m_soundPlayer.setMasterVolume(m_masterVolumeSlider->getValue());

          m_soundPlayer.setMusicVolume(m_musicVolumeSlider->getValue());

          m_soundPlayer.setSoundVolume(m_soundVolumeSlider->getValue());
          break;
        }
        case 1: {
          m_activeWidgetGroup = m_settingsWidgetGroup;
          break;
        }
      }
    };

    shared_ptr<Label> label1 =
      make_shared<Label>(325, 390, 150, 30, " - Audio - ", m_graphicsManager);

    shared_ptr<Label> label2 = make_shared<Label>(
      230, 310, 80, 20, "Master Volume:", m_graphicsManager, 1);
    m_masterVolumeSlider =
      make_shared<Slider>(2, 325, 310, 150, 30, m_graphicsManager, observer, 1);
    m_masterVolumeSlider->setValue(m_soundPlayer.getMasterVolume());
    m_masterVolumeInput =
      make_shared<TextInput>(666, 485, 310, 50, 30, m_graphicsManager, 1);

    shared_ptr<Label> label3 = make_shared<Label>(
      230, 270, 80, 20, "Music Volume:", m_graphicsManager, 1);
    m_musicVolumeSlider =
      make_shared<Slider>(2, 325, 270, 150, 30, m_graphicsManager, observer, 1);
    m_musicVolumeSlider->setValue(m_soundPlayer.getMusicVolume());
    m_musicVolumeInput =
      make_shared<TextInput>(666, 485, 270, 50, 30, m_graphicsManager, 1);

    shared_ptr<Label> label4 = make_shared<Label>(
      230, 230, 80, 20, "Sound Volume:", m_graphicsManager, 1);
    m_soundVolumeSlider =
      make_shared<Slider>(2, 325, 230, 150, 30, m_graphicsManager, observer, 1);
    m_soundVolumeSlider->setValue(m_soundPlayer.getSoundVolume());
    m_soundVolumeInput =
      make_shared<TextInput>(666, 485, 230, 50, 30, m_graphicsManager, 1);

    shared_ptr<Button> button1 = make_shared<Button>(
      0, 245, 130, 150, 30, m_graphicsManager, observer, "Save", 1);
    shared_ptr<Button> button2 = make_shared<Button>(
      1, 400, 130, 150, 30, m_graphicsManager, observer, "Cancel", 1);

    m_audioSettingsWidgetGroup =
      make_shared<WidgetGroup>(0, 200, 120, 400, 230, m_graphicsManager);
    m_audioSettingsWidgetGroup->addWidget({label1,
                                           button1,
                                           button2,
                                           m_masterVolumeSlider,
                                           m_masterVolumeInput,
                                           m_musicVolumeSlider,
                                           m_musicVolumeInput,
                                           m_soundVolumeSlider,
                                           m_soundVolumeInput,
                                           label2,
                                           label3,
                                           label4});
  }
}

shared_ptr<WidgetGroup>
Settings::getMainWidgetGroup() const {
  return m_settingsWidgetGroup;
}
