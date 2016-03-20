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

#include "iGameState.h"

class Game {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	Game() { };

	virtual ~Game() { };

// ########################################################
// Member Functions########################################
// ########################################################

	void run();

	void createNewWorld(std::string name);

	void loadExistingWorld(std::string name);

	void changeStateToMainMenu();

	void quitGame();

// ########################################################
// Implementation #########################################
// ########################################################

private:
	std::shared_ptr<IGameState> m_mainMenu;
	std::shared_ptr<IGameState> m_inGame;
	std::shared_ptr<IGameState> m_currentState;

	bool m_quit = false;
};

#endif /* SRC_GAME_H_ */
