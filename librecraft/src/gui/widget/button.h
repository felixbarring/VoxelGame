
#ifndef SRC_GUI_WIDGET_BUTTON_H_
#define SRC_GUI_WIDGET_BUTTON_H_

#include <string>
#include <memory>

#include "abstractWidget.h"

#include "../../graphics/sprite.h"

namespace widget {

/**
 * \class Button
 *
 * A button can be used to trigger an event when the user clicks on it.
 * Buttons can have a text/name that will be rendered and hence provide
 * the user with visual information about the purpose of the button.
 *
 * When the mouse pointer is inside the buttons border it will hint
 * that it is interactable by highlight it self.
 *
 * \author Felix Bärring
 */
class Button: public AbstractWidget {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	Button(int id, int x, int y, int width, int height,
			std::function<void(int)> observer, std::string name, int layer = 0);

	virtual ~Button() {};

// ########################################################
// Member Functions########################################
// ########################################################

	std::string getName();

	void draw() override;

	void update(float timePassed) override;

	/**
	 * Makes the button behave as if had been clicked.
	 */
	void trigger();

// ########################################################
// Implementation #########################################
// ########################################################

protected:

	std::function<void(int)> m_observer;

	bool m_pointerInsideBorders{false};
	std::string m_name;

	std::shared_ptr<graphics::Sprite> m_sprite;
	std::shared_ptr<graphics::Sprite> m_highlight;
	std::shared_ptr<graphics::Sprite> m_text;
};

} /* namespace widget */

#endif /* SRC_GUI_WIDGET_BUTTON_H_ */
