
#include "meshVNT.h"

#include <GL/glew.h>
#include <vector>

namespace mesh
{

// ########################################################
// Constructor/Destructor #################################
// ########################################################

MeshVNT::MeshVNT(const std::vector<float>& vertices, int vFormat, const std::vector<float>& normals,
			int nFormat, const std::vector<float>& textUV, int tFormat, std::vector<short> &elementData) :
			verticesFormat{vFormat},
			normalsFormat{nFormat},
			textureFormat{tFormat}
{

	numberOfElements = elementData.size();

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	const int FLOAT_SIZE = (sizeof(float));

	glGenBuffers(1, &VBO_Vertices);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Vertices);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*FLOAT_SIZE, &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, verticesFormat, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDisableVertexAttribArray(0);


	glGenBuffers(1, &VBO_Normals);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Normals);
	glBufferData(GL_ARRAY_BUFFER, normals.size()*FLOAT_SIZE, &normals[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, normalsFormat, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDisableVertexAttribArray(1);


	glGenBuffers(1, &VBO_Texture);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Texture);
	glBufferData(GL_ARRAY_BUFFER, textUV.size()*FLOAT_SIZE, &textUV[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, textureFormat, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDisableVertexAttribArray(2);


	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementData.size()*2, &elementData[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

MeshVNT::~MeshVNT()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO_Vertices);
	glDeleteBuffers(1, &VBO_Normals);
	glDeleteBuffers(1, &VBO_Texture);
	glDeleteBuffers(1, &EBO);
}

// ########################################################
// Member Functions########################################
// ########################################################

void MeshVNT::render()
{
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindVertexArray(0);
}

}

