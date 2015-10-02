
#include "aabb.h"

#include <iostream>
#include <algorithm>

namespace entity
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

void AABB::setBounds(double xMinimum, double xMaximum, double yMinimum, double yMaximum, double zMinimum, double zMaximum)
{

	xMin = xMinimum;
	xMax = xMaximum;
	yMin = yMinimum;
	yMax = yMaximum;
	zMin = zMinimum;
	zMax = zMaximum;

}

bool AABB::intersects(const AABB &that)
{
	return
		((this->xMin <= that.xMin && this->xMax >= that.xMin) || (this->xMin <= that.xMax && this->xMax >= that.xMax )) &&
		((this->yMin <= that.yMin && this->yMax >= that.yMin) || (this->yMin <= that.yMax && this->yMax >= that.yMax )) &&
		((this->zMin <= that.zMin && this->zMax >= that.zMin) || (this->zMin <= that.zMax && this->zMax >= that.zMax ));
}


AABB AABB::getSweptBroadPhaseBox(AABB &box, glm::vec3 &velocity){
    float xMin, xMax, yMin, yMax, zMin, zMax;

    if (velocity.x > 0.0f) {
    	xMin = box.xMin;
    	xMax = box.xMax + velocity.x;
    } else {
    	xMin =box.xMin + velocity.x;
    	xMax = box.xMax;
    }

    if (velocity.y > 0.0f) {
    	yMin = box.yMin;
    	yMax = box.yMax + velocity.y;
    } else {
    	yMin = box.yMin + velocity.y;
    	yMax = box.yMax;
    }

    if (velocity.z > 0.0f) {
    	zMin = box.zMin;
    	zMax = box.zMax + velocity.z;
    } else {
    	zMin = box.zMin + velocity.z;
    	zMax = box.zMax;
    }

    return AABB{xMin, xMax, yMin, yMax, zMin, zMax};
}

}
