
#include "inGame.h"

#include "graphics/chunkBatcher.h"
#include "graphics/cubeBatcher.h"

InGame::InGame(Game *game, util::Input &in) :
	input(in),
	player(in),
	game(game)
{
	player.setLocation(7, 7, 7);
}

InGame::~InGame()
{
}


void InGame::update()
{
	input.lockMouse();
	input.updateValues();

	player.update(0.0f);

	glEnable(GL_DEPTH_TEST);
	graphics::ChunkBatcher::getInstance().draw();
	graphics::CubeBatcher::getInstance().draw();
}
