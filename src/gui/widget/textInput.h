
#ifndef SRC_GUI_WIDGET_TEXTINPUT_H_
#define SRC_GUI_WIDGET_TEXTINPUT_H_

#include "abstractWidget.h"

#include <string>

#include "../../graphics/fontMeshBuilder.h"
#include "../../graphics/sprite.h"

namespace widget {

class TextInput : public AbstractWidget {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	TextInput(int id, int x, int y, int width, int height);

	virtual ~TextInput() {};

// ########################################################
// Member Functions########################################
// ########################################################

	void draw() override;

	void update() override;

	void mouseClicked(int button, float x, float y) override;

	void mouseMoved(float x, float y) override;

	void keyTyped(char value) override;

// ########################################################
// Instance Variables #####################################
// ########################################################

private:

	bool hasFocus = false;
	bool pointerInsideBorders = false;

	std::string input = "";

	std::shared_ptr<graphics::Sprite> sprite;
	std::shared_ptr<graphics::Sprite> spriteHover;

	std::shared_ptr<graphics::Sprite> text;

};

} /* namespace widget */

#endif /* SRC_GUI_WIDGET_TEXTINPUT_H_ */
