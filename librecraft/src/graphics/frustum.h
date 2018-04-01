
#ifndef SRC_GRAPHICS_FRUSTUM_H_
#define SRC_GRAPHICS_FRUSTUM_H_

#include <glm/glm.hpp>
#include <vector>

using glm::sqrt;

/**
 * A frustum that can be used to determine if an object is inside its border's.
 *
 * See Real-Time Rendering Third Edition page 774 for explanation on how the
 * plane magic is implemented
 *
 *	Plane equation is: a(x - x0) + b(y - y0) + c(z - z0) = 0
 *	ax + by + cz + d = 0
 *	where d = -(ax0 + cy0 + cz0)
 *
 *	Distance d from a point p(x1, y1, z1) to a plane is
 *	d = |ax1 + by1 + cz1 + d| / sqrt(a^2 + b^2 + c^2)
 *
 *	The division with sqrt(a^2 + b^2 + c^2) can be avoided by normalizing a
 *  b and c
 *
 *  The implementation is inspired by this
 *  https://github.com/danhedron/openrw/blob/master/rwengine/src/render/ViewFrustum.hpp
 *
 */

namespace graphics {

/**
 * @brief A frustum that can be used to determine intersection with basic
 *        volumes
 */
class Frustum {
public:
  /**
   * @brief Should be called to update the frustum bounds.
   *
   * All calls to the intersect functions will compared to the frustum
   * representation of the provided matrix after this call.
   *
   * @param matrix The matrix that will be used to create the frustum planes.
   */
  void update(const glm::mat4& matrix) {
    for (size_t i{0}; i < m_planes.size(); ++i) {
      float sign{(i % 2 == 0) ? 1.0f : -1.0f};
      int r{i / 2};
      m_planes[i].normal.x = matrix[0][3] + matrix[0][r] * sign;
      m_planes[i].normal.y = matrix[1][3] + matrix[1][r] * sign;
      m_planes[i].normal.z = matrix[2][3] + matrix[2][r] * sign;
      m_planes[i].distance = matrix[3][3] + matrix[3][r] * sign;

      float length{glm::length(m_planes[i].normal)};
      m_planes[i].normal /= length;
      m_planes[i].distance /= length;
    }
  }

  /**
   * @brief Check if a sphere intersects with the frustumm
   *
   * @param center The center of the sphere.
   * @param radius The radius of the sphere.
   * @return Whether the sphere intersects with the frustum or not.
   */
  bool intersects(glm::vec3 center, float radius) const {
    for (const Plane& plane : m_planes) {
      if (glm::dot(plane.normal, center) < (-radius - plane.distance))
        return false;
    }
    return true;
  }

  /**
   * @brief Check if Axis Aligned Bounding Box intersects with the frustum.
   *
   * Only the min and max coordinates are necessary, The implementation will
   * test against the other 6 points of the box.
   *
   * @param min All the lowest x, y, and z coordinates.
   * @param max All the highest x, y, and z coordinates.
   * @return Whether the AABB intersects with the frustum or not.
   */
  bool intersects(glm::vec3 min, glm::vec3 max) {
    return intersects(min) || intersects(glm::vec3{max.x, min.y, min.z}) ||
           intersects(glm::vec3{max.x, max.y, min.z}) ||
           intersects(glm::vec3{min.x, max.y, min.z}) ||

           intersects(glm::vec3{min.x, min.y, max.z}) ||
           intersects(glm::vec3{max.x, min.y, max.z}) || intersects(max) ||
           intersects(glm::vec3{min.x, max.y, max.z});
  }

  /**
   * @brief Check if the point intersects with the frustum.
   *
   * @param point The point to check.
   * @return Whether the point intersects with frustum or not.
   */
  inline bool intersects(glm::vec3 point) {
    for (const Plane& plane : m_planes) {
      if (glm::dot(plane.normal, point) < -plane.distance)
        return false;
    }
    return true;
  }

private:
  class Plane {
  public:
    glm::vec3 normal{};
    float distance{};
  };

  std::vector<Plane> m_planes{{}, {}, {}, {}, {}, {}};
};
}

#endif /* SRC_GRAPHICS_FRUSTUM_H_ */
