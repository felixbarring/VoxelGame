
/**
 *  \file [iWidget.h]
 *  \brief An Interface for Widgets
 *  \author Felix Bärring
 *
 *  Copyright (c) 2015, Felix Bärring. All rights reserved.
 */

#ifndef SRC_GUI_WIDGET_IWIDGET_H_
#define SRC_GUI_WIDGET_IWIDGET_H_

namespace widget
{

/**
 *  \class IWidget
 *
 *  Detailed description here
 *
 *  \author Felix Bärring
 */
class IWidget {
public:

	virtual ~IWidget();

	/**
	 *  \brief Makes the Widget draw itself
	 */
	virtual void draw() = 0;

	/**
	 * Notifies that the mouse was pressed and that the widget can act on this
	 * depending on the what button was pressed and where the mouse pointer is at.
	 *
	 * \param[in] button The mouse button that was pressed
	 * \param[in] x The x coordinate of the mouse when the button was pressed
	 * \param[in] y The y coordinate of the mouse when the button was pressed
	 */
	virtual void mouseClicked(int button, float x, float y) = 0;

	/**
	 *  Notifies that the mouse was moved and where the new position is.
	 *  Is particular useful for widgets that hints that they are interactable when
	 *  the mouse pointer is inside its borders.
	 *
	 * \param[in] x The new x location of the mouse pointer
	 * \param[in] The new y location of the mouse pointer
	 */
	virtual void mouseMoved(float x, float y) = 0;

	/**
	 * Communicate that a certain key on the keyboard was pressed.
	 *
	 * \param[in] key The int value of the key
	 */
	virtual void keyPressed(int key, char c) = 0;

	/**
	 * Set the position of the widget, relative to the container it's inside.
	 *
	 * \param[in] x The new horizontal location
	 * \param[in] y The new vertical location
	 */
	virtual void setLocation(int x, int y) = 0;

	/**
	 * Sets the Widget to an active state.
	 * If it is not in an active state all other methods will be ignored.
	 */
	virtual void setActive() = 0;

	/**
	 * Sets the widget to inactive state.
	 * All other methods will be ignored except for setActive.
	 */
	virtual void deactivate() = 0;

	/**
	 * Checks if the widget is active or not
	 *
	 * \return Whether the Widget is active or not
	 */
	virtual bool isActive() = 0;

};

}

#endif /* SRC_GUI_WIDGET_IWIDGET_H_ */