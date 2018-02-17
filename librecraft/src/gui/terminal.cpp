#include "terminal.h"

#include <functional>
#include <memory>
#include <regex>

#include "../gui/widget/textInput.h"
#include "../util/input.h"

using namespace std;
using namespace widget;

namespace gui {

Terminal::History::History(unsigned maxLenght)
 : m_history(maxLenght, "") {
}

void Terminal::History::addToHistory(std::string str) {
 moveAllRight();
 m_history[1] = std::move(str);
 m_historyPointer = 0;
 ++m_actualElements;
}

void Terminal::History::incrementPointer() {
 unsigned ne = m_historyPointer + 1;
 if (ne < m_history.size() && ne < m_actualElements)
   ++m_historyPointer;
}

void Terminal::History::decrementPointer() {
 if (m_historyPointer > 0)
   --m_historyPointer;
}

const std::string& Terminal::History::getPointedElement() {
 return m_history[m_historyPointer];
}

void Terminal::History::moveAllRight() {
 for (long unsigned i{m_history.size() - 1}; i >= 1; --i)
   m_history[i] = m_history[i - 1];
}


unsigned m_actualElements{1};
unsigned m_historyPointer{0};
std::vector<std::string> m_history{""};

Terminal::Terminal(vector<string> commands,
                   graphics::GraphicsManager& graphicsManager,
                   function<void(vector<string>)> commandListener)
  : m_graphicsManager{graphicsManager}
  , m_commands(commands) {

  for (string command : commands)
    m_trie.addString(move(command));

  m_commandListener = commandListener;

  auto observer = [this](int id) {

    switch (id) {
      case 2: {
        string str = m_textInput->getString();
        if (str == "")
          break;
        m_textArea->addLine(str);
        m_history.addToHistory(str);

        regex r("\\S+");
        smatch match;
        vector<string> results{};
        while (regex_search(str, match, r)) {
          results.push_back(match.str());
          str = match.suffix();
        }

        m_commandListener(results);
        m_textInput->setString("");
        break;
      }
      case 3: {
        m_commandListener({"close"});
        // Hack, we lose focus when clicking on close. Next time terminal is
        // opened there is no focus. Fixed by this hack :p
        m_textInput->setFocus();
        break;
      }
    }
  };

  m_widgets =
    make_shared<WidgetGroup>(0, 100, 100, 600, 400, m_graphicsManager, 5);
  m_textInput =
    make_shared<TextInput>(1, 110, 110, 430, 30, m_graphicsManager, 6);
  m_textInput->setFocus();
  m_widgets->addWidget(m_textInput);

  m_enterButton = make_shared<Button>(
    2, 545, 110, 70, 30, m_graphicsManager, observer, "Enter", 6);

  m_widgets->addWidget(m_enterButton);
  m_widgets->addWidget(make_shared<Button>(
    3, 545 + 75, 110, 70, 30, m_graphicsManager, observer, "Close", 6));

  m_textArea = make_shared<TextArea>(
    0, 110, 150, 580, 340, m_graphicsManager, observer, 7);
  m_widgets->addWidget(m_textArea);
}

void
Terminal::update(float timePassed) {
  if (m_skipNextUpdate) {
    m_skipNextUpdate = false;
    return;
  }

  m_widgets->update(timePassed);

  if (util::Input::getInstance()->tabPressed) {
    string str{m_textInput->getString()};
    string autoComplete{m_trie.getFirstWordWithSequence(str)};
    if (!autoComplete.empty())
      m_textInput->setString(autoComplete);
  }
  if (util::Input::getInstance()->enterPressed) {
    m_enterButton->trigger();
  }
  if (util::Input::getInstance()->upPressed) {
    m_history.incrementPointer();
    m_textInput->setString(m_history.getPointedElement());
  }
  if (util::Input::getInstance()->downPressed) {
    m_history.decrementPointer();
    m_textInput->setString(m_history.getPointedElement());
  }
}

void
Terminal::draw() {
  m_widgets->draw();
}

void
Terminal::addLine(string str) {
  m_textArea->addLine(move(str));
}

void
Terminal::skipNextUpdate() {
  m_skipNextUpdate = true;
}

} /* namespace widget */
