#include "graphicalChunk.h"

#include <vector>
#include <iostream>

#include "texturedCube.h"
#include "transform.h"

using namespace std;
using namespace config::cube_data;

namespace graphics {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

GraphicalChunk::GraphicalChunk(float x, float y, float z,
	vector<vector<vector<Voxel>>>&data,
	vector<vector<vector<Voxel>>> *right,
	vector<vector<vector<Voxel>>> *left,
	vector<vector<vector<Voxel>>> *back,
	vector<vector<vector<Voxel>>> *front):
xLocation {x},
yLocation {y},
zLocation {z},
transform {
	x + width / 2 + 0.5f,
	y + height / 2 + 0.5f,
	z + depth / 2 + 0.5f
}
{

	vector<vector<vector<CubeFaceData>>> faceData;
	//CubeFaceData faceData[width][height][depth];

// ############################################################################
// ### Generate Cubeface Data #################################################
// ############################################################################
	for (int i = 0; i < width + 2; i++) {
		faceData.push_back(vector<vector<CubeFaceData>>());
		for (int j = 0; j < height + 2; j++) {
			faceData[i].push_back(vector<CubeFaceData>());
			for (int k = 0; k < depth + 2; k++) {
				CubeFaceData cube;

				auto *voxel = getVoxel(i - 1, j - 1, k - 1, data, right, left,
						back, front);
				if (voxel) {
					cube.id = voxel->id;
					cube.lightValue = voxel->lightValue;
				} else {
					cube.id = AIR;
					cube.lightValue = 0;
				}

				cube.front = true;
				cube.back = true;
				cube.left = true;
				cube.right = true;
				cube.top = true;
				cube.bottom = true;

				faceData[i][j].push_back(cube);
			}
		}
	}

	// Remove faces and compute lightning
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			for (int k = 0; k < depth; k++) {

				CubeFaceData &current = faceData[i + 1][j + 1][k + 1];
				if (current.id == AIR) {
					current.front = false;
					current.back = false;
					current.left = false;
					current.right = false;
					current.top = false;
					current.bottom = false;
					continue;
				}

				// X ##########################################################

				CubeFaceData cd = faceData[i + 2][j + 1][k + 1];
				if (cd.id != AIR) {
					current.right = false;
				} else {

					computeAverageRight(cd.lightValue, i + 1, j + 1, k + 1,
						current.lvRight_BottomLeft,
						current.lvRight_BottomRight,
						current.lvRight_TopRight,
						current.lvRight_TopLeft,
						faceData);

					doAORight(current, i + 1, j + 1, k + 1,faceData);
				}

				cd = faceData[i][j + 1][k + 1];
				if (cd.id != AIR) {
					current.left = false;
				} else {

					computeAverageLeft(cd.lightValue, i + 1, j + 1, k + 1,
						current.lvLeft_BottomLeft,
						current.lvLeft_BottomRight,
						current.lvLeft_TopRight,
						current.lvLeft_TopLeft,
						faceData);

					doAOLeft(current, i + 1, j + 1, k + 1, faceData);
				}

				// Y ##########################################################

				cd = faceData[i + 1][j + 2][k + 1];
				if (cd.id != AIR) {
					current.top = false;
				} else {

					computeAverageTop(cd.lightValue, i + 1, j + 1, k + 1,
						current.lvTop_BottomLeft,
						current.lvTop_BottomRight,
						current.lvTop_TopRight,
						current.lvTop_TopLeft, faceData);

					doAOTop(current, i + 1, j + 1, k + 1, faceData);
				}

				cd = faceData[i + 1][j][k + 1];
				if (cd.id != AIR) {
					current.bottom = false;
				} else {

					computeAverageBottom(cd.lightValue, i + 1, j + 1, k + 1,
						current.lvBottom_BottomLeft,
						current.lvBottom_BottomRight,
						current.lvBottom_TopRight,
						current.lvBottom_TopLeft, faceData);

					doAOBottom(current, i + 1, j + 1, k + 1, faceData);
				}

				// Z ##########################################################

				cd = faceData[i + 1][j + 1][k + 2];
				if (cd.id != AIR) {
					current.back = false;
				} else {

					computeAverageBack(cd.lightValue, i + 1, j + 1, k + 1,
							current.lvBack_BottomLeft,
							current.lvBack_BottomRight,
							current.lvBack_TopRight,
							current.lvBack_TopLeft, faceData);

					doAOBack(current, i + 1, j + 1, k + 1, faceData);
				}

				cd = faceData[i + 1][j + 1][k];
				if (cd.id != AIR) {
					current.front = false;
				} else {

					computeAverageFront(cd.lightValue, i + 1, j + 1, k + 1,
						current.lvFront_BottomLeft,
						current.lvFront_BottomRight,
						current.lvFront_TopRight,
						current.lvFront_TopLeft, faceData);

					doAOFront(current, i + 1, j + 1, k + 1, faceData);
				}
			}
		}
	}

// ############################################################################
// Create the mesh data #######################################################
// ############################################################################

	// The fourth element of the vertex data is the light value.
	vector<GLfloat> vertexData;
	vector<GLfloat> normals;
	vector<GLfloat> UV;
	vector<short> elementData;

	short elementOffset = 0;
	int totalNumberOfFaces = 0;

	float dx = -width / 2;
	float dy = -height / 2;
	float dz = -depth / 2;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			for (int k = 0; k < depth; k++) {

				CubeFaceData fd = faceData[i + 1][j + 1][k + 1];

				int id = fd.id;
				if (id == AIR)
					continue;

				GLfloat sideTexture = BLOCK_TEXTURES[id][SIDE_TEXTURE];
				GLfloat topTexture = BLOCK_TEXTURES[id][TOP_TEXTURE];
				GLfloat bottomTexture = BLOCK_TEXTURES[id][BOTTOM_TEXTURE];

				if (fd.right) {

					vector<GLfloat> vertex {
						0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz,
							fd.lvRight_TopLeft,
						0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz,
							fd.lvRight_TopRight,
						0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz,
							fd.lvRight_BottomRight,
						0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz,
							fd.lvRight_BottomLeft,
					};

					vector<GLfloat> nor {
						1.0f, 0.0f, 0.0f,
						1.0f, 0.0f, 0.0f,
						1.0f, 0.0f, 0.0f,
						1.0f, 0.0f, 0.0f,
					};

					vector<GLfloat> uv {
						0.0f, 0.0f, static_cast<GLfloat>(sideTexture),
						1.0f, 0.0f, static_cast<GLfloat>(sideTexture),
						1.0f, 1.0f, static_cast<GLfloat>(sideTexture),
						0.0f, 1.0f, static_cast<GLfloat>(sideTexture),
					};

					vector<short> el {
						static_cast<short>(0 + elementOffset),
						static_cast<short>(1 + elementOffset),
						static_cast<short>(2 + elementOffset),
						static_cast<short>(0 + elementOffset),
						static_cast<short>(2 + elementOffset),
						static_cast<short>(3 + elementOffset)
					};

					for (auto v : vertex) {vertexData.push_back(v);}
					for (auto n : nor) {normals.push_back(n);}
					for (auto u : uv) {UV.push_back(u);}
					for (auto e : el) {elementData.push_back(e);}

					elementOffset += 4;
					totalNumberOfFaces++;
				}

				if (fd.left) {

					vector<GLfloat> vertex {
						-0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz,
							fd.lvLeft_TopLeft,
						-0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz,
							fd.lvLeft_TopRight ,
						-0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz,
							fd.lvLeft_BottomRight,
						-0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz,
							fd.lvLeft_BottomLeft,
					};

					vector<GLfloat> nor {
						-1.0f, 0.0f, 0.0f,
						-1.0f, 0.0f, 0.0f,
						-1.0f, 0.0f, 0.0f,
						-1.0f, 0.0f, 0.0f,
					};

					vector<GLfloat> uv {
						0.0f, 0.0f, static_cast<GLfloat>(sideTexture),
						1.0f, 0.0f, static_cast<GLfloat>(sideTexture),
						1.0f, 1.0f, static_cast<GLfloat>(sideTexture),
						0.0f, 1.0f, static_cast<GLfloat>(sideTexture),
					};

					vector<short> el {
						static_cast<short>(0 + elementOffset),
						static_cast<short>(1 + elementOffset),
						static_cast<short>(2 + elementOffset),
						static_cast<short>(0 + elementOffset),
						static_cast<short>(2 + elementOffset),
						static_cast<short>(3 + elementOffset)
					};

					for (auto v : vertex) {vertexData.push_back(v);}
					for (auto n : nor) {normals.push_back(n);}
					for (auto u : uv) {UV.push_back(u);}
					for (auto e : el) {elementData.push_back(e);}

					elementOffset += 4;
					totalNumberOfFaces++;
				}

				if (fd.back) {

					vector<GLfloat> vertex {
						0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz,
							fd.lvBack_TopLeft,
						-0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz,
							fd.lvBack_TopRight,
						-0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz,
							fd.lvBack_BottomRight,
						0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz,
						fd.lvBack_BottomLeft,

					};

					vector<GLfloat> nor {
						0.0f, 0.0f, 1.0f,
						0.0f, 0.0f, 1.0f,
						0.0f, 0.0f, 1.0f,
						0.0f, 0.0f, 1.0f,
					};

					vector<GLfloat> uv {
						0.0f, 0.0f, static_cast<GLfloat>(sideTexture),
						1.0f, 0.0f, static_cast<GLfloat>(sideTexture),
						1.0f, 1.0f, static_cast<GLfloat>(sideTexture),
						0.0f, 1.0f, static_cast<GLfloat>(sideTexture),
					};

					vector<short> el {
						static_cast<short>(0 + elementOffset),
						static_cast<short>(1 + elementOffset),
						static_cast<short>(2 + elementOffset),
						static_cast<short>(0 + elementOffset),
						static_cast<short>(2 + elementOffset),
						static_cast<short>(3 + elementOffset)
					};

					for (auto v : vertex) {vertexData.push_back(v);}
					for (auto n : nor) {normals.push_back(n);}
					for (auto u : uv) {UV.push_back(u);}
					for (auto e : el) {elementData.push_back(e);}

					elementOffset += 4;
					totalNumberOfFaces++;
				}

				if (fd.front) {

					vector<GLfloat> vertex {
						-0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz,
							fd.lvFront_TopLeft,
						0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz,
							fd.lvFront_TopRight,
						0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz,
							fd.lvFront_BottomRight,
						-0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz,
							fd.lvFront_BottomLeft,
					};

					vector<GLfloat> nor {
						0.0f, 0.0f, -1.0f,
						0.0f, 0.0f, -1.0f,
						0.0f, 0.0f, -1.0f,
						0.0f, 0.0f, -1.0f
					};

					vector<GLfloat> uv {
						0.0f, 0.0f, static_cast<GLfloat>(sideTexture),
						1.0f, 0.0f, static_cast<GLfloat>(sideTexture),
						1.0f, 1.0f, static_cast<GLfloat>(sideTexture),
						0.0f, 1.0f, static_cast<GLfloat>(sideTexture),
					};

					vector<short> el {
						static_cast<short>(0 + elementOffset),
						static_cast<short>(1 + elementOffset),
						static_cast<short>(2 + elementOffset),
						static_cast<short>(0 + elementOffset),
						static_cast<short>(2 + elementOffset),
						static_cast<short>(3 + elementOffset)
					};

					for (auto v : vertex) {vertexData.push_back(v);}
					for (auto n : nor) {normals.push_back(n);}
					for (auto u : uv) {UV.push_back(u);}
					for (auto e : el) {elementData.push_back(e);}

					elementOffset += 4;
					totalNumberOfFaces++;
				}

				if (fd.top) {

					vector<GLfloat> vertex {
						-0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz,
							fd.lvTop_TopLeft,
						0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz,
							fd.lvTop_TopRight,
						0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz,
							fd.lvTop_BottomRight,
						-0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz,
							fd.lvTop_BottomLeft,
					};

					vector<GLfloat> nor {
						0.0f, 1.0f, 0.0f,
						0.0f, 1.0f, 0.0f,
						0.0f, 1.0f, 0.0f,
						0.0f, 1.0f, 0.0f
					};

					vector<GLfloat> uv {
						0.0f, 0.0f, static_cast<GLfloat>(topTexture),
						1.0f, 0.0f, static_cast<GLfloat>(topTexture),
						1.0f, 1.0f, static_cast<GLfloat>(topTexture),
						0.0f, 1.0f, static_cast<GLfloat>(topTexture)
					};

					vector<short> el {
						static_cast<short>(0 + elementOffset),
						static_cast<short>(1 + elementOffset),
						static_cast<short>(2 + elementOffset),
						static_cast<short>(0 + elementOffset),
						static_cast<short>(2 + elementOffset),
						static_cast<short>(3 + elementOffset)
					};

					for (auto v : vertex) {vertexData.push_back(v);}
					for (auto n : nor) {normals.push_back(n);}
					for (auto u : uv) {UV.push_back(u);}
					for (auto e : el) {elementData.push_back(e);}

					elementOffset += 4;
					totalNumberOfFaces++;
				}

				if (fd.bottom) {

					vector<GLfloat> vertex {
						-0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz,
							fd.lvBottom_TopLeft,

						0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz,
							fd.lvBottom_TopRight,

						0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz,
							fd.lvBottom_BottomRight,

						-0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz,
							fd.lvBottom_BottomLeft,
					};

					vector<GLfloat> nor {
						0.0f, -1.0f, 0.0f,
						0.0f, -1.0f, 0.0f,
						0.0f, -1.0f, 0.0f,
						0.0f, -1.0f, 0.0f
					};

					vector<GLfloat> uv {
						0.0f, 0.0f, static_cast<GLfloat>(bottomTexture),
						1.0f, 0.0f, static_cast<GLfloat>(bottomTexture),
						1.0f, 1.0f, static_cast<GLfloat>(bottomTexture),
						0.0f, 1.0f, static_cast<GLfloat>(bottomTexture)
					};

					vector<short> el {
						static_cast<short>(0 + elementOffset),
						static_cast<short>(1 + elementOffset),
						static_cast<short>(2 + elementOffset),
						static_cast<short>(0 + elementOffset),
						static_cast<short>(2 + elementOffset),
						static_cast<short>(3 + elementOffset)
					};

					for (auto v : vertex) {vertexData.push_back(v);}
					for (auto n : nor) {normals.push_back(n);}
					for (auto u : uv) {UV.push_back(u);}
					for (auto e : el) {elementData.push_back(e);}

					elementOffset += 4;
					totalNumberOfFaces++;
				}
			}
		}
	}

	mesh.reset(new mesh::MeshElement(vertexData, 4, normals, 3, UV, 3,
			elementData));
	//cout<<"Total number of faces: "<<totalNumberOfFaces<<"\n";
}

// ########################################################
// Member Functions########################################
// ########################################################

void GraphicalChunk::draw() {
	mesh->draw();
}

Transform& GraphicalChunk::getTransform() {
	return transform;
}

float GraphicalChunk::getxLocation() {
	return xLocation;
}

float GraphicalChunk::getyLocation() {
	return yLocation;
}

// Function for getting voxels, can collect from neighbor chunks data
// If there is no neighbor, nullptr will be returned
// Trying to get a voxel that is not adjacent to this chunk is an error
Voxel* GraphicalChunk::getVoxel(int x, int y, int z,
	vector<vector<vector<Voxel>>>&data,
	vector<vector<vector<Voxel>>> *right,
	vector<vector<vector<Voxel>>> *left,
	vector<vector<vector<Voxel>>> *back,
	vector<vector<vector<Voxel>>> *front) {

	if (x < width && x >= 0 && y < height && y >= 0 && z < depth && z >= 0) {
		return &data[x][y][z];
	} else if (x == width && (y < height && y >= 0 && z < depth && z >= 0)) {
		if (right != nullptr) {
			return &((*right)[0][y][z]);
		} else {
			return nullptr;
		}
	} else if (x == -1 && (y < height && y >= 0 && z < depth && z >= 0)) {
		if (left != nullptr) {
			return &((*left)[width - 1][y][z]);
		} else {
			return nullptr;
		}
	} else if (z == depth && (x < width && x >= 0 && y < height && y >= 0)) {
		if (back != nullptr) {
			return &((*back)[x][y][0]);
		} else {
			return nullptr;
		}
	} else if (z == -1 && (x < width && x >= 0 && y < height && y >= 0)) {
		if (front != nullptr) {
			return &((*front)[x][y][depth - 1]);
		} else {
			return nullptr;
		}
	}

	return nullptr;
}

int AOFactor = 5;

void GraphicalChunk::doAORight(CubeFaceData &cf, int x, int y, int z,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData) {

	char bottomLeft = 0;
	CubeFaceData v = faceData[x + 1][y - 1][z];
	if (v.id != AIR) bottomLeft++;
	v = faceData[x + 1][y][z - 1];
	if (v.id != AIR) bottomLeft++;
	v = faceData[x + 1][y - 1][z - 1];
	if (v.id != AIR) bottomLeft++;
	cf.lvRight_BottomLeft -= bottomLeft * AOFactor;

	char bottomRight = 0;
	v = faceData[x + 1][y - 1][z];
	if (v.id != AIR) bottomRight++;
	v = faceData[x + 1][y][z];
	if (v.id != AIR) bottomRight++;
	v = faceData[x + 1][y - 1][z + 1];
	if ( v.id != AIR) bottomRight++;
	cf.lvRight_BottomRight -= bottomRight * AOFactor;

	char topRight = 0;
	v = faceData[x + 1][y + 1][z];
	if (v.id != AIR) topRight++;
	v = faceData[x + 1][y][z + 1];
	if (v.id != AIR) topRight++;
	v = faceData[x + 1][y + 1][z + 1];
	if (v.id != AIR) topRight++;
	cf.lvRight_TopRight -= topRight * AOFactor;

	char topLeft = 0;
	v = faceData[x + 1][y + 1][z];
	if (v.id != AIR) topLeft++;
	v = faceData[x + 1][y][z - 1];
	if (v.id != AIR) topLeft++;
	v = faceData[x + 1][y + 1][z - 1];
	if (v.id != AIR) topLeft++;
	cf.lvRight_TopLeft -= topLeft * AOFactor;
}

void GraphicalChunk::doAOLeft(CubeFaceData &cf, int x, int y, int z,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData) {

	char bottomLeft = 0;
	CubeFaceData v = faceData[x - 1][y - 1][z];
	if (v.id != AIR) bottomLeft++;
	v = faceData[x - 1][y][z + 1];
	if (v.id != AIR) bottomLeft++;
	v = faceData[x - 1][y - 1][z + 1];
	if (v.id != AIR) bottomLeft++;
	cf.lvLeft_BottomLeft -= bottomLeft * AOFactor;

	char bottomRight = 0;
	v = faceData[x - 1][y - 1][z];
	if (v.id != AIR) bottomRight++;
	v = faceData[x - 1][y][z - 1];
	if (v.id != AIR) bottomRight++;
	v = faceData[x - 1][y -1 ][z - 1];
	if (v.id != AIR) bottomRight++;
	cf.lvLeft_BottomRight -= bottomRight * AOFactor;

	char topRight = 0;
	v = faceData[x - 1][y + 1][z];
	if (v.id != AIR) topRight++;
	v = faceData[x - 1][y][z + 1];
	if (v.id != AIR) topRight++;
	v = faceData[x - 1][y +1][z + 1];
	if (v.id != AIR) topRight++;
	cf.lvLeft_TopLeft -= topRight * AOFactor;

	char topLeft = 0;
	v = faceData[x - 1][y + 1][z];
	if (v.id != AIR) topLeft++;
	v = faceData[x - 1][y][z - 1];
	if (v.id != AIR) topLeft++;
	v = faceData[x - 1][y + 1][z - 1];
	if (v.id != AIR) topLeft++;
	cf.lvLeft_TopRight -= topLeft * AOFactor;
}

void GraphicalChunk::doAOBack(CubeFaceData &cf, int x, int y, int z,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData) {

	char bottomLeft = 0;
	CubeFaceData v = faceData[x][y - 1][z + 1];
	if (v.id != AIR) bottomLeft++;
	v = faceData[x + 1][y][z + 1];
	if (v.id != AIR) bottomLeft++;
	v = faceData[x + 1][y - 1][z + 1];
	if (v.id != AIR) bottomLeft++;
	cf.lvBack_BottomLeft -= bottomLeft * AOFactor;

	char bottomRight = 0;
	v = faceData[x][y - 1][z + 1];
	if (v.id != AIR) bottomRight++;
	v = faceData[x - 1][y][z + 1];
	if (v.id != AIR) bottomRight++;
	v = faceData[x - 1][y - 1][z + 1];
	if (v.id != AIR) bottomRight++;
	cf.lvBack_BottomRight -= bottomRight * AOFactor;

	char topRight = 0;
	v = faceData[x][y + 1][z + 1];
	if (v.id != AIR) topRight++;
	v = faceData[x - 1][y][z + 1];
	if (v.id != AIR) topRight++;
	v = faceData[x - 1][y + 1][z + 1];
	if (v.id != AIR) topRight++;
	cf.lvBack_TopRight -= topRight * AOFactor;

	char topLeft = 0;
	v = faceData[x][y + 1][z + 1];
	if (v.id != AIR) topLeft++;
	v = faceData[x + 1][y][z + 1];
	if (v.id != AIR) topLeft++;
	v = faceData[x + 1][y + 1][z + 1];
	if (v.id != AIR) topLeft++;
	cf.lvBack_TopLeft -= topLeft * AOFactor;
}

void GraphicalChunk::doAOFront(CubeFaceData &cf, int x, int y, int z,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData) {

	char bottomLeft = 0;
	CubeFaceData v = faceData[x][y - 1][z - 1];
	if (v.id != AIR) bottomLeft++;
	v = faceData[x - 1][y][z - 1];
	if (v.id != AIR) bottomLeft++;
	v = faceData[x - 1][y - 1][z - 1];
	if ( v.id != AIR) bottomLeft++;
	cf.lvFront_BottomLeft -= bottomLeft * AOFactor;

	char bottomRight = 0;
	v = faceData[x][y - 1][z - 1];
	if (v.id != AIR) bottomRight++;
	v = faceData[x + 1][y][z - 1];
	if (v.id != AIR) bottomRight++;
	v = faceData[x + 1][y - 1][z - 1];
	if (v.id != AIR) bottomRight++;
	cf.lvFront_BottomRight -= bottomRight * AOFactor;

	char topRight = 0;
	v = faceData[x][y + 1][z - 1];
	if (v.id != AIR) topRight++;
	v = faceData[x + 1][y][z-+ 1];
	if (v.id != AIR) topRight++;
	v = faceData[x + 1][y + 1][z - 1];
	if (v.id != AIR) topRight++;
	cf.lvFront_TopRight -= topRight * AOFactor;

	char topLeft = 0;
	v = faceData[x][y + 1][z - 1];
	if (v.id != AIR) topLeft++;
	v = faceData[x - 1][y][z - 1];
	if (v.id != AIR) topLeft++;
	v = faceData[x - 1][y + 1][z - 1];
	if (v.id != AIR) topLeft++;
	cf.lvFront_TopLeft -= topLeft * AOFactor;
}

void GraphicalChunk::doAOTop(CubeFaceData &cf, int x, int y, int z,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData) {

	char bottomLeft = 0;
	CubeFaceData v = faceData[x][y + 1][z - 1];
	if (v.id != AIR) bottomLeft++;
	v = faceData[x - 1][y + 1][z];
	if (v.id != AIR) bottomLeft++;
	v = faceData[x - 1][y + 1][z - 1];
	if (v.id != AIR) bottomLeft++;
	cf.lvTop_BottomLeft -= bottomLeft * AOFactor;

	char bottomRight = 0;
	v = faceData[x][y + 1][z - 1];
	if (v.id != AIR) bottomRight++;
	v = faceData[x + 1][y + 1][z];
	if (v.id != AIR) bottomRight++;
	v = faceData[x + 1][y + 1][z - 1];
	if (v.id != AIR) bottomRight++;
	cf.lvTop_BottomRight-= bottomRight * AOFactor;

	char topRight = 0;
	v = faceData[x][y + 1][z + 1];
	if (v.id != AIR) topRight++;
	v = faceData[x + 1][y + 1][z];
	if (v.id != AIR) topRight++;
	v = faceData[x + 1][y + 1][z + 1];
	if (v.id != AIR) topRight++;
	cf.lvTop_TopRight -= topRight * AOFactor;

	char topLeft = 0;
	v = faceData[x][y + 1][z + 1];
	if (v.id != AIR) topLeft++;
	v = faceData[x - 1][y + 1][z];
	if ( v.id != AIR) topLeft++;
	v = faceData[x - 1][y + 1][z + 1];
	if (v.id != AIR) topLeft++;
	cf.lvTop_TopLeft -= topLeft * AOFactor;
}

void GraphicalChunk::doAOBottom(CubeFaceData &cf, int x, int y, int z,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData) {

	char bottomLeft = 0;
	CubeFaceData v = faceData[x][y - 1][z - 1];
	if (v.id != AIR) bottomLeft++;
	v = faceData[x - 1][y - 1][z];
	if (v.id != AIR) bottomLeft++;
	v = faceData[x - 1][y - 1][z - 1];
	if (v.id != AIR) bottomLeft++;
	cf.lvBottom_BottomLeft -= bottomLeft * AOFactor;

	char bottomRight = 0;
	v = faceData[x][y - 1][z - 1];
	if (v.id != AIR) bottomRight++;
	v = faceData[x + 1][y - 1][z];
	if (v.id != AIR) bottomRight++;
	v = faceData[x + 1][y - 1][z - 1];
	if (v.id != AIR) bottomRight++;
	cf.lvBottom_BottomRight-= bottomRight * AOFactor;

	char topRight = 0;
	v = faceData[x][y - 1][z + 1];
	if (v.id != AIR) topRight++;
	v = faceData[x + 1][y - 1][z];
	if (v.id != AIR) topRight++;
	v = faceData[x + 1][y - 1][z + 1];
	if (v.id != AIR) topRight++;
	cf.lvBottom_TopRight -= topRight * AOFactor;

	char topLeft = 0;
	v = faceData[x][y - 1][z + 1];
	if (v.id != AIR) topLeft++;
	v = faceData[x - 1][y - 1][z];
	if (v.id != AIR) topLeft++;
	v = faceData[x - 1][y - 1][z + 1];
	if (v.id != AIR) topLeft++;
	cf.lvBottom_TopLeft -= topLeft * AOFactor;
}

inline void GraphicalChunk::computeAverageHelper(
		int lightValue,
		const CubeFaceData& cLeftLeft,
		const CubeFaceData& cLeftLeft_Opposite,
		const CubeFaceData& cBottomLeft,
		const CubeFaceData& cBottomLeft_Opposite,
		const CubeFaceData& cBottomMiddle,
		const CubeFaceData& cBottomMiddle_Opposite,
		const CubeFaceData& cBottomRight,
		const CubeFaceData& cBottomRight_Opposite,
		const CubeFaceData& cRightRight,
		const CubeFaceData& cRightRight_Opposite,
		const CubeFaceData& cTopRight,
		const CubeFaceData& cTopRight_Opposite,
		const CubeFaceData& cTopMiddle,
		const CubeFaceData& cTopMiddle_Opposite,
		const CubeFaceData& cTopLeft,
		const CubeFaceData& cTopLeft_Opposite,
		float& bottomLeft,
		float& bottomRight,
		float& topRight,
		float& topLeft) {

	float counter {1};
	float acc = lightValue;
	if (cLeftLeft.id != AIR && cLeftLeft_Opposite.id == AIR) {
		acc += cLeftLeft_Opposite.lightValue;
		++counter;
	}
	if (cBottomLeft.id != AIR && cBottomLeft_Opposite.id == AIR) {
		acc += cBottomLeft_Opposite.lightValue;
		++counter;
	}
	if (cBottomMiddle.id != AIR && cBottomMiddle_Opposite.id == AIR) {
		acc += cBottomMiddle_Opposite.lightValue;
		++counter;
	}
	bottomLeft = acc / counter;
	counter = 1;
	acc = lightValue;
	if (cBottomMiddle.id != AIR && cBottomMiddle_Opposite.id == AIR) {
		acc += cBottomMiddle_Opposite.lightValue;
		++counter;
	}
	if (cBottomRight.id != AIR && cBottomRight_Opposite.id == AIR) {
		acc += cBottomRight_Opposite.lightValue;
		++counter;
	}
	if (cRightRight.id != AIR && cRightRight_Opposite.id == AIR) {
		acc += cRightRight_Opposite.lightValue;
		++counter;
	}
	bottomRight = acc / counter;
	counter = 1;
	acc = lightValue;
	if (cRightRight.id != AIR && cRightRight.id == AIR) {
		acc += cRightRight_Opposite.lightValue;
		++counter;
	}
	if (cTopRight.id != AIR && cTopRight_Opposite.id == AIR) {
		acc += cTopRight_Opposite.lightValue;
		++counter;
	}
	topRight = acc / counter;
	counter = 1;
	acc = lightValue;
	if (cTopMiddle.id != AIR && cTopMiddle_Opposite.id == AIR) {
		acc += cTopMiddle_Opposite.lightValue;
		++counter;
	}
	if (cTopLeft.id != AIR && cTopLeft_Opposite.id == AIR) {
		acc += cTopLeft_Opposite.lightValue;
		++counter;
	}
	if (cLeftLeft.id != AIR && cLeftLeft_Opposite.id == AIR) {
		acc += cLeftLeft_Opposite.lightValue;
		++counter;
	}
	topLeft = acc / counter;
}

void GraphicalChunk::computeAverageRight(int lightValue, int x, int y, int z,
		float &bottomLeft, float &bottomRight, float &topRight, float &topLeft,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData) {

	CubeFaceData cBottomLeft = faceData[x][y - 1][z - 1];
	CubeFaceData cBottomLeft_Right = faceData[x + 1][y - 1][z - 1];

	CubeFaceData cBottomMiddle = faceData[x][y - 1][z];
	CubeFaceData cBottomMiddle_Right = faceData[x + 1][y - 1][z];

	CubeFaceData cBottomRight = faceData[x][y - 1][z + 1];
	CubeFaceData cBottomRight_Right = faceData[x + 1][y - 1][z + 1];

	CubeFaceData cRightRight = faceData[x][y][z + 1];
	CubeFaceData cRightRight_Right = faceData[x + 1][y][z + 1];

	CubeFaceData cTopRight = faceData[x][y + 1][z + 1];
	CubeFaceData cTopRight_Right = faceData[x + 1][y + 1][z + 1];

	CubeFaceData cTopMiddle = faceData[x][y + 1][z];
	CubeFaceData cTopMiddle_Right = faceData[x + 1][y + 1][z];

	CubeFaceData cTopLeft = faceData[x][y + 1][z - 1];
	CubeFaceData cTopLeft_Right = faceData[x + 1][y + 1][z - 1];

	CubeFaceData cLeftLeft = faceData[x][y][z - 1];
	CubeFaceData cLeftLeft_Right = faceData[x + 1][y][z - 1];

	computeAverageHelper(lightValue, cLeftLeft, cLeftLeft_Right, cBottomLeft,
			cBottomLeft_Right, cBottomMiddle, cBottomMiddle_Right, cBottomRight,
			cBottomRight_Right, cRightRight, cRightRight_Right, cTopRight,
			cTopRight_Right, cTopMiddle, cTopMiddle_Right, cTopLeft,
			cTopLeft_Right, bottomLeft, bottomRight, topRight, topLeft);
}

void GraphicalChunk::computeAverageLeft(int lightValue, int x, int y, int z,
		float &bottomLeft, float &bottomRight,
		float &topRight, float &topLeft,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData) {

	CubeFaceData cBottomLeft = faceData[x][y - 1][z + 1];
	CubeFaceData cBottomLeft_Left = faceData[x - 1][y - 1][z + 1];

	CubeFaceData cBottomMiddle = faceData[x][y - 1][z];
	CubeFaceData cBottomMiddle_Left = faceData[x - 1][y - 1][z];

	CubeFaceData cBottomRight = faceData[x][y - 1][z +- 1];
	CubeFaceData cBottomRight_Left = faceData[x - 1][y - 1][z - 1];

	CubeFaceData cRightRight = faceData[x][y][z - 1];
	CubeFaceData cRightRight_Left = faceData[x - 1][y][z - 1];

	CubeFaceData cTopRight = faceData[x][y + 1][z - 1];
	CubeFaceData cTopRight_Left = faceData[x - 1][y + 1][z - 1];

	CubeFaceData cTopMiddle = faceData[x][y + 1][z];
	CubeFaceData cTopMiddle_Left = faceData[x - 1][y + 1][z];

	CubeFaceData cTopLeft = faceData[x][y + 1][z + 1];
	CubeFaceData cTopLeft_Left = faceData[x - 1][y + 1][z + 1];

	CubeFaceData cLeftLeft = faceData[x][y][z + 1];
	CubeFaceData cLeftLeft_Left = faceData[x - 1][y][z + 1];

	computeAverageHelper(lightValue, cLeftLeft, cLeftLeft_Left, cBottomLeft,
			cBottomLeft_Left, cBottomMiddle, cBottomMiddle_Left, cBottomRight,
			cBottomRight_Left, cRightRight, cRightRight_Left, cTopRight,
			cTopRight_Left, cTopMiddle, cTopMiddle_Left, cTopLeft,
			cTopLeft_Left, bottomLeft, bottomRight, topRight, topLeft);

}

void GraphicalChunk::computeAverageTop(int lightValue, int x, int y, int z,
		float &bottomLeft, float &bottomRight,
		float &topRight, float &topLeft,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData) {

	CubeFaceData cBottomLeft = faceData[x - 1][y][z - 1];
	CubeFaceData cBottomLeft_Top = faceData[x - 1][y + 1][z - 1];

	CubeFaceData cBottomMiddle = faceData[x][y][z - 1];
	CubeFaceData cBottomMiddle_Top = faceData[x][y + 1][z - 1];

	CubeFaceData cBottomRight = faceData[x + 1][y][z - 1];
	CubeFaceData cBottomRight_Top = faceData[x + 1][y + 1][z + 1];

	CubeFaceData cRightRight = faceData[x + 1][y][z];
	CubeFaceData cRightRight_Top = faceData[x + 1][y + 1][z];

	CubeFaceData cTopRight = faceData[x + 1][y][z + 1];
	CubeFaceData cTopRight_Top = faceData[x + 1][y + 1][z + 1];

	CubeFaceData cTopMiddle = faceData[x][y][z + 1];
	CubeFaceData cTopMiddle_Top = faceData[x][y + 1][z + 1];

	CubeFaceData cTopLeft = faceData[x - 1][y][z + 1];
	CubeFaceData cTopLeft_Top = faceData[x - 1][y + 1][z + 1];

	CubeFaceData cLeftLeft = faceData[x - 1][y][z];
	CubeFaceData cLeftLeft_Top = faceData[x - 1][y + 1][z];

	computeAverageHelper(lightValue, cLeftLeft, cLeftLeft_Top, cBottomLeft,
			cBottomLeft_Top, cBottomMiddle, cBottomMiddle_Top, cBottomRight,
			cBottomRight_Top, cRightRight, cRightRight_Top, cTopRight,
			cTopRight_Top, cTopMiddle, cTopMiddle_Top, cTopLeft,
			cTopLeft_Top, bottomLeft, bottomRight, topRight, topLeft);

}

void GraphicalChunk::computeAverageBottom(int lightValue, int x, int y, int z,
		float &bottomLeft, float &bottomRight,
		float &topRight, float &topLeft,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData) {

	CubeFaceData cBottomLeft = faceData[x - 1][y][z + 1];
	CubeFaceData cBottomLeft_Bottom = faceData[x - 1][y - 1][z + 1];

	CubeFaceData cBottomMiddle = faceData[x][y][z + 1];
	CubeFaceData cBottomMiddle_Bottom = faceData[x][y - 1][z + 1];

	CubeFaceData cBottomRight = faceData[x + 1][y][z + 1];
	CubeFaceData cBottomRight_Bottom = faceData[x + 1][y - 1][z + 1];

	CubeFaceData cRightRight = faceData[x + 1][y][z];
	CubeFaceData cRightRight_Bottom = faceData[x + 1][y - 1][z];

	CubeFaceData cTopRight = faceData[x + 1][y][z - 1];
	CubeFaceData cTopRight_Bottom = faceData[x + 1][y - 1][z - 1];

	CubeFaceData cTopMiddle = faceData[x][y][z - 1];
	CubeFaceData cTopMiddle_Bottom = faceData[x][y - 1][z - 1];

	CubeFaceData cTopLeft = faceData[x - 1][y][z - 1];
	CubeFaceData cTopLeft_Bottom = faceData[x - 1][y - 1][z - 1];

	CubeFaceData cLeftLeft = faceData[x - 1][y][z];
	CubeFaceData cLeftLeft_Bottom = faceData[x - 1][y - 1][z];

	computeAverageHelper(lightValue, cLeftLeft, cLeftLeft_Bottom, cBottomLeft,
			cBottomLeft_Bottom, cBottomMiddle, cBottomMiddle_Bottom, cBottomRight,
			cBottomRight_Bottom, cRightRight, cRightRight_Bottom, cTopRight,
			cTopRight_Bottom, cTopMiddle, cTopMiddle_Bottom, cTopLeft,
			cTopLeft_Bottom, bottomLeft, bottomRight, topRight, topLeft);
}

void GraphicalChunk::computeAverageBack(int lightValue, int x, int y, int z,
		float &bottomLeft, float &bottomRight,
		float &topRight, float &topLeft,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData) {

	// TODO give them correct names

	CubeFaceData cBottomLeft = faceData[x + 1][y - 1][z];
	CubeFaceData cBottomLeft_Back = faceData[x + 1][y - 1][z + 1];

	CubeFaceData cBottomMiddle = faceData[x][y - 1][z];
	CubeFaceData cBottomMiddle_Back = faceData[x][y - 1][z + 1];

	CubeFaceData cBottomRight = faceData[x - 1][y - 1][z];
	CubeFaceData cBottomRight_Back = faceData[x - 1][y - 1][z + 1];

	CubeFaceData cRightRight = faceData[x - 1][y][z];
	CubeFaceData cRightRight_Back = faceData[x - 1][y][z + 1];

	CubeFaceData cTopRight = faceData[x - 1][y + 1][z];
	CubeFaceData cTopRight_Back = faceData[x - 1][y + 1][z + 1];

	CubeFaceData cTopMiddle = faceData[x][y + 1][z];
	CubeFaceData cTopMiddle_Back = faceData[x][y + 1][z + 1];

	CubeFaceData cTopLeft = faceData[x + 1][y + 1][z];
	CubeFaceData cTopLeft_Back = faceData[x + 1][y + 1][z + 1];

	CubeFaceData cLeftLeft = faceData[x + 1][y][z];
	CubeFaceData cLeftLeft_Back = faceData[x + 1][y][z + 1];

	computeAverageHelper(lightValue, cLeftLeft, cLeftLeft_Back, cBottomLeft,
			cBottomLeft_Back, cBottomMiddle, cBottomMiddle_Back, cBottomRight,
			cBottomRight_Back, cRightRight, cRightRight_Back, cTopRight,
			cTopRight_Back, cTopMiddle, cTopMiddle_Back, cTopLeft,
			cTopLeft_Back, bottomLeft, bottomRight, topRight, topLeft);

}

void GraphicalChunk::computeAverageFront(int lightValue, int x, int y, int z,
		float &bottomLeft, float &bottomRight,
		float &topRight, float &topLeft,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData) {

	// TODO give the correct names

	CubeFaceData cBottomLeft = faceData[x - 1][y - 1][z];
	CubeFaceData cBottomLeft_Front = faceData[x - 1][y - 1][z - 1];

	CubeFaceData cBottomMiddle = faceData[x][y - 1][z];
	CubeFaceData cBottomMiddle_Front = faceData[x][y - 1][z - 1];

	CubeFaceData cBottomRight = faceData[x + 1][y - 1][z];
	CubeFaceData cBottomRight_Front = faceData[x + 1][y - 1][z - 1];

	CubeFaceData cRightRight = faceData[x + 1][y][z];
	CubeFaceData cRightRight_Front = faceData[x + 1][y][z - 1];

	CubeFaceData cTopRight = faceData[x + 1][y + 1][z];
	CubeFaceData cTopRight_Front = faceData[x + 1][y + 1][z - 1];

	CubeFaceData cTopMiddle = faceData[x][y + 1][z];
	CubeFaceData cTopMiddle_Front = faceData[x][y + 1][z - 1];

	CubeFaceData cTopLeft = faceData[x - 1][y + 1][z];
	CubeFaceData cTopLeft_Front = faceData[x - 1][y + 1][z - 1];

	CubeFaceData cLeftLeft = faceData[x - 1][y][z];
	CubeFaceData cLeftLeft_Front = faceData[x - 1][y][z - 1];

	computeAverageHelper(lightValue, cLeftLeft, cLeftLeft_Front, cBottomLeft,
			cBottomLeft_Front, cBottomMiddle, cBottomMiddle_Front, cBottomRight,
			cBottomRight_Front, cRightRight, cRightRight_Front, cTopRight,
			cTopRight_Front, cTopMiddle, cTopMiddle_Front, cTopLeft,
			cTopLeft_Front, bottomLeft, bottomRight, topRight, topLeft);

}

}
