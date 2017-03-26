#ifndef SRC_GUI_WIDGET_ABSTRACTWIDGET_H_
#define SRC_GUI_WIDGET_ABSTRACTWIDGET_H_

#include "iWidget.h"

#include <memory>
#include <iostream>

#include "../../graphics/sprite.h"
#include "../../graphics/spriteBatcher.h"

namespace widget {

/**
 * \brief Skeletal implementation of iWidget.h
 *
 * Implements the fundamental functionality that all widgets should share.
 * These includes checking if a point is inside this widget, setting the
 * location of the widget and also setting the isActive state.
 *
 * The x and y location is the left bottom corner of the widget.
 *
 */
class AbstractWidget: public IWidget {
public:

	/**
	 * \brief Constructs a AbstractWidget.
	 *
	 * \param id The id of the button, will be used when notifying
	 *           the listener that this button was pressed
	 *
	 * \param text A String that will rendered ontop of the button
	 *                 providing information of what the purpose of this button is
	 *
	 * \param listener The listener that implements the IButtonListener
	 *                     interface. When the button is pressed the listener will
	 *                     be notified.
	 *
	 * \param x The x location of the bottom left corner
	 * \param y The y location of the bottom left corner
	 * \param width The width, that is, the horizontal distance between the
	 *                  left and right corners
	 *
	 * \param height The height, that is, the vertical distance between the
	 *                   bottom and top corners
	 */
	AbstractWidget(int id, int x, int y, int width, int height);

	virtual ~AbstractWidget() = default;

	int getId() override;

	void update(float timePassed) override;

	void setLocation(float x, float y) override;

	void setActive() override;

	void setInactive() override;

	bool isActive() override;

protected:

	/**
	 *  \brief Determines if the point is inside this widgets borders
	 *
	 *	Detailed description here
	 *
	 *  \param x The x coordinate of the point
	 *  \param y The y coordinate of the point
	 *  \return True if the point is inside the borders, else false
	 */
	bool isInsideBorders(double x, double y);

	bool m_active {true};
	const int m_id;
	int m_xCoordinate, m_yCoordinate;
	const int m_width, m_height;
};

} /* namespace widget */

#endif /* SRC_GUI_WIDGET_ABSTRACTWIDGET_H_ */
