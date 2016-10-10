
#include "terminal.h"

#include <memory>

#include "../gui/widget/textInput.h"
#include "../util/input.h"

using namespace std;
using namespace widget;

namespace gui {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Terminal::Terminal(vector<string> commands,	function<void(string, vector<string>)> commandListener)
	: m_commands(commands)
{

	for (auto command : commands)
		m_trie.addString(command);

	m_commandListener = commandListener;

//	string s = "foo bar  baz";
//	regex e("\\s+");
//	regex_token_iterator<string::iterator> i(s.begin(), s.end(), e, -1);
//	regex_token_iterator<string::iterator> end;
//	while (i != end)
//	   cout << " [" << *i++ << "]";


	auto observer = [this](int id)
	{
		auto arg = vector<string>{};

		switch (id)
		{
		case 2: {
			auto str =  m_textInput->getString();
			m_textArea->addLine(str);
			m_commandListener(str, arg);
			m_textInput->setString("");
			break;
		}
		case 3: {
			m_commandListener("close", arg);
			m_textInput->setFocus(); // Hack, we lose focus when clicking on close. Next time terminal is opened there
									// is no focus. Fixed by this hack :p
			break;
		}
		}
	};

	m_widgets = make_shared<WidgetGroup>(0, 100, 100, 600, 400, observer, 5);
	m_textInput = make_shared<TextInput>(1, 110, 110, 430, 30, 6);
	m_textInput->setFocus();
	m_widgets->addWidget(m_textInput);

	m_enterButton = make_shared<Button>(2, 545, 110, 70, 30, observer, "Enter", 6);

	m_widgets->addWidget(m_enterButton);
	m_widgets->addWidget(make_shared<Button>(3, 545 + 75, 110, 70, 30, observer, "Close", 6));

	m_textArea = make_shared<TextArea>(0, 110, 150, 580, 340, observer, 7);
	m_widgets->addWidget(m_textArea);
}

// ########################################################
// Member Functions########################################
// ########################################################

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
}

void Terminal::draw() {
	m_widgets->draw();
}

void Terminal::addLine(std::string str) {
	m_textArea->addLine(move(str));
}

} /* namespace widget */
