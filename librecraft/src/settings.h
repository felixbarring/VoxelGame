
#ifndef SRC_SETTINGS_H_
#define SRC_SETTINGS_H_

#include "gui/widget/slider.h"
#include "gui/widget/textInput.h"
#include "gui/widget/widgetGroup.h"
#include "util/soundPlayer.h"

/**
 * @brief Class that controls the settings.
 *
 * The purpose is to provide a widget group that controls the game settings.
 * The user is responsible to call the update and draw functions for the widget
 * group. This class only handles the logic managing the settings. This class
 * can be reused and exists as more than one instance, for instance in the main
 * menu and in the ingame menu.
 */
class Settings {
public:

  /**
   * @brief Creates an instance of this class.
   *
   * @param active The active widget group that the user is responsible to call
   *               update and draw on.
   * @param parent The parent widget group. Will be set as the active widget
   *               group when the user is done with the settings
   * @param soundPlayer Needed to play sounds when GUI events occurs.
   */
  Settings(std::shared_ptr<widget::WidgetGroup>& active,
           std::shared_ptr<widget::WidgetGroup>& parent,
           util::SoundPlayer &soundPlayer);

  /**
   * @brief Updates the settings
   *
   * All the internal logic for handling the settings will be handled here.
   *
   * @param timePassed The amount of time that has passed since the last frame.
   */
  void update(double timePassed);

  /**
   * @return The main widget group / the entry point of the settings.
   */
  std::shared_ptr<widget::WidgetGroup> getMainWidgetGroup() {
    return m_settingsWidgetGroup;
  }

private:

  util::SoundPlayer &m_soundPlayer;

  std::shared_ptr<widget::WidgetGroup>& m_activeWidgetGroup;
  std::shared_ptr<widget::WidgetGroup>& m_parentWidgetGroup;

  std::shared_ptr<widget::WidgetGroup> m_settingsWidgetGroup{};

  std::shared_ptr<widget::WidgetGroup> m_gameSettingsWidgetGroup{};
  std::shared_ptr<widget::WidgetGroup> m_inputSettingsWidgetGroup{};
  std::shared_ptr<widget::WidgetGroup> m_graphicsSettingsWidgetGroup{};
  std::shared_ptr<widget::WidgetGroup> m_audioSettingsWidgetGroup{};


  std::shared_ptr<widget::TextInput> m_mouseSensitivityInput{};
  std::shared_ptr<widget::Slider> m_mouseSensitivitySlider{};

  std::shared_ptr<widget::Slider> m_fovSlider{};
  std::shared_ptr<widget::Slider> m_renderDistanceSlider{};

  std::shared_ptr<widget::Slider> m_masterVolumeSlider{};
  std::shared_ptr<widget::TextInput> m_masterVolumeInput{};

  std::shared_ptr<widget::Slider> m_musicVolumeSlider{};
  std::shared_ptr<widget::TextInput> m_musicVolumeInput{};

  std::shared_ptr<widget::Slider> m_soundVolumeSlider{};
  std::shared_ptr<widget::TextInput> m_soundVolumeInput{};


  std::shared_ptr<widget::TextInput> m_fovTextInput{};
  std::shared_ptr<widget::TextInput> m_renderDistanceInput{};
};

#endif /* SRC_SETTINGS_H_ */
