
#include "abstractWidget.h"

namespace widget {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

AbstractWidget::AbstractWidget(int id, int x, int y, int width, int height) :
	id{id},
	x{x},
	y{y},
	width{width},
	height{height}
{
	std::cout << "Abstract widget: " << id << "\n";
}

// ########################################################
// Member Functions########################################
// ########################################################

bool AbstractWidget::isActive()
{
	return active;
}

bool AbstractWidget::isInsideBorders(float x, float y)
{
	return (x > this->x && x < this->x + width &&
			y > this->y && y < this->y + height);
}

void AbstractWidget::setActive()
{
	active = true;
}

void AbstractWidget::setInactive()
{
	active = false;
}

void AbstractWidget::setLocation(float x, float y, float z)
{
	this->x = x;
	this->y = y;
}

} /* namespace widget */
