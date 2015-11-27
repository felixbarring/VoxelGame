
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


float AABB::collisionTime(AABB &box1, AABB &box2, glm::vec3 &collisionNormal, glm::vec3 &velocity)
{

	float xInvEntry, yInvEntry, zInvEntry;
	float xInvExit, yInvExit, zInvExit;

	if (velocity.x > 0.0f) {
		xInvEntry = box2.xMin - box1.xMax;
		xInvExit = box2.xMax - box1.xMin;
	} else {
		xInvEntry = box2.xMax - box1.xMin;
		xInvExit = box2.xMin - box1.xMax;
	}

	if (velocity.y > 0.0f) {
		yInvEntry = box2.yMin - box1.yMax;
		yInvExit = box2.yMax - box1.yMin;
	} else {
		yInvEntry = box2.yMax - box1.yMin;
		yInvExit = box2.yMin - box1.yMax;
	}

	if (velocity.z > 0.0f) {
		zInvEntry = box2.zMin - box1.zMax;
		zInvExit = box2.zMax - box1.zMin;
	} else {
		zInvEntry = box2.zMax - box1.zMin;
		zInvExit = box2.zMin - box1.zMax;
	}

	float xEntry, yEntry, zEntry;
	float xExit, yExit, zExit;

	if (velocity.x == 0) {
		xEntry = -std::numeric_limits<float>::infinity();
		xExit = std::numeric_limits<float>::infinity();
	} else {
		xEntry = xInvEntry / velocity.x;
		xExit = xInvExit / velocity.x;
	}

	if (velocity.y == 0) {
		yEntry = -std::numeric_limits<float>::infinity();
		yExit = std::numeric_limits<float>::infinity();
	} else {
		yEntry = yInvEntry / velocity.y;
		yExit = yInvExit / velocity.y;
	}

	if (velocity.z == 0) {
		zEntry = -std::numeric_limits<float>::infinity();
		zExit = std::numeric_limits<float>::infinity();
	} else {
		zEntry = zInvEntry / velocity.z;
		zExit = zInvExit / velocity.z;
	}

	// TODO Correct?
	if (xEntry > 1.0f){ xEntry = -1000000; }
	if (yEntry > 1.0f){ yEntry = -1000000; }
	if (zEntry > 1.0f){ zEntry = -1000000; }

	float entryTime = std::max(xEntry, std::max(yEntry, zEntry));
	float exitTime = std::min(xExit, std::min(yExit, zExit));

	std::cout << "xEntry: " << xEntry << "\n";
	std::cout << "yEntry: " << yEntry << "\n";
	std::cout << "zEntry: " << zEntry << "\n";

	std::cout << "Entry Time: " << entryTime << "\n";
	std::cout << "Exit Time: " << exitTime << "\n";

	// No collision
	if(entryTime > exitTime || (xEntry < 0.0f && yEntry < 0.0f && zEntry < 0.0f)){
		collisionNormal = glm::vec3(0.0f, 0.0f, 0.0f);
		return 1.1f;
	}

	// Collision!
	if(xEntry > yEntry && xEntry > zEntry){
		if(xInvEntry < 0.0f){
			collisionNormal = glm::vec3(1.0f, 0.0f, 0.0f);
		} else {
			collisionNormal = glm::vec3(-1.0f, 0.0f, 0.0f);
		}
	} else if (yEntry > xEntry && yEntry > zEntry) {
		if(yInvEntry < 0.0f){
			collisionNormal = glm::vec3(0.0f, 1.0f, 0.0f);

		} else {
			collisionNormal = glm::vec3(0.0f, -1.0f, 0.0f);
		}
	} else {
		if(zInvEntry < 0.0f){
			collisionNormal = glm::vec3(0.0f, 0.0f, 1.0f);
		} else {
			collisionNormal = glm::vec3(0.0f, 0.0f, -1.0f);
		}
	}

	return entryTime;

}

}
