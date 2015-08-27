
#ifndef SRC_GAME_H_
#define SRC_GAME_H_

enum class GameState {
	MainMenu,
	InGame
};

/**
 *  \file    game.h
 *
 *  \brief   Pootis Pow HAHA
 *
 *  \author  Felix Bärring
 *  \date    1337
 *
 * Copyright (c) 2010, Felix Bärring. All rights reserved.
 */

class Game {
public:

	// ########################################################
	// Constructor/Destructor #################################
	// ########################################################

	Game();
	virtual ~Game();

	// ########################################################
	// Member Functions########################################
	// ########################################################

	void run();

	// ########################################################
	// Instance Variables #####################################
	// ########################################################

private:

	GameState gameState;

};

#endif /* SRC_GAME_H_ */
