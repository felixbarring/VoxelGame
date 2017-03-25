
#ifndef SRC_SETTINGS_H_
#define SRC_SETTINGS_H_

#include "gui/widget/slider.h"
#include "gui/widget/textInput.h"
#include "gui/widget/widgetGroup.h"

class Settings {
public:

  Settings(std::shared_ptr<widget::WidgetGroup>& active,
           std::shared_ptr<widget::WidgetGroup>& parrent);

  void update(double timePassed);

  std::shared_ptr<widget::WidgetGroup> getMainWidgetGroup() {
    return m_settingsWidgetGroup;
  }

private:

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
