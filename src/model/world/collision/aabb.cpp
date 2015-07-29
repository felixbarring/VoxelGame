
#include "aabb.h"

namespace collision
{

// ########################################################
// Constructor/Destructor #################################
// ########################################################

AABB::AABB(double xMinimum, double xMaximum, double yMinimum, double yMaximum, double zMinimum, double zMaximum) :
	xMin{xMinimum},
	xMax{xMaximum},
	yMin{yMinimum},
	yMax{yMaximum},
	zMin{zMinimum},
	zMax{zMaximum}
{

}

AABB::~AABB()
{
}

// ########################################################
// Member Functions########################################
// ########################################################

bool AABB::intersects(const AABB &that)
{
	return
		((this->xMin < that.xMin && this->xMax > that.xMin) || (this->xMin < that.xMax && this->xMax > that.xMax )) &&
		((this->yMin < that.yMin && this->yMax > that.yMin) || (this->yMin < that.yMax && this->yMax > that.yMax )) &&
		((this->zMin < that.zMin && this->zMax > that.zMin) || (this->zMin < that.zMax && this->zMax > that.zMax ));
}

}
