
/**
 *  \file [button.h]
 *  \brief A button that can be pressed by a mouse pointer.
 *  \author Felix Bärring
 *
 *  Copyright (c) 2015, Felix Bärring. All rights reserved.
 */

#ifndef SRC_GUI_WIDGET_BUTTON_H_
#define SRC_GUI_WIDGET_BUTTON_H_

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
class Button {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	Button();

	virtual ~Button();

// ########################################################
// Member Functions########################################
// ########################################################

// ########################################################
// Instance Variables #####################################
// ########################################################

};

} /* namespace widget */

#endif /* SRC_GUI_WIDGET_BUTTON_H_ */
