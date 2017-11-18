#ifndef SRC_GUI_TERMINAL_H_
#define SRC_GUI_TERMINAL_H_

#include <vector>
#include <string>
#include <memory>
#include <functional>

#include "../gui/widget/button.h"
#include "../gui/widget/textInput.h"
#include "../gui/widget/widgetGroup.h"
#include "widget/textArea.h"
#include "../util/trie.h"

namespace gui {

/**
 * @brief Can be used to let the user type in commands.
 *
 * The terminal is designed to have a easy to use interface. It supports letting
 * the user type in commands in an input text field. When hitting enter or
 * pressing the enter button the callback function will be called with the
 * entered string. There is also a text area that gives feedback to the user,
 * such as if the command was valid and any possible results of the command.
 * The terminal also allows for auto completion on the commands provided in the
 * constructor by hitting the tab button. The terminal also keeps history of the
 * strings entered and allows the user to go thru the history using the up and
 * down button.
 *
 * The user should call update once each frame so that it can handle user input
 * and send callbacks.
 */
class Terminal {
public:

  /**
   * @brief Constructs a Terminal.
   *
   * @param commands The commands that the user can auto complete on by pressing
   *                  tab
   * @param commandListener The callback function that will be called when the
   *                        user presses the keyboard enter or the widget enter
   *                        button.
   */
  Terminal(std::vector<std::string> commands,
      graphics::GraphicsManager &graphicsManager,
      std::function<void(std::vector<std::string>)> commandListener);

  /**
   * @brief Updates the terminal.
   *
   * Should be called once each frame. Handles all the logic of the terminal,
   * such as sending commands to the callback function, using auto completion,
   * and going thru the history of entered strings.
   *
   * @param timePassed
   */
  void update(float timePassed);

  /**
   * @brief Draws all the widgets that the terminal is made up of.
   */
  void draw();

  /**
   * @brief Adds a new line to the text area.
   *
   * Useful for giving the user feedback of a command, or debugg information and
   * similar. The added string can be of any realistic size since it will be
   * clipped if it is longer that can fit in one line of the textarea.
   *
   * @param str The string that shall be added.
   */
  void addLine(std::string str);

private:

  class History {
  public:

    explicit History(unsigned maxLenght)
      : m_history(maxLenght, "")
    {
    }

    void addToHistory(std::string str) {
      moveAllRight();
      m_history[0] = std::move(str);
      m_historyPointer = -1;
      m_actualElements == m_history.size() ?
          m_actualElements : ++m_actualElements;
    }

    void incrementPointer() {
      auto ne = m_historyPointer + 1;
      if (ne < m_history.size() && ne < m_actualElements)
        ++m_historyPointer;
    }

    void decrementPointer() {
      if (m_historyPointer > 0)
        --m_historyPointer;
    }

    std::string& getPointedElement() {
      return m_history[m_historyPointer];
    }

  private:

    void moveAllRight() {
      for (unsigned i = m_history.size() - 1; i >= 1; --i)
        m_history[i] = m_history[i - 1];
    }

    unsigned m_actualElements{0};
    unsigned m_historyPointer{0};
    std::vector<std::string> m_history;
  };

  graphics::GraphicsManager &m_graphicsManager;

  std::function<void(std::vector<std::string>)> m_commandListener;

  std::shared_ptr<widget::TextArea> m_textArea{};
  std::shared_ptr<widget::TextInput> m_textInput{};

  util::Trie m_trie{};

  unsigned m_historyLenght{10};
  History m_history{m_historyLenght};

  std::vector<std::string> m_commands{};

  std::shared_ptr<widget::Button> m_enterButton;
  std::shared_ptr<widget::WidgetGroup> m_widgets{};
};

} /* namespace widget */

#endif /* SRC_GUI_TERMINAL_H_ */
