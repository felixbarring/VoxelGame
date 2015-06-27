
#include "graphicalChunk.h"

#define GLM_FORCE_RADIANS

#include <vector>

#include "texturedCube.h"
#include "transform.h"

// ########################################################
// Constructor/Destructor #################################
// ########################################################

GraphicalChunk::GraphicalChunk(float xOffset, float yOffset, float zOffset, char data[16][16][16]):
	xLocation{xOffset},
	yLocation{yOffset},
	zLocation{zOffset},
	transform{xOffset, yOffset, zOffset}
{

	std::vector<GLfloat> vertexData;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> UV;
	std::vector<short> elementData;

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			for (int k = 0; k < 16; k++) {
				int id = data[i][j][k];
				if (id == 0) {
					continue;
				}

				std::vector<GLfloat> vd {
					// Front
					-0.5f + i, -0.5f + j, -0.5f + k, // 0
					0.5f + i, -0.5f + j, -0.5f + k, // 1
					0.5f + i, 0.5f + j, -0.5f + k, // 2
					-0.5f + i, 0.5f + j, -0.5f + k, // 3

					// Back
					0.5f + i, -0.5f + j, 0.5f + k, // 0
					-0.5f + i, -0.5f + j, 0.5f + k, // 1
					-0.5f + i, 0.5f + j, 0.5f + k, // 2
					0.5f + i, 0.5f + j, 0.5f + k, // 3

					// Left
					-0.5f + i, -0.5f + j, 0.5f + k, // 0
					-0.5f + i, -0.5f + j, -0.5f + k, // 1
					-0.5f + i, 0.5f + j, -0.5f + k, // 2
					-0.5f + i, 0.5f + j, 0.5f + k, // 3

					// Right
					0.5f + i, -0.5f + j, -0.5f + k, // 0
					0.5f + i, -0.5f + j, 0.5f + k, // 1
					0.5f + i, 0.5f + j, 0.5f + k, // 2
					0.5f + i, 0.5f + j, -0.5f + k, // 3

					// Top
					-0.5f + i, 0.5f + j, -0.5f + k, // 0
					0.5f + i, 0.5f + j, -0.5f + k, // 1
					0.5f + i, 0.5f + j, 0.5f + k, // 2
					-0.5f + i, 0.5f + j, 0.5f + k, // 3

					// Bottom
					-0.5f + i, -0.5f + j, 0.5f + k, // 0
					0.5f + i, -0.5f + j, 0.5f + k, // 1
					0.5f + i, -0.5f + j, -0.5f + k, // 2
					-0.5f + i, -0.5f + j, -0.5f + k, // 3
				};

				for (auto v :vd) {
					vertexData.push_back(v);
				}

				std::vector<GLfloat> nor {
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

				for (auto n : nor) {
					normals.push_back(n);
				}

				int row = id % 16;
				int col = int(id / 16.0f);

				float offset = 1.0f / 16.0f;

				float bottomLeftX = row * offset;
				float bottomLeftY = col * offset;

				float bottomRightX = row * offset + offset;
				float bottomRightY = col * offset;

				float topRightX = row * offset + offset;
				float topRightY = col * offset + offset;

				float topLeftX = row * offset;
				float topLeftY = col * offset + offset;

				std::vector<GLfloat> u {

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

				for (auto uv : u) {
					UV.push_back(uv);
				}

				std::vector<short> ed{
					0, 1, 2, 0, 2, 3,
					0+4, 1+4, 2+4, 0+4, 2+4, 3+4,
					0+8, 1+8, 2+8, 0+8, 2+8, 3+8,
					0+12, 1+12, 2+12, 0+12, 2+12, 3+12,
					0+16, 1+16, 2+16, 0+16, 2+16, 3+16,
					0+20, 1+20, 2+20, 0+20, 2+20, 3+20,
				};

				for (auto e : ed) {
					elementData.push_back(e + 24 * (i + 16*j + 16*16*k));
				}

			}
		}
	}

	/*
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

	int id = 240;

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
	*/

	mesh = new MeshVNT(vertexData, normals, UV, elementData);

}

GraphicalChunk::~GraphicalChunk()
{
}

// ########################################################
// Member Functions########################################
// ########################################################

void GraphicalChunk::draw()
{
	mesh->render();
}



