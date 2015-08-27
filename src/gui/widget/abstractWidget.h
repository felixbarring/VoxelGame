
/**
 *  \file [abstractWidget.h]
 *  \brief An abstract class that all Widgets should inherit
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
 */
class AbstractWidget : public IWidget{
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	AbstractWidget();

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
	graphics::Sprite sprite;


};


} /* namespace widget */

#endif /* SRC_GUI_WIDGET_ABSTRACTWIDGET_H_ */
