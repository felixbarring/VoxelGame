
#ifndef SRC_GUI_TERMINAL_H_
#define SRC_GUI_TERMINAL_H_

#include <vector>
#include <string>
#include <memory>

#include "../gui/widget/textInput.h"
#include "../gui/widget/widgetGroup.h"
#include "widget/textArea.h"

namespace gui {

class Terminal {
public:
// ########################################################
// Constructor/Destructor #################################
// ########################################################

	Terminal(std::vector<std::string> commands,
			std::function<void(std::string)> commandListener);

	virtual ~Terminal() {}

// ########################################################
// Member Functions########################################
// ########################################################

	void update(float timePassed);

	void draw();

// ########################################################
// Implementation #########################################
// ########################################################

private:

	std::function<void(std::string)> m_commandListener;

	std::shared_ptr<widget::TextArea> m_textArea;
	std::shared_ptr<widget::TextInput> m_textInput;

	std::vector<std::string> m_commands;
	std::shared_ptr<widget::WidgetGroup> m_widgets;
};

} /* namespace widget */

#endif /* SRC_GUI_TERMINAL_H_ */
