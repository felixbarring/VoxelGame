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
#include "gui/mouse.h"
#include "gui/terminal.h"
#include "model/world/timeAndWeather/timeCycle.h"
#include "settings.h"

/**
 * @brief This class is responsible to handle all the mayor ingame abstractions.
 *
 * Updates the inagme state, controls the HUD and the GUI.
 */
class InGame: public IGameState {
public:

  /**
   * @brief Creates an instance of the InGame class.
   *
   * @param game The game class instance. Callback functions in this class will
   *             be called when changing state to ingame.
   * @param chunkManager The ChunkManager that handles all the chunks. The
   *                     user is responsible to create and make it ready for
   *                     use before calling this constructor, and then moving
   *                     it in, giving the ownership to this instance.
   * @param soundPlayer The SoundPlayer that will be used to play sounds with.
   * @param graphicsManager Needed for drawing 2d and 3d graphics.
   */
  InGame(Game &game, chunk::ChunkManager &&chunkManager,
      util::SoundPlayer &soundPlayer,
      graphics::GraphicsManager &graphicsManager);

  void update(double timePassed) override;

private:

  enum class GameState {
    NoOverlay,
    OverlayMenu,
    Terminal
  };

  GameState m_state = GameState::NoOverlay;
  std::shared_ptr<gui::Terminal> m_terminal;
  glm::mat4 m_virtualProjection;

  Game &m_game;
  chunk::ChunkManager m_chunkManager;
  entity::Player m_player;

  util::SoundPlayer &m_soundPlayer;
  graphics::GraphicsManager &m_graphicsManager;

  gui::Mouse m_mouse;

  TimeCycle m_timeCycle;

  bool m_displayDebugInfo{false};

  double m_fpsDisplayCounter{0};
  // The fps should only be drawn 5 times/second
  double m_fpsDisplayDelay{1.0 / 5.0};

  Settings m_settings;

  std::shared_ptr<widget::WidgetGroup> m_activeWidgetGroup;
  std::shared_ptr<widget::WidgetGroup> m_mainWidgetGroup;

  std::shared_ptr<graphics::Sprite> m_crossHair;
  std::shared_ptr<graphics::Sprite> m_direction;
  std::shared_ptr<graphics::Sprite> m_fps;
  std::shared_ptr<graphics::Sprite> m_lastSelecteCube;

  std::vector<std::shared_ptr<graphics::Sprite>> m_selectedCubeThumbnails;

};

#endif /* SRC_INGAME_H_ */
