#ifndef SRC_MAINMENU_H_
#define SRC_MAINMENU_H_

#include <memory>

#include "iGameState.h"
#include "game.h"

#include "util/input.h"

#include "gui/image.h"
#include "gui/widget/button.h"
#include "gui/widget/label.h"
#include "gui/widget/textInput.h"
#include "gui/widget/slider.h"
#include "gui/widget/widgetGroup.h"
#include "gui/widget/selectableList.h"
#include "gui/guiUtil.h"
#include "gui/terminal.h"

class MainMenu: public IGameState {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	MainMenu(Game *game);

// ########################################################
// Member Functions########################################
// ########################################################

	void update(float timePassed);

// ########################################################
// Implementation #########################################
// ########################################################

private:

	std::string randomName();

	Game *m_game;

	glm::mat4 m_virtualProjection;

	std::shared_ptr<gui::Image> m_title;
	std::shared_ptr<widget::SelectableList> m_worldList;

	std::shared_ptr<widget::WidgetGroup> m_activeWidgetGroup;

	std::shared_ptr<widget::WidgetGroup> m_mainWidgetGroup;

	std::shared_ptr<widget::WidgetGroup> m_playWidgetGroup;
	std::shared_ptr<widget::WidgetGroup> m_newWorldWidgetGroup;
	std::shared_ptr<widget::WidgetGroup> m_loadWorldWidgetGroup;

	std::shared_ptr<widget::WidgetGroup> m_settingsWidgetGroup;
	std::shared_ptr<widget::WidgetGroup> m_gameSettingsWidgetGroup;
	std::shared_ptr<widget::WidgetGroup> m_inputSettingsWidgetGroup;
	std::shared_ptr<widget::WidgetGroup> m_graphicsSettingsWidgetGroup;

	std::shared_ptr<widget::TextInput> m_textInput;
	std::shared_ptr<widget::TextInput> m_textInput2;
	std::shared_ptr<widget::TextInput> m_textInput3;

	std::shared_ptr<widget::TextInput> m_mouseSensitivityInput;
	std::shared_ptr<widget::Slider> m_mouseSensitivitySlider;

	std::shared_ptr<widget::Slider> m_slider;
	std::shared_ptr<widget::Slider> m_slider2;

};

#endif /* SRC_MAINMENU_H_ */
