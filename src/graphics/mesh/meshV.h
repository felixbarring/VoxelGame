
#ifndef SRC_GRAPHICS_MESH_MESHV_H_
#define SRC_GRAPHICS_MESH_MESHV_H_

#include <GL/glew.h>
#include <vector>

#include "iMesh.h"

class MeshV : public virtual IMesh{
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	MeshV(const std::vector<float> &vertices,  int vFormat, const std::vector<short> &elementData);
	virtual ~MeshV();

// ########################################################
// Member Functions########################################
// ########################################################

	void render() override;

// ########################################################
// Instance Variables #####################################
// ########################################################

private:
	GLuint VAO;
	GLuint VBO_Vertices;
	int verticesFormat;
	GLuint EBO;
	int numberOfElements;
};

#endif /* SRC_GRAPHICS_MESH_MESHV_H_ */
