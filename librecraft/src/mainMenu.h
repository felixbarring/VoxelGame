#ifndef SRC_MAINMENU_H_
#define SRC_MAINMENU_H_

#include <memory>

#include "game.h"
#include "graphics/graphicsManager.h"
#include "iGameState.h"

#include "util/input.h"

#include "gui/guiUtil.h"
#include "gui/image.h"
#include "gui/mouse.h"
#include "gui/terminal.h"
#include "gui/widget/button.h"
#include "gui/widget/label.h"
#include "gui/widget/selectableList.h"
#include "gui/widget/slider.h"
#include "gui/widget/textInput.h"
#include "gui/widget/widgetGroup.h"
#include "settings.h"

/**
 * @brief Class that is responsible to provide the main menu.
 *
 * Extends the game state interface. The update function will update the
 * internal logic of the main menu. Logic involves changing settings, creating
 * new worlds, loading existing ones and quitting the game.
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
  MainMenu(Game& game,
           util::SoundPlayer& soundPlayer,
           graphics::GraphicsManager& graphicsManager);

  void update(double timePassed) override;

private:
  Game& m_game;
  glm::mat4 m_virtualProjection{};

  util::SoundPlayer& m_soundPlayer;
  graphics::GraphicsManager& m_graphicsManager;
  gui::Mouse m_mouse;

  Settings m_settings;

  std::vector<std::unique_ptr<widget::IWidget>> m_widgets;

  std::unique_ptr<gui::Image> m_title{};
  std::unique_ptr<widget::SelectableList> m_worldList{};

  widget::WidgetGroup* m_activeWidgetGroup{};

  widget::WidgetGroup* m_mainWidgetGroupRaw{nullptr};
  std::unique_ptr<widget::WidgetGroup> m_mainWidgetGroup{};

  std::unique_ptr<widget::WidgetGroup> m_playWidgetGroup{};
  std::unique_ptr<widget::WidgetGroup> m_newWorldWidgetGroup{};
  std::unique_ptr<widget::WidgetGroup> m_newWorldWidgetGroupAdvanced{};

  std::unique_ptr<widget::WidgetGroup> m_loadWorldWidgetGroup{};

  std::unique_ptr<widget::WidgetGroup> m_errorEmptyName{};
  std::unique_ptr<widget::WidgetGroup> m_errorUsedName{};

  std::unique_ptr<widget::TextInput> m_textInput3{};
  std::unique_ptr<widget::TextInput> m_textInput4{};

  std::unique_ptr<widget::ToggleButton> button1;
  std::unique_ptr<widget::ToggleButton> button2;
  std::unique_ptr<widget::ToggleButton> button3;
  std::unique_ptr<widget::ToggleButton> button4;
};

#endif /* SRC_MAINMENU_H_ */
