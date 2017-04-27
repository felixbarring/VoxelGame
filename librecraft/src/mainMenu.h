#ifndef SRC_MAINMENU_H_
#define SRC_MAINMENU_H_

#include <memory>

#include "iGameState.h"
#include "game.h"
#include "graphics/graphicsManager.h"

#include "util/input.h"

#include "gui/image.h"
#include "gui/widget/button.h"
#include "gui/widget/label.h"
#include "gui/widget/textInput.h"
#include "gui/widget/slider.h"
#include "gui/widget/widgetGroup.h"
#include "gui/widget/selectableList.h"
#include "gui/guiUtil.h"
#include "gui/mouse.h"
#include "gui/terminal.h"
#include "settings.h"

/**
 * @brief Class that is responsible to provide the main menu.
 *
 * Extends the game state interface. The update function will update the
 * internal logic of the main menu. Logic involves changing settings, creating
 * new worlds and loading existin ones.
 */
class MainMenu : public IGameState {
public:

  /**
   * @brief Creates an instance of the MainMenu.
   *
   * @param game The game class instance. Callback functions in this class will
   *             be called when changing state to ingame.
   * @param soundPlayer The soundplayer that will be used to play sounds on
   *                    various GUI events.
   * @param graphicsManager Needed for drawing 2d and 3d graphics.
   */
  MainMenu(Game &game, util::SoundPlayer& soundPlayer,
      graphics::GraphicsManager &graphicsManager);

  void update(double timePassed) override;

private:

  Game *m_game;
  glm::mat4 m_virtualProjection{};

  util::SoundPlayer &m_soundPlayer;
  graphics::GraphicsManager &m_graphicsManager;
  gui::Mouse m_mouse;

  Settings m_settings;
  bool m_inSettings{false};

  chunk::CreationOptions m_options;

  std::unique_ptr<gui::Image> m_title{};
  std::shared_ptr<widget::SelectableList> m_worldList{};

  std::shared_ptr<widget::WidgetGroup> m_activeWidgetGroup{};

  std::shared_ptr<widget::WidgetGroup> m_mainWidgetGroup{};

  std::shared_ptr<widget::WidgetGroup> m_playWidgetGroup{};
  std::shared_ptr<widget::WidgetGroup> m_newWorldWidgetGroup{};
  std::shared_ptr<widget::WidgetGroup> m_newWorldWidgetGroupAdvanced{};

  std::shared_ptr<widget::WidgetGroup> m_loadWorldWidgetGroup{};

  std::shared_ptr<widget::WidgetGroup> m_errorEmptyName{};
  std::shared_ptr<widget::WidgetGroup> m_errorUsedName{};

  std::shared_ptr<widget::TextInput> m_textInput3{};

  std::shared_ptr<widget::ToggleButton> button1;
  std::shared_ptr<widget::ToggleButton> button2;
  std::shared_ptr<widget::ToggleButton> button3;
  std::shared_ptr<widget::ToggleButton> button4;

};

std::string randomName();

#endif /* SRC_MAINMENU_H_ */
