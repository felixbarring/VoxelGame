
#ifndef SRC_GRAPHICS_TEXTUREDMESH_H_
#define SRC_GRAPHICS_TEXTUREDMESH_H_

#include <GL/glew.h>
#include <vector>
#include "iMesh.h"

class MeshVNT: public virtual IMesh {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	MeshVNT(std::vector<float> &vertices, std::vector<float> &normals, std::vector<float> textUV, std::vector<short> &elementData);
	virtual ~MeshVNT();

// ########################################################
// Member Functions########################################
// ########################################################

	void render() override;

// ########################################################
// Instance Variables #####################################
// ########################################################
private:
	GLuint VAO;
	GLuint VBO_V;
	GLuint VBO_N;
	GLuint VBO_UV;
	GLuint EBO;
	int numberOfElements;
};

#endif /* SRC_GRAPHICS_TEXTUREDMESH_H_ */
