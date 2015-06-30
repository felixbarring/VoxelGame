#define GLM_FORCE_RADIANS

#include <vector>

#include "texturedCube.h"
#include "transform.h"


// ########################################################
// Constructor/Destructor #################################
// ########################################################

TexturedCube::TexturedCube(float xOffset, float yOffset, float zOffset, int id):
	xLocation{xOffset},
	yLocation{yOffset},
	zLocation{zOffset},
	transform{xOffset, yOffset, zOffset}
{

	std::vector<GLfloat> vertexData {
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

	mesh = new MeshVNT(vertexData, 3, normals, 3, UV, 2, elementData);

}

TexturedCube::~TexturedCube()
{
}

// ########################################################
// Member Functions########################################
// ########################################################

void TexturedCube::draw()
{
	mesh->render();
}

