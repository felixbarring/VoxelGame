#ifndef SRC_GUI_WIDGET_TEXTINPUT_H_
#define SRC_GUI_WIDGET_TEXTINPUT_H_

#include "abstractWidget.h"

#include <string>
#include <memory>

#include "../../graphics/fontMeshBuilder.h"
#include "../../graphics/sprite.h"

namespace widget {

class TextInput: public AbstractWidget {
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

	void update(float timePassed) override;

	void setFocus(bool value = true);

// ########################################################
// Implementation #########################################
// ########################################################

private:

	bool m_hasFocus{false};
	std::string m_input{""};
	unsigned m_maxInputLength;

	float m_eraseDelay{0.1f};
	float m_accumulatedEraseTime{0.0f};

	float m_blinkIntervall{1.0 / 3.0}; // Blink three times / second
	float m_blinkTime{0.0};
	bool m_cursorVissible{true};

	int m_textHeight{};

	std::shared_ptr<graphics::Sprite> m_background;
	std::shared_ptr<graphics::Sprite> m_cursor;
	std::shared_ptr<graphics::Sprite> m_text;
};

} /* namespace widget */

#endif /* SRC_GUI_WIDGET_TEXTINPUT_H_ */
