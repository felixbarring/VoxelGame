
#ifndef SRC_GRAPHICS_FRUSTUM_H_
#define SRC_GRAPHICS_FRUSTUM_H_

#include <glm/glm.hpp>

/**
 * A frustum that can be used to determine if an object is inside its border's.
 * Based on the libgdx implementation
 * 	see: https://github.com/libgdx/libgdx/blob/master/gdx/src/com/badlogic/gdx/math/Frustum.java
 */
class Frustum {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	Frustum();

	virtual ~Frustum();

// ########################################################
// Member Functions########################################
// ########################################################

	void update(glm::mat4 inverseProjectionView);

	bool pointInFrustum(glm::vec3 point);

	bool sphereInFrustum(glm::vec3 center, float radius);



};

#endif /* SRC_GRAPHICS_FRUSTUM_H_ */
