
#ifndef SRC_GUI_TERMINAL_H_
#define SRC_GUI_TERMINAL_H_

#include <vector>
#include <string>
#include <memory>

#include "../gui/widget/button.h"
#include "../gui/widget/textInput.h"
#include "../gui/widget/widgetGroup.h"
#include "widget/textArea.h"
#include "../util/trie.h"

namespace gui {

class Terminal {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

    Terminal(std::vector<std::string> commands,	std::function<void(std::vector<std::string>)> commandListener);

// ########################################################
// Member Functions########################################
// ########################################################

    void update(float timePassed);

    void draw();

    void addLine(std::string str);

// ########################################################
// Implementation #########################################
// ########################################################

private:

    class History {
    public:

        History(unsigned maxLenght)
            : m_history(maxLenght, "")
        { }

        void addToHistory(std::string str) {
            moveAllRight();
            m_history[0] = std::move(str);
            m_historyPointer = -1;
            m_actualElements == m_history.size() ? m_actualElements : ++m_actualElements;
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
