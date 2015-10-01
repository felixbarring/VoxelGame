
/**
 *  \file    game.h
 *
 *  \brief :o
 *
 *  \author  Felix Bärring
 *  \date    1337
 *
 * Copyright (c) 2010, Felix Bärring. All rights reserved.
 */

#ifndef SRC_GAME_H_
#define SRC_GAME_H_

#include <memory>

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

	void changeStateToIngame();

	void changeStateToMainMenu();

	void quitGame();

// ########################################################
// Instance Variables #####################################
// ########################################################

private:

	enum class GameState {
		MainMenu,
		InGame,
	};

	GameState state = GameState::MainMenu;

	bool quit = false;
	bool blool = true;

};

#endif /* SRC_GAME_H_ */
