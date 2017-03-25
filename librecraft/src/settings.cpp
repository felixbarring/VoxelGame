
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

Settings::Settings(
    std::shared_ptr<widget::WidgetGroup>& active,
    std::shared_ptr<widget::WidgetGroup>& parrent
)
  : m_activeWidgetGroup{active}
  , m_parentWidgetGroup{parrent}
{

  // ########################################################################

 {
   auto observer = [this](int id)
   {
     switch(id) {
       case 0: m_activeWidgetGroup = m_gameSettingsWidgetGroup; break;
       case 1: m_activeWidgetGroup = m_inputSettingsWidgetGroup; break;
       case 2: m_activeWidgetGroup = m_graphicsSettingsWidgetGroup; break;
       case 3: m_activeWidgetGroup = m_audioSettingsWidgetGroup; break;
       case 4: m_activeWidgetGroup = m_parentWidgetGroup; break;
     }
   };

   auto label = make_shared<Label>(325, 390, 150, 30, " - Settings - ");
   auto button1 = make_shared<Button>(0, 325, 350, 150, 30, observer, "Game");
   auto button2 = make_shared<Button>(1, 325, 310, 150, 30, observer, "Input");
   auto button3 = make_shared<Button>(2, 325, 270, 150, 30, observer,
       "Graphics");
   auto button4 = make_shared<Button>(3, 325, 230, 150, 30, observer, "Audio");
   auto button5 = make_shared<Button>(4, 325, 190, 150, 30, observer, "Back");

   m_settingsWidgetGroup = make_shared<WidgetGroup>(0, 300, 180, 200, 210);

   m_settingsWidgetGroup->addWidget(
       {label, button1, button2, button3, button4, button5});
 }

 // ########################################################################

 {
   auto observer = [this](int id)
   {
     switch(id) {
       case 0: m_activeWidgetGroup = m_settingsWidgetGroup; break;
     }
   };

   auto label = make_shared<Label>(325, 390, 150, 30, " - Game - ");
   auto button = make_shared<Button>(0, 325, 230, 150, 30, observer, "Back");

   m_gameSettingsWidgetGroup = make_shared<WidgetGroup>(0, 300, 220, 200, 170);

   m_gameSettingsWidgetGroup->addWidget(label);
   m_gameSettingsWidgetGroup->addWidget(button);
 }

 // ########################################################################

 {
   auto observer =
       [this](int id)
       {
         switch(id) {
           case 0: {
             config::input_data::mouseSensitivityX = (
                 m_mouseSensitivitySlider->getValue() / 100);
             config::input_data::mouseSensitivityY = (
                 m_mouseSensitivitySlider->getValue() / 100);
             m_activeWidgetGroup = m_settingsWidgetGroup; break;
           }
           case 1: {
             m_mouseSensitivityInput->setString(to_string(
                 m_mouseSensitivitySlider->getValue() / 100));
             break;
           }
         }
       };

   shared_ptr<IWidget> label(new Label{325, 390, 150, 30, " - Input - "});

   m_mouseSensitivitySlider = make_shared<Slider>(1, 125 + 100, 310, 150, 30,
       observer, 1);
   m_mouseSensitivityInput = make_shared<TextInput>(666, 125 + 100, 270, 150,
       30, 1);

   auto button1 = make_shared<Button>(0, 245, 130, 150, 30, observer, "Save",
       1);
   auto button2 = make_shared<Button>(0, 400, 130, 150, 30, observer, "Cancel",
       1);

   m_inputSettingsWidgetGroup = make_shared<WidgetGroup>(0, 100, 100, 600,
       250);

   m_inputSettingsWidgetGroup->addWidget({label, m_mouseSensitivityInput,
       m_mouseSensitivitySlider, button1, button2});
 }

 // ########################################################################

 {
   auto  observer = [this](int id)
   {
     switch(id) {
       case 0: m_activeWidgetGroup = m_settingsWidgetGroup; break;
       case 1: m_activeWidgetGroup = m_settingsWidgetGroup; break;
       case 2: {
         m_fovTextInput->setString(to_string(m_fovSlider->getValue()));
         break;
       }
       case 3: {
         m_renderDistanceInput->setString(to_string(
             m_renderDistanceSlider->getValue()));
         break;
       }
     }
   };

   auto label1 = make_shared<Label>(325, 390, 150, 30, " - Graphics - ");
   auto label2 = make_shared<Label>(30 + 100, 310, 80, 20, "FOV:", 1);

   m_fovSlider = make_shared<Slider>(2, 125 + 100, 310, 150, 30, observer, 1);
   m_fovTextInput = make_shared<TextInput>(666, 285 + 100, 310, 100, 30, 1);

   auto label3 = make_shared<Label>(30 + 100, 270, 80, 20, "Render Distance:",
       1);

   m_renderDistanceSlider = make_shared<Slider>(3, 125 + 100, 270, 150, 30,
       observer, 1);
   m_renderDistanceInput = make_shared<TextInput>(-1, 285 + 100, 270, 100, 30,
       1);

   auto button1 = make_shared<Button>(0, 245, 130, 150, 30, observer, "Save",
       1);
   auto button2 = make_shared<Button>(1, 400, 130, 150, 30, observer, "Cancel",
       1);

   m_graphicsSettingsWidgetGroup = make_shared<WidgetGroup>(0, 100, 100, 600,
       250);

   m_graphicsSettingsWidgetGroup->addWidget({label1, label2, m_fovSlider,
       m_fovTextInput, label3, m_renderDistanceSlider, m_renderDistanceInput,
       button1, button2});
 }

 // ########################################################################

 {

   auto observer = [this](int id)
   {
     int stupid{100};
     switch(id) {
       case 0: {
         SoundPlayer::getInstance().setMasterVolume(
             m_masterVolumeSlider->getValue());

         SoundPlayer::getInstance().setSoundVolume(
             stupid * m_soundVolumeSlider->getValue());

         SoundPlayer::getInstance().setMusicVolume(
             stupid * m_musicVolumeSlider->getValue());
         break;
       }
       case 1: {
         m_activeWidgetGroup = m_settingsWidgetGroup;
         break;
       }
     }
   };

   auto label1 = make_shared<Label>(325, 390, 150, 30, " - Audio - ");

   auto label2 = make_shared<Label>(230, 310, 80, 20, "Master Volume:",
       1);
   m_masterVolumeSlider = make_shared<Slider>(2, 325, 310, 150, 30,
       observer, 1);
   m_masterVolumeInput = make_shared<TextInput>(666, 485, 310, 50, 30, 1);

   auto label3 = make_shared<Label>(230, 270, 80, 20, "Music Volume:", 1);
   m_musicVolumeSlider = make_shared<Slider>(2, 325, 270, 150, 30,
       observer, 1);
   m_musicVolumeInput = make_shared<TextInput>(666, 485, 270, 50, 30,
       1);

   auto label4 = make_shared<Label>(230, 230, 80, 20, "Sound Volume:", 1);
   m_soundVolumeSlider = make_shared<Slider>(2, 325, 230, 150, 30,
       observer, 1);
   m_soundVolumeInput = make_shared<TextInput>(666, 485, 230, 50, 30, 1);


   auto button1 = make_shared<Button>(0, 245, 130, 150, 30, observer, "Save",
      1);
   auto button2 = make_shared<Button>(1, 400, 130, 150, 30, observer, "Cancel",
      1);

   m_audioSettingsWidgetGroup = make_shared<WidgetGroup>(0, 200, 120, 400,
     230);
   m_audioSettingsWidgetGroup->addWidget({label1, button1, button2,
     m_masterVolumeSlider, m_masterVolumeInput,
     m_musicVolumeSlider, m_musicVolumeInput,
     m_soundVolumeSlider, m_soundVolumeInput,
     label2, label3, label4});
 }

}

void Settings::update(double timePassed) {
  m_activeWidgetGroup->update(timePassed);
}

