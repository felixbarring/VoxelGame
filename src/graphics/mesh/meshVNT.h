
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

	MeshVNT(const std::vector<float>& vertices, int vFormat, const std::vector<float>& normals,
			int nFormat, const std::vector<float>& textUV, int tFormat, std::vector<short> &elementData);
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
	GLuint VBO_Vertices;
	int verticesFormat;
	GLuint VBO_Normals;
	int normalsFormat;
	GLuint VBO_Texture;
	int textureFormat;
	GLuint EBO;
	int numberOfElements;
};

#endif /* SRC_GRAPHICS_TEXTUREDMESH_H_ */
