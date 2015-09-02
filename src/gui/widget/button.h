
/**
 *  \file [button.h]
 *  \brief A button that can be pressed by a mouse pointer.
 *  \author Felix Bärring
 *
 *  Copyright (c) 2015, Felix Bärring. All rights reserved.
 */

#ifndef SRC_GUI_WIDGET_BUTTON_H_
#define SRC_GUI_WIDGET_BUTTON_H_

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
 * that it is interactable by highlight itself.
 *
 * \author Felix Bärring
 */
class Button : public AbstractWidget  {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	Button(int id, int x, int y, int width, int height);

	virtual ~Button();

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

private:

	bool pointerInsideBorders = false;

	std::shared_ptr<graphics::Sprite> sprite;
	std::shared_ptr<graphics::Sprite> spriteHover;

};

} /* namespace widget */

#endif /* SRC_GUI_WIDGET_BUTTON_H_ */
