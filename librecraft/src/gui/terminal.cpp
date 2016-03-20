
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

Terminal::Terminal(vector<string> commands)
	: m_commands(commands)
{

	auto observer = [](int id)
	{
	};

	m_widgets = make_shared<WidgetGroup>(0, 100, 100, 600, 400, observer, 5);
	m_widgets->addWidget(make_shared<Button>(
			0, 545, 110, 150, 30, observer, "Close", 6));
	m_widgets->addWidget(make_shared<TextInput>(
			1, 110, 110, 430, 30));
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
