
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

	MeshVT(std::vector<float> &vertices, std::vector<float> textUV, std::vector<short> &elementData);
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
	GLuint VBO_V;
	GLuint VBO_UV;
	GLuint EBO;
	int numberOfElements;
};

#endif /* GAME_MODEL_MESH_H_ */
