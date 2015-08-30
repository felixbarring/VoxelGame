
/**
 *  \file [abstractWidget.h]
 *  \author Felix Bärring
 *
 *  Copyright (c) 2015, Felix Bärring. All rights reserved.
 */

#ifndef SRC_GUI_WIDGET_ABSTRACTWIDGET_H_
#define SRC_GUI_WIDGET_ABSTRACTWIDGET_H_

#include "iWidget.h"

#include <memory>

#include "../../graphics/sprite.h"
#include "../../graphics/spriteBatcher.h"

namespace widget {

/**
 * \class AbstractWidget
 * \brief Do this need brief?
 *
 * Implements the fundamental functionality that all widgets should share.
 * These includes checking if a point is inside this widget, setting the
 * location of the widget and also setting the isActive state.
 *
 * The x and y location is the left bottom corner of the widget.
 *
 * \author Felix Bärring
 */
class AbstractWidget : public IWidget{
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	/**
	 * \brief Constructs a AbstractWidget.
	 *
	 * \param[in] id The id of the button, will be used when notifying
	 *           the listener that this button was pressed
	 *
	 * \param[in] text A String that will rendered ontop of the button
	 *             providing information of what the purpose of this button is
	 *
	 * \param[in] listener The listener that implements the IButtonListener interface.
	 *                 When the button is pressed the listener will be notified.
	 *
	 * \param[in] x The x location of the bottom left corner
	 * \param[in] y The y location of the bottom left corner
	 * \param[in] width The width, that is, the horizontal distance between the
	 *              left and right corners
	 *
	 * \param[in] height The height, that is, the vertical distance between the
	 *               bottom and top corners
	 */
	AbstractWidget(int id, int x, int y, int width, int height);

	virtual ~AbstractWidget();

// ########################################################
// Member Functions########################################
// ########################################################

	/**
	 *  \brief Determines if the point is inside this widgets borders
	 *
	 *	Detailed description here
	 *
	 *  \param[in] x The x coordinate of the point
	 *  \param[in] y The y coordinate of the point
	 *  \return True if the point is inside the borders, else false
	 */
	bool isInsideBorders(float x, float y);

	/**
	 *  \brief Sets the location of this widget
	 *
	 *  Detailed description here
	 *
	 *  \param[in] x The new x coordinate of this widget
	 *  \param[in] y The new y coordinate of this widget
	 *  \param[in] z The new z coordinate of this widget
	 */
	void setLocation(float x, float y, float z);

	/**
	 *  \brief Sets the Widgets state to active
	 *
	 *  If the widget is not active, no functions, except for setActive, setInactive and isActive, will work.
	 *
	 */
	void setActive();

	/**
	 *  \brief Sets the Widgets stat to inactive
	 *
	 *  If the widget is not active, no functions, except for setActive, setInactive and isActive, will work.
	 *
	 */
	void setInactive();

	/**
	 *  \brief Check if the Widget is active or not
	 *
	 *  \return True if the Widget is in an active state, else false
	 */
	bool isActive();

// ########################################################
// Instance Variables #####################################
// ########################################################

private:

	bool active = true;
	//graphics::Sprite sprite;

	const int id;
	const int x, y;
	const int width, height;

};


} /* namespace widget */

#endif /* SRC_GUI_WIDGET_ABSTRACTWIDGET_H_ */
