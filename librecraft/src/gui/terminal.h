
#ifndef SRC_GUI_TERMINAL_H_
#define SRC_GUI_TERMINAL_H_

#include <vector>
#include <string>
#include <memory>

#include "../gui/widget/widgetGroup.h"

namespace gui {

class Terminal {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	Terminal(std::vector<std::string> commands);

	virtual ~Terminal() {};

// ########################################################
// Member Functions########################################
// ########################################################

	void update(float timePassed);

	void draw();

// ########################################################
// Implementation #########################################
// ########################################################

private:

	std::vector<std::string> m_commands;
	std::shared_ptr<widget::WidgetGroup> m_widgets;

};

} /* namespace widget */

#endif /* SRC_GUI_TERMINAL_H_ */
