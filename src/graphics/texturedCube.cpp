#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "texturedCube.h"
#include "transform.h"

#include "mesh/meshVNT.h"


// ########################################################
// Constructor/Destructor #################################
// ########################################################

TexturedCube::TexturedCube(float xOffset, float yOffset):
	xLocation{xOffset},
	yLocation{yOffset},
	width{1},
	height{1}
{

	glm::mat4 m = glm::mat4(1.0);
	modelMatrix = glm::translate(m, glm::vec3(xOffset, yOffset, 0.0f));

	std::vector<GLfloat> vetexData {
		// Front
		-0.5f, -0.5f, -0.5f, // 0
		0.5f, -0.5f, -0.5f, // 1
		0.5f, 0.5f, -0.5f, // 2
		-0.5f, 0.5f, -0.5f, // 3

		// Back
		0.5f, -0.5f, 0.5f, // 0
		-0.5f, -0.5f, 0.5f, // 1
		-0.5f, 0.5f, 0.5f, // 2
		0.5f, 0.5f, 0.5f, // 3

		// Left
		-0.5f, -0.5f, 0.5f, // 0
		-0.5f, -0.5f, -0.5f, // 1
		-0.5f, 0.5f, -0.5f, // 2
		-0.5f, 0.5f, 0.5f, // 3

		// Right
		0.5f, -0.5f, -0.5f, // 0
		0.5f, -0.5f, 0.5f, // 1
		0.5f, 0.5f, 0.5f, // 2
		0.5f, 0.5f, -0.5f, // 3

		// Top
		-0.5f, 0.5f, -0.5f, // 0
		0.5f, 0.5f, -0.5f, // 1
		0.5f, 0.5f, 0.5f, // 2
		-0.5f, 0.5f, 0.5f, // 3

		// Bottom
		-0.5f, -0.5f, 0.5f, // 0
		0.5f, -0.5f, 0.5f, // 1
		0.5f, -0.5f, -0.5f, // 2
		-0.5f, -0.5f, -0.5f, // 3
	};

	std::vector<GLfloat> normals {
		// Normals
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
	};

	int id = 240-16+1;

	int row = id % 16;
	int col = int(id / 16.0f);

	float k = 1.0f / 16.0f;

	float bottomLeftX = row * k;
	float bottomLeftY = col * k;

	float bottomRightX = row * k + k;
	float bottomRightY = col * k;

	float topRightX = row * k + k;
	float topRightY = col * k + k;

	float topLeftX = row * k;
	float topLeftY = col * k + k;

	std::vector<GLfloat> UV {

		topLeftX, topLeftY,
		topRightX, topRightY,
		bottomRightX, bottomRightY,
		bottomLeftX, bottomLeftY,

		topLeftX, topLeftY,
		topRightX, topRightY,
		bottomRightX, bottomRightY,
		bottomLeftX, bottomLeftY,

		topLeftX, topLeftY,
		topRightX, topRightY,
		bottomRightX, bottomRightY,
		bottomLeftX, bottomLeftY,

		topLeftX, topLeftY,
		topRightX, topRightY,
		bottomRightX, bottomRightY,
		bottomLeftX, bottomLeftY,

		topLeftX, topLeftY,
		topRightX, topRightY,
		bottomRightX, bottomRightY,
		bottomLeftX, bottomLeftY,

		topLeftX, topLeftY,
		topRightX, topRightY,
		bottomRightX, bottomRightY,
		bottomLeftX, bottomLeftY
	};

	std::vector<short> elementData{
		0, 1, 2, 0, 2, 3,
		0+4, 1+4, 2+4, 0+4, 2+4, 3+4,
		0+8, 1+8, 2+8, 0+8, 2+8, 3+8,
		0+12, 1+12, 2+12, 0+12, 2+12, 3+12,
		0+16, 1+16, 2+16, 0+16, 2+16, 3+16,
		0+20, 1+20, 2+20, 0+20, 2+20, 3+20,
	};

	mesh = new MeshVNT(vetexData, normals, UV, elementData);

}

TexturedCube::~TexturedCube()
{
}

// ########################################################
// Member Functions########################################
// ########################################################

glm::mat4& TexturedCube::getModelMatrix()
{
	return modelMatrix;
}

void TexturedCube::draw()
{
	mesh->render();
}


/*

	std::vector<GLfloat> UV {

			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,

			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,

			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,

			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,

			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,

			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,

		};

*/
