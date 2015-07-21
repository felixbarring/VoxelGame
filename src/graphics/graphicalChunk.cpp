
#include "graphicalChunk.h"

#include <vector>
#include <iostream>

#include "texturedCube.h"
#include "transform.h"
//#include "../config/data.h"

// ########################################################
// Constructor/Destructor #################################
// ########################################################

GraphicalChunk::GraphicalChunk(int x, int y, int z,
		Voxel data[chunk_data::GRAPHICAL_CHUNK_WIDTH][chunk_data::GRAPHICAL_CHUNK_HEIGHT][chunk_data::GRAPHICAL_CHUNK_DEPTH]):
xLocation{x},
yLocation{y},
zLocation{z},
transform{x, y, z}
{

	CubeFaceData faceData[width][height][depth];

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			for (int k = 0; k < depth; k++) {
				CubeFaceData cube;
				cube.id = data[i][j][k].id;
				cube.front = true;
				cube.back = true;
				cube.left = true;
				cube.right = true;
				cube.top = true;
				cube.bottom = true;
				faceData[i][j][k] = cube;
			}
		}
	}

	// Remove faces
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			for (int k = 0; k < depth; k++) {

				if (i != width - 1) {
					const char right = faceData[i + 1][j][k].id;
					if (right != cube_data::AIR) {
						faceData[i][j][k].right = false;
						faceData[i + 1][j][k].left = false;
					}
				}

				if (j != height - 1) { // Okay to check up
					const char up = faceData[i][j + 1][k].id;
					if (up != cube_data::AIR) {
						faceData[i][j][k].top = false;
						faceData[i][j + 1][k].bottom = false;
					}
				}

				if (k != depth - 1) {
					const char back = faceData[i][j][k + 1].id;
					if (back != cube_data::AIR) {
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

	short elementOffset = 0;
	int totalNumberOfFaces = 0;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			for (int k = 0; k < depth; k++) {

				CubeFaceData fd = faceData[i][j][k];

				int id = fd.id;

				GLfloat sideTexture = cube_data::BLOCK_TEXTURES[id][cube_data::SIDE_TEXTURE];
				GLfloat topTexture = cube_data::BLOCK_TEXTURES[id][cube_data::TOP_TEXTURE];
				GLfloat bottomTexture = cube_data::BLOCK_TEXTURES[id][cube_data::BOTTOM_TEXTURE];

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

						0.0f, 0.0f, static_cast<GLfloat>(sideTexture),
						1.0f, 0.0f, static_cast<GLfloat>(sideTexture),
						1.0f, 1.0f, static_cast<GLfloat>(sideTexture),
						0.0f, 1.0f, static_cast<GLfloat>(sideTexture),
					};

					std::vector<short> el{
						0 + elementOffset, 1 + elementOffset, 2 + elementOffset,
						0 + elementOffset, 2 + elementOffset, 3 + elementOffset
					};

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
						0.0f, 0.0f, static_cast<GLfloat>(sideTexture),
						1.0f, 0.0f, static_cast<GLfloat>(sideTexture),
						1.0f, 1.0f, static_cast<GLfloat>(sideTexture),
						0.0f, 1.0f, static_cast<GLfloat>(sideTexture),
					};

					std::vector<short> el{
						0 + elementOffset, 1 + elementOffset, 2 + elementOffset,
						0 + elementOffset, 2 + elementOffset, 3 + elementOffset
					};

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
						0.0f, 0.0f, static_cast<GLfloat>(sideTexture),
						1.0f, 0.0f, static_cast<GLfloat>(sideTexture),
						1.0f, 1.0f, static_cast<GLfloat>(sideTexture),
						0.0f, 1.0f, static_cast<GLfloat>(sideTexture),
					};

					std::vector<short> el{
						0 + elementOffset, 1 + elementOffset, 2 + elementOffset,
						0 + elementOffset, 2 + elementOffset, 3 + elementOffset
					};

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
						0.0f, 0.0f, static_cast<GLfloat>(sideTexture),
						1.0f, 0.0f, static_cast<GLfloat>(sideTexture),
						1.0f, 1.0f, static_cast<GLfloat>(sideTexture),
						0.0f, 1.0f, static_cast<GLfloat>(sideTexture),
					};

					std::vector<short> el{
						0 + elementOffset, 1 + elementOffset, 2 + elementOffset,
						0 + elementOffset, 2 + elementOffset, 3 + elementOffset
					};

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
						-0.5f + i, 0.5f + j, 0.5f + k // 3
					};

					std::vector<GLfloat> nor {
						0.0f, 1.0f, 0.0f,
						0.0f, 1.0f, 0.0f,
						0.0f, 1.0f, 0.0f,
						0.0f, 1.0f, 0.0f
					};

					std::vector<GLfloat> uv {
						0.0f, 0.0f, static_cast<GLfloat>(topTexture),
						1.0f, 0.0f, static_cast<GLfloat>(topTexture),
						1.0f, 1.0f, static_cast<GLfloat>(topTexture),
						0.0f, 1.0f, static_cast<GLfloat>(topTexture)
					};

					std::vector<short> el{
						0 + elementOffset, 1 + elementOffset, 2 + elementOffset,
						0 + elementOffset, 2 + elementOffset, 3 + elementOffset
					};

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
						0.0f, -1.0f, 0.0f
					};

					std::vector<GLfloat> uv {
						0.0f, 0.0f, static_cast<GLfloat>(bottomTexture),
						1.0f, 0.0f, static_cast<GLfloat>(bottomTexture),
						1.0f, 1.0f, static_cast<GLfloat>(bottomTexture),
						0.0f, 1.0f, static_cast<GLfloat>(bottomTexture)
					};



					std::vector<short> el{
						0 + elementOffset, 1 + elementOffset, 2 + elementOffset,
						0 + elementOffset, 2 + elementOffset, 3 + elementOffset
					};

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

	mesh = new MeshVNT(vertexData, 3, normals, 3, UV, 3, elementData);
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

