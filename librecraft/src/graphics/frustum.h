
#ifndef SRC_GRAPHICS_FRUSTUM_H_
#define SRC_GRAPHICS_FRUSTUM_H_

#include <glm/glm.hpp>
#include <vector>

using glm::sqrt;

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

    Frustum(glm::mat4 mvp) {
        // See http://web.archive.org/web/20120531231005/http://crazyjoke.free.fr/doc/3D/plane%20extraction.pdf
        // and https://github.com/danhedron/openrw/blob/master/rwengine/src/render/ViewFrustum.hpp

        mvp = glm::transpose(mvp);
        for (unsigned i = 0; i < m_planes.size(); ++i) {
            float sign = (i % 2 == 0) ? 1.f : -1.f;
            int r = i / 2;
            m_planes[i].x = mvp[0][3] + mvp[0][r] * sign;
            m_planes[i].y = mvp[1][3] + mvp[1][r] * sign;
            m_planes[i].z = mvp[2][3] + mvp[2][r] * sign;
            m_planes[i].w = mvp[3][3] + mvp[3][r] * sign;

            auto l = glm::length(glm::vec3(m_planes[i].x, m_planes[i].y, m_planes[i].z));
            m_planes[i] /= l;
            m_planes[i].w /= l;
        }
    }

    virtual ~Frustum() {};

    bool isSphereInFrustum(glm::vec3 center, float radius) {
        for (size_t i = 0; i < 6; ++i) {
            auto poot = m_planes[i];
            auto d = glm::dot(glm::vec3(poot.x, poot.y, poot.z), center) + m_planes[i].w;
            if (d < -radius)
                return false;
        }
        return true;
    }

    bool isCubeInFrustum(float x, float y, float z, int xSize, int ySize, int zSize ) {

        for(int p = 0; p < 6; p++ ) {

            glm::vec4 plane = m_planes[p];
            glm::vec3 plane3 = glm::vec3(plane.x, plane.y, plane.z);

            if(glm::dot(plane3, glm::vec3(x - xSize, y - ySize, z - zSize)) + plane[3] > 0 )
             continue;
            if(glm::dot(plane3, glm::vec3(x + xSize, y - ySize, z - zSize)) + plane[3] > 0 )
             continue;
            if(glm::dot(plane3, glm::vec3(x - xSize, y + ySize, z - zSize)) + plane[3] > 0 )
             continue;
            if(glm::dot(plane3, glm::vec3(x + xSize, y + ySize, z - zSize)) + plane[3] > 0 )
             continue;
            if(glm::dot(plane3, glm::vec3(x - xSize, y - ySize, z + zSize)) + plane[3] > 0 )
             continue;
            if(glm::dot(plane3, glm::vec3(x + xSize, y - ySize, z + zSize)) + plane[3] > 0 )
             continue;
            if(glm::dot(plane3, glm::vec3(x - xSize, y + ySize, z + zSize)) + plane[3] > 0 )
             continue;
            if(glm::dot(plane3, glm::vec3(x + xSize, y + ySize, z + zSize)) + plane[3] > 0 )
             continue;

            return false;
        }
    return true;

    /*
    for(int p = 0; p < 6; p++ ) {
        if( m_planes[p][0] * (x - xSize) + m_planes[p][1] * (y - ySize) + m_planes[p][2] * (z - zSize) + m_planes[p][3] > 0 )
         continue;
        if( m_planes[p][0] * (x + xSize) + m_planes[p][1] * (y - ySize) + m_planes[p][2] * (z - zSize) + m_planes[p][3] > 0 )
         continue;
        if( m_planes[p][0] * (x - xSize) + m_planes[p][1] * (y + ySize) + m_planes[p][2] * (z - zSize) + m_planes[p][3] > 0 )
         continue;
        if( m_planes[p][0] * (x + xSize) + m_planes[p][1] * (y + ySize) + m_planes[p][2] * (z - zSize) + m_planes[p][3] > 0 )
         continue;
        if( m_planes[p][0] * (x - xSize) + m_planes[p][1] * (y - ySize) + m_planes[p][2] * (z + zSize) + m_planes[p][3] > 0 )
         continue;
        if( m_planes[p][0] * (x + xSize) + m_planes[p][1] * (y - ySize) + m_planes[p][2] * (z + zSize) + m_planes[p][3] > 0 )
         continue;
        if( m_planes[p][0] * (x - xSize) + m_planes[p][1] * (y + ySize) + m_planes[p][2] * (z + zSize) + m_planes[p][3] > 0 )
         continue;
        if( m_planes[p][0] * (x + xSize) + m_planes[p][1] * (y + ySize) + m_planes[p][2] * (z + zSize) + m_planes[p][3] > 0 )
         continue;

        return false;
    }
    return true;
    */
}


private:

    std::vector<glm::vec4> m_planes{glm::vec4{}, glm::vec4{}, glm::vec4{}, glm::vec4{}, glm::vec4{}, glm::vec4{}};

};

#endif /* SRC_GRAPHICS_FRUSTUM_H_ */
