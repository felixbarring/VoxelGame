
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

	TextInput(int id, int x, int y, int width, int height, int layer = 0);

	virtual ~TextInput() {};

// ########################################################
// Member Functions########################################
// ########################################################

	void draw() override;

	void update() override;

	void mouseClicked(int button, float x, float y) override;

	void mouseMoved(float x, float y) override;

	void keyTyped(char value) override;

	void setString(std::string str);

	std::string getString();

// ########################################################
// Instance Variables #####################################
// ########################################################

private:

	bool m_hasFocus{false};
	bool m_pointerInsideBorders{false};

	std::string m_input{""};

	std::shared_ptr<graphics::Sprite> m_sprite;
	std::shared_ptr<graphics::Sprite> m_text;

};

} /* namespace widget */

#endif /* SRC_GUI_WIDGET_TEXTINPUT_H_ */
