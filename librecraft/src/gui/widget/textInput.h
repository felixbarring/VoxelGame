
#ifndef SRC_GUI_WIDGET_TEXTINPUT_H_
#define SRC_GUI_WIDGET_TEXTINPUT_H_

#include "abstractWidget.h"

#include <string>
#include <memory>

#include "../../graphics/fontMeshBuilder.h"
#include "../../graphics/sprite.h"

namespace widget {

class TextInput : public AbstractWidget {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	TextInput(int id, int x, int y, unsigned width, int height, int layer = 0);

	virtual ~TextInput() {};

// ########################################################
// Member Functions########################################
// ########################################################

	void setString(std::string str);

	std::string getString();

	void draw() override;

	void update() override;

// ########################################################
// Instance Variables #####################################
// ########################################################

private:

	bool m_hasFocus{false};

	std::string m_input{""};
	unsigned m_maxInputLength;

	std::shared_ptr<graphics::Sprite> m_sprite;
	std::shared_ptr<graphics::Sprite> m_text;

};

} /* namespace widget */

#endif /* SRC_GUI_WIDGET_TEXTINPUT_H_ */
