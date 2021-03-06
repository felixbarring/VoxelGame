#ifndef SRC_GAME_H_
#define SRC_GAME_H_

#include <memory>

#include "ThreadPool.h"
#include "iGameState.h"

#include "config/data.h"
#include "graphics/graphicsManager.h"
#include "util/fpsManager.h"

#include "model/world/chunk/chunkManager.h"
#include "model/world/chunk/creationOptions.h"
#include "util/soundPlayer.h"

/**
 *
 */
class Game {
public:
  /**
   *
   */
  void run();

  /**
   *
   * @param options
   */
  void createWorld(chunk::CreationOptions options);

  /**
   *
   */
  void changeStateToMainMenu();

  /**
   *
   */
  void quitGame();

private:
  util::FPSManager m_fpsManager{config::graphics_data::fps};
  util::SoundPlayer m_soundPlayer{};

  // TODO It should be possible to not have this as a ptr.
  std::unique_ptr<graphics::GraphicsManager> m_graphicsmanager;

  sf::Window* m_window{};

  std::unique_ptr<IGameState> m_mainMenu{};
  std::unique_ptr<IGameState> m_inGame{};
  IGameState* m_currentState{};

  bool m_quit{false};
};

#endif /* SRC_GAME_H_ */
