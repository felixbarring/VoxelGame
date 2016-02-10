
#ifndef SRC_INGAME_H_
#define SRC_INGAME_H_

#include "iGameState.h"
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
#include "graphics/sprite.h"

class InGame : public IGameState {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	InGame(Game *game, std::string name);

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

	glm::mat4 m_virtualProjection;

	Game *m_game;
	std::string m_name;
	entity::Player m_player;

	std::shared_ptr<widget::WidgetGroup> m_widgetGroup1;
	std::shared_ptr<graphics::Sprite> m_crossHair;

	graphics::CubeMap skybox;

};


#endif /* SRC_INGAME_H_ */
