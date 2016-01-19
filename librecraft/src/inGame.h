
#ifndef SRC_INGAME_H_
#define SRC_INGAME_H_

#include "game.h"

#include "model/world/chunk/chunk.h"
#include "model/world/entity/player.h"

#include "util/input.h"

#include "gui/widget/button.h"
#include "gui/widget/label.h"
#include "gui/widget/widgetGroup.h"
#include "gui/guiUtil.h"

#include "graphics/cubeMap.h"
#include "graphics/resources.h"

class InGame {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	InGame(Game *game);

	virtual ~InGame() {};

// ########################################################
// Member Functions########################################
// ########################################################

	void update(float timePassed);

// ########################################################
// Instance Variables #####################################
// ########################################################

private:

	enum class GameState {
		NoOverlay,
		OverlayMenu,
	};

	GameState state = GameState::NoOverlay;

	Game *game;
	entity::Player player;

	std::shared_ptr<widget::WidgetGroup> widgetGroup1;

	graphics::CubeMap skybox;

};


#endif /* SRC_INGAME_H_ */
