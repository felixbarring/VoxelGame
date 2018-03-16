
#ifndef SRC_SETTINGS_H_
#define SRC_SETTINGS_H_

#include "graphics/graphicsManager.h"
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
   * @param graphicsManager Needed for drawing 2d and 3d graphics.
   */
  Settings(widget::WidgetGroup*& active,
           widget::WidgetGroup*& parent,
           util::SoundPlayer& soundPlayer,
           graphics::GraphicsManager& graphicsManager);

  /**
   * @return The main widget group / the entry point of the settings.
   */
  widget::WidgetGroup* getMainWidgetGroup() const;

private:
  util::SoundPlayer& m_soundPlayer;
  graphics::GraphicsManager& m_graphicsManager;

  std::vector<std::unique_ptr<widget::IWidget>> m_widgets;

  widget::WidgetGroup*& m_activeWidgetGroup;

  widget::WidgetGroup*& m_parentWidgetGroup;

  std::unique_ptr<widget::WidgetGroup> m_settingsWidgetGroup{};

  std::unique_ptr<widget::WidgetGroup> m_gameSettingsWidgetGroup{};
  std::unique_ptr<widget::WidgetGroup> m_inputSettingsWidgetGroup{};
  std::unique_ptr<widget::WidgetGroup> m_graphicsSettingsWidgetGroup{};
  std::unique_ptr<widget::WidgetGroup> m_audioSettingsWidgetGroup{};

  std::unique_ptr<widget::TextInput> m_mouseSensitivityInput{};
  std::unique_ptr<widget::Slider> m_mouseSensitivitySlider{};

  std::unique_ptr<widget::Slider> m_fovSlider{};
  std::unique_ptr<widget::Slider> m_renderDistanceSlider{};

  std::unique_ptr<widget::Slider> m_masterVolumeSlider{};
  std::unique_ptr<widget::TextInput> m_masterVolumeInput{};

  std::unique_ptr<widget::Slider> m_musicVolumeSlider{};
  std::unique_ptr<widget::TextInput> m_musicVolumeInput{};

  std::unique_ptr<widget::Slider> m_soundVolumeSlider{};
  std::unique_ptr<widget::TextInput> m_soundVolumeInput{};

  std::unique_ptr<widget::TextInput> m_fovTextInput{};
  std::unique_ptr<widget::TextInput> m_renderDistanceInput{};
};

#endif /* SRC_SETTINGS_H_ */
