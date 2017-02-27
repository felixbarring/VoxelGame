#ifndef SRC_GRAPHICS_MESH_MESHELEMENT_H_
#define SRC_GRAPHICS_MESH_MESHELEMENT_H_

#include "iMesh.h"

#include <GL/glew.h>
#include <vector>

namespace mesh {

class MeshElement: public virtual IMesh {
public:

	MeshElement(
        const std::vector<float> &vbd1, int vbd1Format,
        const std::vector<short> &elementData);

	MeshElement(
        const std::vector<float> &vbd1, int vbd1Format,
        const std::vector<float> &vbd2, int vbd2Format,
        const std::vector<short> &elementData);

	MeshElement(
        const std::vector<float> &vbd1, int vbd1Format,
        const std::vector<float> &vbd2, int vbd2Format,
        const std::vector<float> &vbd3, int vbd3Format,
        const std::vector<short> &elementData);

	MeshElement(
        const std::vector<float> &vbd1, int vbd1Format,
        const std::vector<float> &vbd2, int vbd2Format,
        const std::vector<float> &vbd3, int vbd3Format,
        const std::vector<float> &vbd4, int vbd4Format,
        const std::vector<short> &elementData);

	virtual ~MeshElement();

    void draw() override;

private:

    GLuint m_VAO;
    GLuint m_EBO;
    std::vector<GLuint> m_vboHandels;
    int m_numberOfElements;

};

} /* namespace mesh */

#endif /* SRC_GRAPHICS_MESH_MESHELEMENT_H_ */
