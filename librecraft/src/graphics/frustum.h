
#ifndef SRC_GRAPHICS_FRUSTUM_H_
#define SRC_GRAPHICS_FRUSTUM_H_

#include <glm/glm.hpp>

/**
 * A frustum that can be used to determine if an object is inside its border's.
 *
 * See Real-Time Rendering Third Edition page 774 for explanation on how the plane magic is implemented
 *
 *	Plane equation is: a(x - x0) + b(y - y0) + c(z - z0) = 0
 *	ax + by + cz + d = 0
 *	where d = -(ax0 + cy0 + cz0)
 *
 *	Distance d from a point p(x1, y1, z1) to a plane is
 *	d = |ax1 + by1 + cz1 + d| / sqrt(a^2 + b^2 + c^2)
 *
 *	The division with sqrt(a^2 + b^2 + c^2) can be avoided by normalizing a b and c
 *
 */
class Frustum {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	Frustum(glm::mat4 mvp);

	virtual ~Frustum() {};

// ########################################################
// Member Functions########################################
// ########################################################

	bool sphereInFrustum(glm::vec3 center, float radius);

// ########################################################
// Implementation #########################################
// ########################################################

private:

	struct Plane {
		glm::vec3 m_normal;
	};



	std::vector<int> rightPlane{};
	std::vector<int> leftPlane{};
	std::vector<int> topPlane{};
	std::vector<int> bottomPlane{};
	std::vector<int> nearPlane{};
	std::vector<int> fatPlane{};

};

#endif /* SRC_GRAPHICS_FRUSTUM_H_ */
