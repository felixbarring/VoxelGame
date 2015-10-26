
#ifndef SRC_MAINMENU_H_
#define SRC_MAINMENU_H_

#include <memory>

#include "game.h"

#include "util/input.h"

#include "gui/widget/button.h"
#include "gui/widget/label.h"
#include "gui/widget/textInput.h"
#include "gui/widget/slider.h"
#include "gui/widget/widgetGroup.h"
#include "gui/guiUtil.h"

class MainMenu {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	MainMenu(Game *game);

	virtual ~MainMenu() {};

// ########################################################
// Member Functions########################################
// ########################################################

	void update();

// ########################################################
// Instance Variables #####################################
// ########################################################

enum class MainMenuState {
	MainMenu,
	Play,
	Settings,
	GameSettings
};

private:

	Game *game;
	MainMenuState state = MainMenuState::MainMenu;

	std::shared_ptr<widget::WidgetGroup> activeWidgetGroup;

	std::shared_ptr<widget::WidgetGroup> mainWidgetGroup;
	std::shared_ptr<widget::WidgetGroup> playWidgetGroup;
	std::shared_ptr<widget::WidgetGroup> settingsWidgetGroup;
	std::shared_ptr<widget::WidgetGroup> gameSettingsWidgetGroup;

};


#endif /* SRC_MAINMENU_H_ */
