#ifndef SRC_MODEL_WORLD_ENTITY_AABB_H_
#define SRC_MODEL_WORLD_ENTITY_AABB_H_

#include <glm/glm.hpp>

namespace entity {

class AABB {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

    /**
     *
     * @param xMinimum
     * @param xMaximum
     * @param yMinimum
     * @param yMaximum
     * @param zMinimum
     * @param zMaximum
     */
	AABB(double xMinimum, double xMaximum, double yMinimum, double yMaximum, double zMinimum, double zMaximum);

// ########################################################
// Member Functions########################################
// ########################################################

	/**
	 *
	 * @param xMinimum
	 * @param xMaximum
	 * @param yMinimum
	 * @param yMaximum
	 * @param zMinimum
	 * @param zMaximum
	 */
	void setBounds(double xMinimum, double xMaximum, double yMinimum, double yMaximum, double zMinimum,
	        double zMaximum);

	/**
	 *
	 * @param that
	 * @return
	 */
	bool intersects(const AABB &that);

	/**
	 *
	 * @param box
	 * @param velocity
	 * @return
	 */
	static AABB getSweptBroadPhaseBox(AABB &box, glm::vec3 &velocity);

	/**
	 *
	 * @param box1
	 * @param box2
	 * @param collisionNormal
	 * @param velocity
	 * @return Entry time. [0, 1) is considered a collision.
	 */
	static float collisionTime(AABB &box1, AABB &box2, glm::vec3 &collisionNormal, glm::vec3 &velocity);

// ########################################################
// Implementation #########################################
// ########################################################

	double xMin, xMax, yMin, yMax, zMin, zMax;

};

} /* namespace entity */

#endif /* SRC_MODEL_WORLD_ENTITY_AABB_H_ */
