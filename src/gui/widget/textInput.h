
#ifndef SRC_GUI_WIDGET_TEXTINPUT_H_
#define SRC_GUI_WIDGET_TEXTINPUT_H_

#include "abstractWidget.h"

#include <string>

namespace widget {

class TextInput : public AbstractWidget {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	TextInput(int id, int x, int y, int width, int height, std::function<void(int, const std::string&)> observer);

	virtual ~TextInput() {};

// ########################################################
// Member Functions########################################
// ########################################################

	void draw() override;

	void mouseClicked(int button, float x, float y) override;

	void mouseMoved(float x, float y) override;

	void keyPressed(int key, char c) override;

// ########################################################
// Instance Variables #####################################
// ########################################################

	std::function<void(int, const std::string&)> observer;

	std::string input = "";

};

} /* namespace widget */

#endif /* SRC_GUI_WIDGET_TEXTINPUT_H_ */
