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
#include "ThreadPool.h"

#include "config/data.h"
#include "util/fpsManager.h"

#include "model/world/chunk/chunkManager.h"

class Game {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

// ########################################################
// Member Functions########################################
// ########################################################

	void run();

	void createWorld(chunk::ChunkManager::CreationOptions options);

	void changeStateToMainMenu();

	void quitGame();

// ########################################################
// Implementation #########################################
// ########################################################

private:

	util::FPSManager m_fpsManager{config::graphics_data::fps};

	sf::Window *window{};

	std::shared_ptr<IGameState> m_mainMenu{};
	std::shared_ptr<IGameState> m_inGame{};
	std::shared_ptr<IGameState> m_currentState{};

	bool m_changeStateToIngame{false};
	bool m_quit{false};
};

#endif /* SRC_GAME_H_ */
