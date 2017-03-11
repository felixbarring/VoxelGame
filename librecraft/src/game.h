#ifndef SRC_GAME_H_
#define SRC_GAME_H_

#include <memory>

#include "iGameState.h"
#include "ThreadPool.h"

#include "config/data.h"
#include "util/fpsManager.h"

#include "model/world/chunk/chunkManager.h"
#include "model/world/chunk/creationOptions.h"

class Game {
public:

  void run();

  void createWorld(chunk::CreationOptions options);

  void changeStateToMainMenu();

  void quitGame();

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
