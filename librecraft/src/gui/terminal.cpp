
#include "terminal.h"

#include <memory>
#include <regex>

#include "../gui/widget/textInput.h"
#include "../util/input.h"

using namespace std;
using namespace widget;

namespace gui {

Terminal::Terminal(vector<string> commands,	function<void(vector<string>)> commandListener)
    : m_commands(commands)
{

    for (auto command : commands)
        m_trie.addString(command);

    m_commandListener = commandListener;

    auto observer = [this](int id)
    {

        switch (id)	{
        case 2: {
            auto str =  m_textInput->getString();
            m_textArea->addLine(str);
            m_history.addToHistory(str);

            regex r("\\S+");
            smatch match;
            vector<string> results{};
            while(regex_search(str, match, r)) {
                results.push_back(match.str());
                str = match.suffix();
            }

            m_commandListener(results);
            m_textInput->setString("");
            break;
        }
        case 3: {
            auto arg = vector<string>{};
            arg.push_back("close");
            m_commandListener(arg);
            m_textInput->setFocus(); // Hack, we lose focus when clicking on close. Next time terminal is opened there
                                     // is no focus. Fixed by this hack :p
            break;
        }
        }
    };

    m_widgets = make_shared<WidgetGroup>(0, 100, 100, 600, 400, 5);
    m_textInput = make_shared<TextInput>(1, 110, 110, 430, 30, 6);
    m_textInput->setFocus();
    m_widgets->addWidget(m_textInput);

    m_enterButton = make_shared<Button>(2, 545, 110, 70, 30, observer, "Enter", 6);

    m_widgets->addWidget(m_enterButton);
    m_widgets->addWidget(make_shared<Button>(3, 545 + 75, 110, 70, 30, observer, "Close", 6));

    m_textArea = make_shared<TextArea>(0, 110, 150, 580, 340, observer, 7);
    m_widgets->addWidget(m_textArea);
}

void Terminal::update(float timePassed) {
    m_widgets->update(timePassed);

    if (util::Input::getInstance()->tabPressed) {
        auto str =  m_textInput->getString();
        auto autoComplete = m_trie.getFirstWordWithSequence(str);
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

void Terminal::draw() {
    m_widgets->draw();
}

void Terminal::addLine(string str) {
    m_textArea->addLine(move(str));
}

} /* namespace widget */
