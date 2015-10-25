
#ifndef SRC_MAINMENU_H_
#define SRC_MAINMENU_H_

#include <memory>

#include "game.h"

#include "util/input.h"

#include "gui/widget/button.h"
#include "gui/widget/label.h"
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

private:

	Game *game;

	std::shared_ptr<widget::WidgetGroup> widgetGroup1;

};


#endif /* SRC_MAINMENU_H_ */
