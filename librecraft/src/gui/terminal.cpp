
#include "terminal.h"

#include <memory>

#include "../gui/widget/textInput.h"
#include "../gui/widget/button.h"

using namespace std;
using namespace widget;

namespace gui {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Terminal::Terminal(vector<string> commands,
		function<void(string)> commandListener)
	: m_commands(commands)
{

	m_commandListener = commandListener;

	auto observer = [this](int id)
	{
		switch (id)
		{
		case 0:
			m_textArea->addLine(m_textInput->getString());
			m_commandListener.operator ()(m_textInput->getString());
			m_textInput->setString("");
		case 1:
			m_commandListener.operator ()("CLOSE");
		}
	};

	m_widgets = make_shared<WidgetGroup>(0, 100, 100, 600, 400, observer, 5);
	m_textInput = make_shared<TextInput>(1, 110, 110, 430, 30);
	m_widgets->addWidget(m_textInput);
	m_widgets->addWidget(make_shared<Button>(
			0, 545, 110, 70, 30, observer, "Enter", 6));
	m_widgets->addWidget(make_shared<Button>(
			1, 545 + 75, 110, 70, 30, observer, "Close", 6));

	m_textArea = make_shared<TextArea>(0, 110, 150, 580, 340, observer, 6);
	m_textArea->addLine("Hello World");
	m_widgets->addWidget(m_textArea);
}

// ########################################################
// Member Functions########################################
// ########################################################

void Terminal::update(float timePassed) {
	m_widgets->update(timePassed);
}

void Terminal::draw() {
	m_widgets->draw();
}

} /* namespace widget */
