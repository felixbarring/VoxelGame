#include "abstractWidget.h"

#include <iostream>

namespace widget {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

AbstractWidget::AbstractWidget(int id, int x, int y, int width, int height)
		: m_id{id}, m_xCoordinate{x}, m_yCoordinate{y}, m_width{width}, m_height{height} {
}

// ########################################################
// Member Functions########################################
// ########################################################

int AbstractWidget::getId() {
	return m_id;
}

void AbstractWidget::update(float timePassed) {
	// Do nothing, Subclasses can override this function if they need update functionality.
}

bool AbstractWidget::isActive() {
	return m_active;
}

void AbstractWidget::setActive() {
	m_active = true;
}

void AbstractWidget::setInactive() {
	m_active = false;
}

void AbstractWidget::setLocation(float x, float y, float z) {
	this->m_xCoordinate = x;
	this->m_yCoordinate = y;
}

bool AbstractWidget::isInsideBorders(double x, double y) {
	return (x > this->m_xCoordinate && x < this->m_xCoordinate + m_width
			&& y > this->m_yCoordinate && y < this->m_yCoordinate + m_height);
}

} /* namespace widget */
