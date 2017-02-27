#include "aabb.h"

#include <iostream>
#include <algorithm>

namespace entity {

AABB::AABB(double xMinimum, double xMaximum, double yMinimum, double yMaximum, double zMinimum, double zMaximum)
    : xMin {xMinimum}, xMax {xMaximum}, yMin {yMinimum}, yMax {yMaximum}, zMin {
            zMinimum}, zMax {zMaximum} {
}

void AABB::setBounds(double xMinimum, double xMaximum, double yMinimum, double yMaximum, double zMinimum,
        double zMaximum) {

    xMin = xMinimum;
    xMax = xMaximum;
    yMin = yMinimum;
    yMax = yMaximum;
    zMin = zMinimum;
    zMax = zMaximum;

}

bool AABB::intersects(const AABB &that) {
    return ((this->xMin >= that.xMin && this->xMin <= that.xMax) ||
            (this->xMax >= that.xMin && this->xMax <= that.xMax)) &&
           ((this->yMin >= that.yMin && this->yMin <= that.yMax) ||
            (this->yMax >= that.yMin && this->yMax <= that.yMax)) &&
           ((this->zMin >= that.zMin && this->zMin <= that.zMax) ||
            (this->zMax >= that.zMin && this->zMax <= that.zMax));
}

AABB AABB::getSweptBroadPhaseBox(AABB &box, glm::vec3 &velocity) {
    float xMin, xMax, yMin, yMax, zMin, zMax;

    if (velocity.x > 0.0f) {
        xMin = box.xMin;
        xMax = box.xMax + velocity.x;
    } else {
        xMin = box.xMin + velocity.x;
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

float AABB::collisionTime(AABB &box1, AABB &box2, glm::vec3 &collisionNormal, glm::vec3 &velocity) {

    float xEntryDistance, yEntryDistance, zEntryDistance;
    float xExitDistance, yExitDistance, zExitDistance;

    if (velocity.x > 0.0f) {
        xEntryDistance = box2.xMin - box1.xMax;
        xExitDistance = box2.xMax - box1.xMin;
    } else {
        xEntryDistance = box2.xMax - box1.xMin;
        xExitDistance = box2.xMin - box1.xMax;
    }

    if (velocity.y > 0.0f) {
        yEntryDistance = box2.yMin - box1.yMax;
        yExitDistance = box2.yMax - box1.yMin;
    } else {
        yEntryDistance = box2.yMax - box1.yMin;
        yExitDistance = box2.yMin - box1.yMax;
    }

    if (velocity.z > 0.0f) {
        zEntryDistance = box2.zMin - box1.zMax;
        zExitDistance = box2.zMax - box1.zMin;
    } else {
        zEntryDistance = box2.zMax - box1.zMin;
        zExitDistance = box2.zMin - box1.zMax;
    }

    float xEntryTime, yEntryTime, zEntryTime;
    float xExitTime, yExitTime, zExitTime;

    if (velocity.x == 0) {
        xEntryTime = -std::numeric_limits<float>::infinity();
        xExitTime = std::numeric_limits<float>::infinity();
    } else {
        xEntryTime = xEntryDistance / velocity.x;
        xExitTime = xExitDistance / velocity.x;
    }

    if (velocity.y == 0) {
        yEntryTime = -std::numeric_limits<float>::infinity();
        yExitTime = std::numeric_limits<float>::infinity();
    } else {
        yEntryTime = yEntryDistance / velocity.y;
        yExitTime = yExitDistance / velocity.y;
    }

    if (velocity.z == 0) {
        zEntryTime = -std::numeric_limits<float>::infinity();
        zExitTime = std::numeric_limits<float>::infinity();
    } else {
        zEntryTime = zEntryDistance / velocity.z;
        zExitTime = zExitDistance / velocity.z;
    }

    // TODO Correct?
    if (xEntryTime > 1.0f)
        xEntryTime = -1000000;
    if (yEntryTime > 1.0f)
        yEntryTime = -1000000;
    if (zEntryTime > 1.0f)
        zEntryTime = -1000000;

    float entryTime = std::max(xEntryTime, std::max(yEntryTime, zEntryTime));
    float exitTime = std::min(xExitTime, std::min(yExitTime, zExitTime));

    // No collision
    if (entryTime > exitTime || (xEntryTime < 0.0f && yEntryTime < 0.0f && zEntryTime < 0.0f))
        return 1.0f;

    // Collision!
    if (xEntryTime > yEntryTime && xEntryTime > zEntryTime) {
        collisionNormal = glm::vec3(1.0f, 0.0f, 0.0f);
    } else if (yEntryTime > xEntryTime && yEntryTime > zEntryTime) {
        collisionNormal = glm::vec3(0.0f, 1.0f, 0.0f);
    } else {
        collisionNormal = glm::vec3(0.0f, 0.0f, 1.0f);
    }

    return entryTime;
}

}
