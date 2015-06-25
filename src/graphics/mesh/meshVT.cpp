
#include "meshVT.h"
#include <vector>
#include <GL/glew.h>

// ########################################################
// Constructor/Destructor #################################
// ########################################################

MeshVT::MeshVT(std::vector<GLfloat> &vertices, std::vector<GLfloat> textUV, std::vector<GLshort> &elementData)
{
	numberOfElements = elementData.size();

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	const int FLOAT_SIZE = (sizeof(float));

	glGenBuffers(1, &VBO_V);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_V);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*FLOAT_SIZE, &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDisableVertexAttribArray(0);

	glGenBuffers(1, &VBO_UV);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_UV);
	glBufferData(GL_ARRAY_BUFFER, textUV.size()*FLOAT_SIZE, &textUV[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDisableVertexAttribArray(1);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementData.size()*2, &elementData[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

MeshVT::~MeshVT()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO_V);
	glDeleteBuffers(1, &VBO_UV);
	glDeleteBuffers(1, &EBO);
}

// ########################################################
// Member Functions########################################
// ########################################################

void MeshVT::render()
{
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
}


