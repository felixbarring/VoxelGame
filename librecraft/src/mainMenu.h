
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
#include "gui/guiUtil.h"

class MainMenu : public IGameState {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	MainMenu(Game *game);

	virtual ~MainMenu() {};

// ########################################################
// Member Functions########################################
// ########################################################

	void update(float timePassed);

// ########################################################
// Instance Variables #####################################
// ########################################################

private:

	Game *game;

	std::unique_ptr<gui::Image> title;

	std::shared_ptr<widget::WidgetGroup> activeWidgetGroup;

	std::shared_ptr<widget::WidgetGroup> mainWidgetGroup;

	std::shared_ptr<widget::WidgetGroup> playWidgetGroup;
	std::shared_ptr<widget::WidgetGroup> newWorldWidgetGroup;
	std::shared_ptr<widget::WidgetGroup> loadWorldWidgetGroup;

	std::shared_ptr<widget::WidgetGroup> settingsWidgetGroup;
	std::shared_ptr<widget::WidgetGroup> gameSettingsWidgetGroup;
	std::shared_ptr<widget::WidgetGroup> inputSettingsWidgetGroup;
	std::shared_ptr<widget::WidgetGroup> graphicsSettingsWidgetGroup;

	std::shared_ptr<widget::TextInput> textInput;
	std::shared_ptr<widget::TextInput> textInput2;
	std::shared_ptr<widget::TextInput> textInput3;

};


#endif /* SRC_MAINMENU_H_ */
