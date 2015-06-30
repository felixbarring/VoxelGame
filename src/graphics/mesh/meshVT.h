
#ifndef GAME_MODEL_MESH_H_
#define GAME_MODEL_MESH_H_

#include <GL/glew.h>
#include <vector>
#include "iMesh.h"

class MeshVT: public virtual IMesh {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	MeshVT(std::vector<float> &vertices,  int vFormat, std::vector<float> textUV, int tFormat, std::vector<short> &elementData);
	virtual ~MeshVT();

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
	GLuint VBO_Texture;
	int textureFormat;
	GLuint EBO;
	int numberOfElements;
};

#endif /* GAME_MODEL_MESH_H_ */
