
#ifndef SRC_INGAME_H_
#define SRC_INGAME_H_

#include "game.h"

#include "model/world/chunk/chunk.h"
#include "model/world/entity/player.h"

#include "util/input.h"

class InGame {
public:

	InGame(Game *game, util::Input &in);

	virtual ~InGame();

	void update();

private:

	Game *game;
	util::Input &input;
	entity::Player player;

};


#endif /* SRC_INGAME_H_ */
