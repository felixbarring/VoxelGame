
#include "abstractWidget.h"

namespace widget {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

AbstractWidget::AbstractWidget()
{
}

AbstractWidget::~AbstractWidget()
{
}

// ########################################################
// Member Functions########################################
// ########################################################

bool AbstractWidget::isActive()
{
	return true;
}

bool AbstractWidget::isInsideBorders(float x, float y)
{
	return true;
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
	//
}

} /* namespace widget */
