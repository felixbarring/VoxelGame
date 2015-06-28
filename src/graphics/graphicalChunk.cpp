
#include "graphicalChunk.h"

//#define GLM_FORCE_RADIANS

#include <vector>
#include <iostream>

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

	int width = 16;
	int height = 16;
	int depth = 16;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			for (int k = 0; k < depth; k++) {
				CubeFaceData cube;
				faceData[i][j][k] = cube;
				cube.vissible = data[i][j][k] != 0; // = for air
				cube.id = data[i][j][k];
				cube.front = true;
				cube.back = true;
				cube.left = true;
				cube.right = true;
				cube.top = true;
				cube.bottom = true;
			}
		}
	}

	// Remove faces
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			for (int k = 0; k < depth; k++) {

				if (i != width - 1) {
					const char right = faceData[i + 1][j][k].id;
					if (right != 0) {
						faceData[i][j][k].right = false;
						faceData[i + 1][j][k].left = false;
					}
				}

				if (j != height - 1) { // Okay to check up
					const char up = faceData[i][j + 1][k].id;
					if (up != 0) {
						faceData[i][j][k].top = false;
						faceData[i][j + 1][k].bottom = false;
					}
				}

				if (k != depth - 1) {
					const char back = faceData[i][j][k + 1].id;
					if (back != 0) {
						faceData[i][j][k].back = false;
						faceData[i][j][k + 1].front = false;
					}
				}
			}
		}
	}

	std::vector<GLfloat> vertexData;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> UV;
	std::vector<short> elementData;

	int elementOffset = 0;
	int totalNumberOfFaces = 0;

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			for (int k = 0; k < 16; k++) {

				CubeFaceData fd = faceData[i][j][k];

				if (!fd.vissible) {
					continue;
				}

				int row = fd.id % 16;
				int col = int(fd.id / 16.0f);

				float offset = 1.0f / 16.0f;

				float hack = 0.001;

				float bottomLeftX = row * offset + hack;
				float bottomLeftY = col * offset + hack;

				float bottomRightX = row * offset + offset - hack;
				float bottomRightY = col * offset + hack;

				float topRightX = row * offset + offset - hack;
				float topRightY = col * offset + offset - hack;

				float topLeftX = row * offset + hack;
				float topLeftY = col * offset + offset - hack;

				if (fd.front) {

					std::vector<GLfloat> vertex {
						-0.5f + i, -0.5f + j, -0.5f + k,
						0.5f + i, -0.5f + j, -0.5f + k,
						0.5f + i, 0.5f + j, -0.5f + k,
						-0.5f + i, 0.5f + j, -0.5f + k
					};

					std::vector<GLfloat> nor {
						0.0f, 0.0f, -1.0f,
						0.0f, 0.0f, -1.0f,
						0.0f, 0.0f, -1.0f,
						0.0f, 0.0f, -1.0f
					};

					std::vector<GLfloat> uv {
						topLeftX, topLeftY,
						topRightX, topRightY,
						bottomRightX, bottomRightY,
						bottomLeftX, bottomLeftY
					};

					std::vector<short> el{
						0 + elementOffset, 1 + elementOffset,
						2 + elementOffset, 0 + elementOffset,
						2 + elementOffset, 3 + elementOffset};

					for (auto v : vertex) { vertexData.push_back(v);}
					for (auto n : nor) { normals.push_back(n);}
					for (auto u : uv) { UV.push_back(u);}
					for (auto e : el) { elementData.push_back(e);}

					elementOffset += 4;
					totalNumberOfFaces++;
				}


				if (fd.back) {

					std::vector<GLfloat> vertex {
						0.5f + i, -0.5f + j, 0.5f + k, // 0
						-0.5f + i, -0.5f + j, 0.5f + k, // 1
						-0.5f + i, 0.5f + j, 0.5f + k, // 2
						0.5f + i, 0.5f + j, 0.5f + k, // 3
					};

					std::vector<GLfloat> nor {
						0.0f, 0.0f, 1.0f,
						0.0f, 0.0f, 1.0f,
						0.0f, 0.0f, 1.0f,
						0.0f, 0.0f, 1.0f,
					};

					std::vector<GLfloat> uv {
						topLeftX, topLeftY,
						topRightX, topRightY,
						bottomRightX, bottomRightY,
						bottomLeftX, bottomLeftY,
					};

					std::vector<short> el{
						0 + elementOffset, 1 + elementOffset,
						2 + elementOffset, 0 + elementOffset,
						2 + elementOffset, 3 + elementOffset};

					for (auto v : vertex) { vertexData.push_back(v);}
					for (auto n : nor) { normals.push_back(n);}
					for (auto u : uv) { UV.push_back(u);}
					for (auto e : el) {elementData.push_back(e);}

					elementOffset += 4;
					totalNumberOfFaces++;
				}

				if (fd.left) {

					std::vector<GLfloat> vertex {
						-0.5f + i, -0.5f + j, 0.5f + k, // 0
						-0.5f + i, -0.5f + j, -0.5f + k, // 1
						-0.5f + i, 0.5f + j, -0.5f + k, // 2
						-0.5f + i, 0.5f + j, 0.5f + k, // 3
					};

					std::vector<GLfloat> nor {
						-1.0f, 0.0f, 0.0f,
						-1.0f, 0.0f, 0.0f,
						-1.0f, 0.0f, 0.0f,
						-1.0f, 0.0f, 0.0f,
					};

					std::vector<GLfloat> uv {
						topLeftX, topLeftY,
						topRightX, topRightY,
						bottomRightX, bottomRightY,
						bottomLeftX, bottomLeftY,
					};

					std::vector<short> el{
						0 + elementOffset, 1 + elementOffset,
						2 + elementOffset, 0 + elementOffset,
						2 + elementOffset, 3 + elementOffset};

					for (auto v : vertex) { vertexData.push_back(v);}
					for (auto n : nor) { normals.push_back(n);}
					for (auto u : uv) { UV.push_back(u);}
					for (auto e : el) {elementData.push_back(e);}

					elementOffset += 4;
					totalNumberOfFaces++;
				}

				if (fd.right) {

					std::vector<GLfloat> vertex {
						0.5f + i, -0.5f + j, -0.5f + k, // 0
						0.5f + i, -0.5f + j, 0.5f + k, // 1
						0.5f + i, 0.5f + j, 0.5f + k, // 2
						0.5f + i, 0.5f + j, -0.5f + k, // 3
					};

					std::vector<GLfloat> nor {
						1.0f, 0.0f, 0.0f,
						1.0f, 0.0f, 0.0f,
						1.0f, 0.0f, 0.0f,
						1.0f, 0.0f, 0.0f,
					};

					std::vector<GLfloat> uv {
						topLeftX, topLeftY,
						topRightX, topRightY,
						bottomRightX, bottomRightY,
						bottomLeftX, bottomLeftY,
					};

					std::vector<short> el{
						0 + elementOffset, 1 + elementOffset,
						2 + elementOffset, 0 + elementOffset,
						2 + elementOffset, 3 + elementOffset};

					for (auto v : vertex) { vertexData.push_back(v);}
					for (auto n : nor) { normals.push_back(n);}
					for (auto u : uv) { UV.push_back(u);}
					for (auto e : el) {elementData.push_back(e);}

					elementOffset += 4;
					totalNumberOfFaces++;
				}


				if (fd.top) {

					std::vector<GLfloat> vertex {
						-0.5f + i, 0.5f + j, -0.5f + k, // 0
						0.5f + i, 0.5f + j, -0.5f + k, // 1
						0.5f + i, 0.5f + j, 0.5f + k, // 2
						-0.5f + i, 0.5f + j, 0.5f + k, // 3
					};

					std::vector<GLfloat> nor {
						0.0f, 1.0f, 0.0f,
						0.0f, 1.0f, 0.0f,
						0.0f, 1.0f, 0.0f,
						0.0f, 1.0f, 0.0f,
					};

					std::vector<GLfloat> uv {
						topLeftX, topLeftY,
						topRightX, topRightY,
						bottomRightX, bottomRightY,
						bottomLeftX, bottomLeftY,
					};

					std::vector<short> el{
						0 + elementOffset, 1 + elementOffset,
						2 + elementOffset, 0 + elementOffset,
						2 + elementOffset, 3 + elementOffset};

					for (auto v : vertex) { vertexData.push_back(v);}
					for (auto n : nor) { normals.push_back(n);}
					for (auto u : uv) { UV.push_back(u);}
					for (auto e : el) {elementData.push_back(e);}

					elementOffset += 4;
					totalNumberOfFaces++;
				}

				if (fd.bottom) {

					std::vector<GLfloat> vertex {
						-0.5f + i, -0.5f + j, 0.5f + k, // 0
						0.5f + i, -0.5f + j, 0.5f + k, // 1
						0.5f + i, -0.5f + j, -0.5f + k, // 2
						-0.5f + i, -0.5f + j, -0.5f + k // 3
					};

					std::vector<GLfloat> nor {
						0.0f, -1.0f, 0.0f,
						0.0f, -1.0f, 0.0f,
						0.0f, -1.0f, 0.0f,
						0.0f, -1.0f, 0.0f,
					};

					std::vector<GLfloat> uv {
						topLeftX, topLeftY,
						topRightX, topRightY,
						bottomRightX, bottomRightY,
						bottomLeftX, bottomLeftY
					};

					std::vector<short> el{
						0 + elementOffset, 1 + elementOffset,
						2 + elementOffset, 0 + elementOffset,
						2 + elementOffset, 3 + elementOffset};

					for (auto v : vertex) { vertexData.push_back(v);}
					for (auto n : nor) { normals.push_back(n);}
					for (auto u : uv) { UV.push_back(u);}
					for (auto e : el) {elementData.push_back(e);}

					elementOffset += 4;
					totalNumberOfFaces++;
				}
			}
		}
	}

	mesh = new MeshVNT(vertexData, normals, UV, elementData);
	std::cout<<"Total number of faces: "<<totalNumberOfFaces<<"\n";

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



