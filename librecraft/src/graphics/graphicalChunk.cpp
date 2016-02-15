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
// ### Generate Cubeface Data##################################################
// ############################################################################
	for (int i = 0; i < width + 2; i++) {
		faceData.push_back(vector<vector<CubeFaceData>>());
		for (int j = 0; j < height + 2; j++) {
			faceData[i].push_back(vector<CubeFaceData>());
			for (int k = 0; k < depth + 2; k++) {
				CubeFaceData cube;

				auto *voxel = getVoxel(i - 1, j - 1, k - 1, data, right, left, back, front);
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

//					current.lvRight_BottomLeft = cd.lightValue;
//					current.lvRight_BottomRight = cd.lightValue;
//					current.lvRight_TopRight = cd.lightValue;
//					current.lvRight_TopLeft = cd.lightValue;

					computeAverageRight(cd.lightValue, i + 1, j + 1, k + 1,
							current.lvRight_BottomLeft,
							current.lvRight_BottomRight,
							current.lvRight_TopRight,
							current.lvRight_TopLeft,
							faceData);

//					doAORight(current, i + 1, j + 1, k + 1,faceData);
				}

//				if (voxel) {
//					if (voxel->id != AIR) {
//						current.right = false;
//					} else {
//
//						current.lvRight_BottomLeft = voxel->lightValue;
//						current.lvRight_BottomRight = voxel->lightValue;
//						current.lvRight_TopRight = voxel->lightValue;
//						current.lvRight_TopLeft = voxel->lightValue;
//
//						computeAverageRight(current.lightValue, i + 1, j + 1, k + 1,
//								current.lvRight_BottomLeft,
//								current.lvRight_BottomRight,
//								current.lvRight_TopRight,
//								current.lvRight_TopLeft,
//								faceData);
//
//						doAORight(current, i + 1, j + 1, k + 1,faceData);
//					}
//				}

				cd = faceData[i][j + 1][k + 1];
				if (cd.id != AIR) {
					current.left = false;
				} else {
//					current.lvLeft_BottomLeft = cd.lightValue;
//					current.lvLeft_BottomRight = cd.lightValue;
//					current.lvLeft_TopRight = cd.lightValue;
//					current.lvLeft_TopLeft = cd.lightValue;
//
//					cout << 0 + cd.lightValue << "\n";

					computeAverageLeft(cd.lightValue, i + 1, j + 1, k + 1,
						current.lvLeft_BottomLeft,
						current.lvLeft_BottomRight,
						current.lvLeft_TopRight,
						current.lvLeft_TopLeft,
						faceData);

//					doAOLeft(current, i + 1, j + 1, k + 1, faceData);
				}
//				if (voxel) {
//					if (voxel->id != AIR) {
//						current.left = false;
//					} else {
//						current.lvLeft_BottomLeft = voxel->lightValue;
//						current.lvLeft_BottomRight = voxel->lightValue;
//						current.lvLeft_TopRight = voxel->lightValue;
//						current.lvLeft_TopLeft = voxel->lightValue;
//
//						doAOLeft(current, i + 1, j + 1, k + 1, faceData);
//					}
//				}

				// Y ##########################################################

				cd = faceData[i + 1][j + 2][k + 1];

				//Voxel *voxel = getVoxel(i, j + 1, k, data, right, left, back, front);
				if (cd.id != AIR) {
					current.top = false;
				} else {
//					current.lvTop_BottomLeft = cd.lightValue;
//					current.lvTop_BottomRight = cd.lightValue;
//					current.lvTop_TopRight = cd.lightValue;
//					current.lvTop_TopLeft = cd.lightValue;

					computeAverageTop(cd.lightValue, i + 1, j + 1, k + 1,
						current.lvTop_BottomLeft,
						current.lvTop_BottomRight,
						current.lvTop_TopRight,
						current.lvTop_TopLeft, faceData);

//					doAOTop(current, i + 1, j + 1, k + 1, faceData);
				}
//				if (voxel) {
//					if (voxel->id != AIR) {
//						current.top = false;
//					} else {
//						current.lvTop_BottomLeft = voxel->lightValue;
//						current.lvTop_BottomRight = voxel->lightValue;
//						current.lvTop_TopRight = voxel->lightValue;
//						current.lvTop_TopLeft = voxel->lightValue;
//
//						doAOTop(current, i + 1, j + 1, k + 1, faceData);
//					}
//				}

//				Voxel *voxel = getVoxel(i, j - 1, k, data, right, left, back, front);
				cd = faceData[i + 1][j][k + 1];
				if (cd.id != AIR) {
					current.bottom = false;
				} else {
//					current.lvBottom_BottomLeft = cd.lightValue;
//					current.lvBottom_BottomRight = cd.lightValue;
//					current.lvBottom_TopRight = cd.lightValue;
//					current.lvBottom_TopLeft = cd.lightValue;

					computeAverageBottom(cd.lightValue, i + 1, j + 1, k + 1,
						current.lvBottom_BottomLeft,
						current.lvBottom_BottomRight,
						current.lvBottom_TopRight,
						current.lvBottom_TopLeft, faceData);

//					doAOBottom(current, i + 1, j + 1, k + 1, faceData);
				}

//				Voxel *voxel = getVoxel(i, j - 1, k, data, right, left, back, front);
//				if (voxel) {
//					if (voxel->id != AIR) {
//						current.bottom = false;
//					} else {
//						current.lvBottom_BottomLeft = voxel->lightValue;
//						current.lvBottom_BottomRight = voxel->lightValue;
//						current.lvBottom_TopRight = voxel->lightValue;
//						current.lvBottom_TopLeft = voxel->lightValue;
//
//						doAOBottom(current, i + 1, j + 1, k + 1, faceData);
//					}
//				}

				// Z ##########################################################

//				Voxel *voxel = getVoxel(i, j, k + 1, data, right, left, back, front);

				cd = faceData[i + 1][j + 1][k + 2];

				if (cd.id != AIR) {
					current.back = false;
				} else {
					current.lvBack_BottomLeft = cd.lightValue;
					current.lvBack_BottomRight = cd.lightValue;
					current.lvBack_TopRight = cd.lightValue;
					current.lvBack_TopLeft = cd.lightValue;

//					doAOBack(current, i + 1, j + 1, k + 1, faceData);
				}

//				Voxel *voxel = getVoxel(i, j, k + 1, data, right, left, back, front);
//				if (voxel) {
//					if (voxel->id != AIR) {
//						current.back = false;
//					} else {
//						current.lvBack_BottomLeft = voxel->lightValue;
//						current.lvBack_BottomRight = voxel->lightValue;
//						current.lvBack_TopRight = voxel->lightValue;
//						current.lvBack_TopLeft = voxel->lightValue;
//
//						doAOBack(current, i + 1, j + 1, k + 1, faceData);
//					}
//				}

//				Voxel *voxel = getVoxel(i, j, k - 1, data, right, left, back, front);
				cd = faceData[i + 1][j + 1][k];
					if (cd.id != AIR) {
						current.front = false;
					} else {
						current.lvFront_BottomLeft = cd.lightValue;
						current.lvFront_BottomRight = cd.lightValue;
						current.lvFront_TopRight = cd.lightValue;
						current.lvFront_TopLeft = cd.lightValue;

//						doAOFront(current, i + 1, j + 1, k + 1, faceData);
					}

//				Voxel *voxel = getVoxel(i, j, k - 1, data, right, left, back, front);
//				if (voxel) {
//					if (voxel->id != AIR) {
//						current.front = false;
//					} else {
//						current.lvFront_BottomLeft = voxel->lightValue;
//						current.lvFront_BottomRight = voxel->lightValue;
//						current.lvFront_TopRight = voxel->lightValue;
//						current.lvFront_TopLeft = voxel->lightValue;
//
//						doAOFront(current, i + 1, j + 1, k + 1, faceData);
//					}
//				}
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

					// TODO Dose not follow the same pattern as the others?!?
					// TODO FIX THIS SHIT
					// TODO
					// TODO
					// TODO
					// TODO
					// TODO
					// TODO
					// TODO
					vector<GLfloat> vertex {

						-0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz,
							fd.lvBottom_BottomLeft,

						0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz,
							fd.lvBottom_BottomRight,

						0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz,
							fd.lvBottom_TopRight,

						-0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz,
							fd.lvBottom_TopLeft,
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

int AOFactor = 2;

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

	float counter {1};
	float acc = lightValue;
	if (cLeftLeft.id != AIR && cLeftLeft_Right.id == AIR) {
		acc += cLeftLeft_Right.lightValue;
		++counter;
	}
	if (cBottomLeft.id != AIR && cBottomLeft_Right.id == AIR) {
		acc += cBottomLeft_Right.lightValue;
		++counter;
	}
	if (cBottomMiddle.id != AIR && cBottomMiddle_Right.id == AIR) {
		acc += cBottomMiddle_Right.lightValue;
		++counter;
	}
	bottomLeft = acc / counter;

	counter = 1;
	acc = lightValue;
	if (cBottomMiddle.id != AIR && cBottomMiddle_Right.id == AIR) {
		acc += cBottomMiddle_Right.lightValue;
		++counter;
	}
	if (cBottomRight.id != AIR && cBottomRight_Right.id == AIR) {
		acc += cBottomRight_Right.lightValue;
		++counter;
	}
	if (cRightRight.id != AIR && cRightRight_Right.id == AIR) {
		acc += cRightRight_Right.lightValue;
		++counter;
	}
	bottomRight = acc / counter;

	counter = 1;
	acc = lightValue;
	if (cRightRight.id != AIR && cRightRight.id == AIR) {
		acc += cRightRight_Right.lightValue;
		++counter;
	}
	if (cTopRight.id != AIR && cTopRight_Right.id == AIR) {
		acc += cTopRight_Right.lightValue;
		++counter;
	}
	topRight = acc / counter;

	counter = 1;
	acc = lightValue;
	if (cTopMiddle.id != AIR && cTopMiddle_Right.id == AIR) {
		acc += cTopMiddle_Right.lightValue;
		++counter;
	}
	if (cTopLeft.id != AIR && cTopLeft_Right.id == AIR) {
		acc += cTopLeft_Right.lightValue;
		++counter;
	}
	if (cLeftLeft.id != AIR && cLeftLeft_Right.id == AIR) {
		acc += cLeftLeft_Right.lightValue;
		++counter;
	}
	topLeft = acc / counter;

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

	float counter {1};
	float acc = lightValue;
	if (cLeftLeft.id != AIR && cLeftLeft_Left.id == AIR) {
		acc += cLeftLeft_Left.lightValue;
		++counter;
	}
	if (cBottomLeft.id != AIR && cBottomLeft_Left.id == AIR) {
		acc += cBottomLeft_Left.lightValue;
		++counter;
	}
	if (cBottomMiddle.id != AIR && cBottomMiddle_Left.id == AIR) {
		acc += cBottomMiddle_Left.lightValue;
		++counter;
	}
	bottomLeft = acc / counter;

	counter = 1;
	acc = lightValue;
	if (cBottomMiddle.id != AIR && cBottomMiddle_Left.id == AIR) {
		acc += cBottomMiddle_Left.lightValue;
		++counter;
	}
	if (cBottomRight.id != AIR && cBottomRight_Left.id == AIR) {
		acc += cBottomRight_Left.lightValue;
		++counter;
	}
	if (cRightRight.id != AIR && cRightRight_Left.id == AIR) {
		acc += cRightRight_Left.lightValue;
		++counter;
	}
	bottomRight = acc / counter;

	counter = 1;
	acc = lightValue;
	if (cRightRight.id != AIR && cRightRight.id == AIR) {
		acc += cRightRight_Left.lightValue;
		++counter;
	}
	if (cTopRight.id != AIR && cTopRight_Left.id == AIR) {
		acc += cTopRight_Left.lightValue;
		++counter;
	}
	if (cTopMiddle.id != AIR && cTopMiddle_Left.id == AIR) {
		acc += cTopMiddle_Left.lightValue;
		++counter;
	}
	topRight = acc / counter;

	counter = 1;
	acc = lightValue;
	if (cTopMiddle.id != AIR && cTopMiddle_Left.id == AIR) {
		acc += cTopMiddle_Left.lightValue;
		++counter;
	}
	if (cTopLeft.id != AIR && cTopLeft_Left.id == AIR) {
		acc += cTopLeft_Left.lightValue;
		++counter;
	}
	if (cLeftLeft.id != AIR && cLeftLeft_Left.id == AIR) {
		acc += cLeftLeft_Left.lightValue;
		++counter;
	}
	topLeft = acc / counter;
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

	float counter {1};
	float acc = lightValue;
	if (cLeftLeft.id != AIR && cLeftLeft_Top.id == AIR) {
		acc += cLeftLeft_Top.lightValue;
		++counter;
	}
	if (cBottomLeft.id != AIR && cBottomLeft_Top.id == AIR) {
		acc += cBottomLeft_Top.lightValue;
		++counter;
	}
	if (cBottomMiddle.id != AIR && cBottomMiddle_Top.id == AIR) {
		acc += cBottomMiddle_Top.lightValue;
		++counter;
	}
	bottomLeft = acc / counter;

	counter = 1;
	acc = lightValue;
	if (cBottomMiddle.id != AIR && cBottomMiddle_Top.id == AIR) {
		acc += cBottomMiddle_Top.lightValue;
		++counter;
	}
	if (cBottomRight.id != AIR && cBottomRight_Top.id == AIR) {
		acc += cBottomRight_Top.lightValue;
		++counter;
	}
	if (cRightRight.id != AIR && cRightRight_Top.id == AIR) {
		acc += cRightRight_Top.lightValue;
		++counter;
	}
	bottomRight = acc / counter;

	counter = 1;
	acc = lightValue;
	if (cRightRight.id != AIR && cRightRight.id == AIR) {
		acc += cRightRight_Top.lightValue;
		++counter;
	}
	if (cTopRight.id != AIR && cTopRight_Top.id == AIR) {
		acc += cTopRight_Top.lightValue;
		++counter;
	}
	topRight = acc / counter;

	counter = 1;
	acc = lightValue;
	if (cTopMiddle.id != AIR && cTopMiddle_Top.id == AIR) {
		acc += cTopMiddle_Top.lightValue;
		++counter;
	}
	if (cTopLeft.id != AIR && cTopLeft_Top.id == AIR) {
		acc += cTopLeft_Top.lightValue;
		++counter;
	}
	if (cLeftLeft.id != AIR && cLeftLeft_Top.id == AIR) {
		acc += cLeftLeft_Top.lightValue;
		++counter;
	}
	topLeft = acc / counter;

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

//	bottomLeft = lightValue;
//	bottomRight = lightValue;
//	topRight = lightValue;
//	topLeft = lightValue;


	float counter {1};
	float acc = lightValue;
	if (cLeftLeft.id != AIR && cLeftLeft_Bottom.id == AIR) {
		acc += cLeftLeft_Bottom.lightValue;
		++counter;
	}
	if (cBottomLeft.id != AIR && cBottomLeft_Bottom.id == AIR) {
		acc += cBottomLeft_Bottom.lightValue;
		++counter;
	}
	if (cBottomMiddle.id != AIR && cBottomMiddle_Bottom.id == AIR) {
		acc += cBottomMiddle_Bottom.lightValue;
		++counter;
	}
	bottomLeft = acc / counter;

	counter = 1;
	acc = lightValue;
	if (cBottomMiddle.id != AIR && cBottomMiddle_Bottom.id == AIR) {
		acc += cBottomMiddle_Bottom.lightValue;
		++counter;
	}
	if (cBottomRight.id != AIR && cBottomRight_Bottom.id == AIR) {
		acc += cBottomRight_Bottom.lightValue;
		++counter;
	}
	if (cRightRight.id != AIR && cRightRight_Bottom.id == AIR) {
		acc += cRightRight_Bottom.lightValue;
		++counter;
	}
	bottomRight = acc / counter;

	counter = 1;
	acc = lightValue;
	if (cRightRight.id != AIR && cRightRight.id == AIR) {
		acc += cRightRight_Bottom.lightValue;
		++counter;
	}
	if (cTopRight.id != AIR && cTopRight_Bottom.id == AIR) {
		acc += cTopRight_Bottom.lightValue;
		++counter;
	}
	topRight = acc / counter;

	counter = 1;
	acc = lightValue;
	if (cTopMiddle.id != AIR && cTopMiddle_Bottom.id == AIR) {
		acc += cTopMiddle_Bottom.lightValue;
		++counter;
	}
	if (cTopLeft.id != AIR && cTopLeft_Bottom.id == AIR) {
		acc += cTopLeft_Bottom.lightValue;
		++counter;
	}
	if (cLeftLeft.id != AIR && cLeftLeft_Bottom.id == AIR) {
		acc += cLeftLeft_Bottom.lightValue;
		++counter;
	}
	topLeft = acc / counter;

}

void GraphicalChunk::computeAverageBack(int lightValue, int x, int y, int z,
		float &bottomLeft, float &bottomRight,
		float &topRight, float &topLeft,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData) {
}

void GraphicalChunk::computeAverageFront(int lightValue, int x, int y, int z,
		float &bottomLeft, float &bottomRight,
		float &topRight, float &topLeft,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData) {
}

}


//
//void GraphicalChunk::doAORight(CubeFaceData &cf, int x, int y, int z,
//vector<vector<vector<Voxel>>>&data,
//vector<vector<vector<Voxel>>> *right,
//vector<vector<vector<Voxel>>> *left,
//vector<vector<vector<Voxel>>> *back,
//vector<vector<vector<Voxel>>> *front) {
//
//	char bottomLeft = 0;
//	Voxel *v = getVoxel(x + 1, y - 1, z, data, right, left, back, front);
//	if (v && v->id != AIR) bottomLeft++;
//	v = getVoxel(x + 1, y, z - 1, data, right, left, back, front);
//	if (v && v->id != AIR) bottomLeft++;
//	v = getVoxel(x + 1, y - 1, z - 1, data, right, left, back, front);
//	if (v && v->id != AIR) bottomLeft++;
//	cf.lvRight_BottomLeft -= bottomLeft * AOFactor;
//
//	char bottomRight = 0;
//	v = getVoxel(x + 1, y - 1, z, data, right, left, back, front);
//	if (v && v->id != AIR) bottomRight++;
//	v = getVoxel(x + 1, y, z + 1, data, right, left, back, front);
//	if (v && v->id != AIR) bottomRight++;
//	v = getVoxel(x + 1, y - 1, z + 1, data, right, left, back, front);
//	if (v && v->id != AIR) bottomRight++;
//	cf.lvRight_BottomRight -= bottomRight * AOFactor;
//
//	char topRight = 0;
//	v = getVoxel(x + 1, y + 1, z, data, right, left, back, front);
//	if (v && v->id != AIR) topRight++;
//	v = getVoxel(x + 1, y, z + 1, data, right, left, back, front);
//	if (v && v->id != AIR) topRight++;
//	v = getVoxel(x + 1, y + 1, z + 1, data, right, left, back, front);
//	if (v && v->id != AIR) topRight++;
//	cf.lvRight_TopRight -= topRight * AOFactor;
//
//	char topLeft = 0;
//	v = getVoxel(x + 1, y + 1, z, data, right, left, back, front);
//	if (v && v->id != AIR) topLeft++;
//	v = getVoxel(x + 1, y, z - 1, data, right, left, back, front);
//	if (v && v->id != AIR) topLeft++;
//	v = getVoxel(x + 1, y + 1, z - 1, data, right, left, back, front);
//	if (v && v->id != AIR) topLeft++;
//	cf.lvRight_TopLeft -= topLeft * AOFactor;
//}
//
//void GraphicalChunk::doAOLeft(CubeFaceData &cf, int x, int y, int z,
//vector<vector<vector<Voxel>>>&data,
//vector<vector<vector<Voxel>>> *right,
//vector<vector<vector<Voxel>>> *left,
//vector<vector<vector<Voxel>>> *back,
//vector<vector<vector<Voxel>>> *front) {
//
//	char bottomLeft = 0;
//	Voxel *v = getVoxel(x - 1, y - 1, z, data, right, left, back, front);
//	if (v && v->id != AIR) bottomLeft++;
//	v = getVoxel(x - 1, y, z + 1, data, right, left, back, front);
//	if (v && v->id != AIR) bottomLeft++;
//	v = getVoxel(x - 1, y - 1, z + 1, data, right, left, back, front);
//	if (v && v->id != AIR) bottomLeft++;
//	cf.lvLeft_BottomLeft -= bottomLeft * AOFactor;
//
//	char bottomRight = 0;
//	v = getVoxel(x - 1, y - 1, z, data, right, left, back, front);
//	if (v && v->id != AIR) bottomRight++;
//	v = getVoxel(x - 1, y, z - 1, data, right, left, back, front);
//	if (v && v->id != AIR) bottomRight++;
//	v = getVoxel(x - 1, y - 1, z - 1, data, right, left, back, front);
//	if (v && v->id != AIR) bottomRight++;
//	cf.lvLeft_BottomRight -= bottomRight * AOFactor;
//
//	char topRight = 0;
//	v = getVoxel(x - 1, y + 1, z, data, right, left, back, front);
//	if (v && v->id != AIR) topRight++;
//	v = getVoxel(x - 1, y, z + 1, data, right, left, back, front);
//	if (v && v->id != AIR) topRight++;
//	v = getVoxel(x - 1, y + 1, z + 1, data, right, left, back, front);
//	if (v && v->id != AIR) topRight++;
//	cf.lvLeft_TopLeft -= topRight * AOFactor;
//
//	char topLeft = 0;
//	v = getVoxel(x - 1, y + 1, z, data, right, left, back, front);
//	if (v && v->id != AIR) topLeft++;
//	v = getVoxel(x - 1, y, z - 1, data, right, left, back, front);
//	if (v && v->id != AIR) topLeft++;
//	v = getVoxel(x - 1, y + 1, z - 1, data, right, left, back, front);
//	if (v && v->id != AIR) topLeft++;
//	cf.lvLeft_TopRight -= topLeft * AOFactor;
//}
//
//void GraphicalChunk::doAOBack(CubeFaceData &cf, int x, int y, int z,
//vector<vector<vector<Voxel>>>&data,
//vector<vector<vector<Voxel>>> *right,
//vector<vector<vector<Voxel>>> *left,
//vector<vector<vector<Voxel>>> *back,
//vector<vector<vector<Voxel>>> *front) {
//
//	char bottomLeft = 0;
//	Voxel *v = getVoxel(x, y - 1, z + 1, data, right, left, back, front);
//	if (v && v->id != AIR) bottomLeft++;
//	v = getVoxel(x + 1, y, z + 1, data, right, left, back, front);
//	if (v && v->id != AIR) bottomLeft++;
//	v = getVoxel(x + 1, y - 1, z + 1, data, right, left, back, front);
//	if (v && v->id != AIR) bottomLeft++;
//	cf.lvBack_BottomLeft -= bottomLeft * AOFactor;
//
//	char bottomRight = 0;
//	v = getVoxel(x, y - 1, z + 1, data, right, left, back, front);
//	if (v && v->id != AIR) bottomRight++;
//	v = getVoxel(x - 1, y, z + 1, data, right, left, back, front);
//	if (v && v->id != AIR) bottomRight++;
//	v = getVoxel(x - 1, y - 1, z + 1, data, right, left, back, front);
//	if (v && v->id != AIR) bottomRight++;
//	cf.lvBack_BottomRight -= bottomRight * AOFactor;
//
//	char topRight = 0;
//	v = getVoxel(x, y + 1, z + 1, data, right, left, back, front);
//	if (v && v->id != AIR) topRight++;
//	v = getVoxel(x - 1, y, z + 1, data, right, left, back, front);
//	if (v && v->id != AIR) topRight++;
//	v = getVoxel(x - 1, y + 1, z + 1, data, right, left, back, front);
//	if (v && v->id != AIR) topRight++;
//	cf.lvBack_TopRight -= topRight * AOFactor;
//
//	char topLeft = 0;
//	v = getVoxel(x, y + 1, z + 1, data, right, left, back, front);
//	if (v && v->id != AIR) topLeft++;
//	v = getVoxel(x + 1, y, z + 1, data, right, left, back, front);
//	if (v && v->id != AIR) topLeft++;
//	v = getVoxel(x + 1, y + 1, z + 1, data, right, left, back, front);
//	if (v && v->id != AIR) topLeft++;
//	cf.lvBack_TopLeft -= topLeft * AOFactor;
//}
//
//void GraphicalChunk::doAOFront(CubeFaceData &cf, int x, int y, int z,
//		vector<vector<vector<Voxel>>>&data,
//vector<vector<vector<Voxel>>> *right,
//vector<vector<vector<Voxel>>> *left,
//vector<vector<vector<Voxel>>> *back,
//vector<vector<vector<Voxel>>> *front) {
//
//	char bottomLeft = 0;
//	Voxel *v = getVoxel(x, y - 1, z - 1, data, right, left, back, front);
//	if (v && v->id != AIR) bottomLeft++;
//	v = getVoxel(x - 1, y, z - 1, data, right, left, back, front);
//	if (v && v->id != AIR) bottomLeft++;
//	v = getVoxel(x - 1, y - 1, z - 1, data, right, left, back, front);
//	if (v && v->id != AIR) bottomLeft++;
//	cf.lvFront_BottomLeft -= bottomLeft * AOFactor;
//
//	char bottomRight = 0;
//	v = getVoxel(x, y - 1, z - 1, data, right, left, back, front);
//	if (v && v->id != AIR) bottomRight++;
//	v = getVoxel(x + 1, y, z - 1, data, right, left, back, front);
//	if (v && v->id != AIR) bottomRight++;
//	v = getVoxel(x + 1, y - 1, z - 1, data, right, left, back, front);
//	if (v && v->id != AIR) bottomRight++;
//	cf.lvFront_BottomRight -= bottomRight * AOFactor;
//
//	char topRight = 0;
//	v = getVoxel(x, y + 1, z - 1, data, right, left, back, front);
//	if (v && v->id != AIR) topRight++;
//	v = getVoxel(x + 1, y, z-+ 1, data, right, left, back, front);
//	if (v && v->id != AIR) topRight++;
//	v = getVoxel(x + 1, y + 1, z - 1, data, right, left, back, front);
//	if (v && v->id != AIR) topRight++;
//	cf.lvFront_TopRight -= topRight * AOFactor;
//
//	char topLeft = 0;
//	v = getVoxel(x, y + 1, z - 1, data, right, left, back, front);
//	if (v && v->id != AIR) topLeft++;
//	v = getVoxel(x - 1, y, z - 1, data, right, left, back, front);
//	if (v && v->id != AIR) topLeft++;
//	v = getVoxel(x - 1, y + 1, z - 1, data, right, left, back, front);
//	if (v && v->id != AIR) topLeft++;
//	cf.lvFront_TopLeft -= topLeft * AOFactor;
//}
//
//void GraphicalChunk::doAOTop(CubeFaceData &cf, int x, int y, int z,
//		vector<vector<vector<Voxel>>>&data,
//vector<vector<vector<Voxel>>> *right,
//vector<vector<vector<Voxel>>> *left,
//vector<vector<vector<Voxel>>> *back,
//vector<vector<vector<Voxel>>> *front) {
//
//	char bottomLeft = 0;
//	Voxel *v = getVoxel(x, y + 1, z - 1, data, right, left, back, front);
//	if (v && v->id != AIR) bottomLeft++;
//	v = getVoxel(x - 1, y + 1, z, data, right, left, back, front);
//	if (v && v->id != AIR) bottomLeft++;
//	v = getVoxel(x - 1, y + 1, z - 1, data, right, left, back, front);
//	if (v && v->id != AIR) bottomLeft++;
//	cf.lvTop_BottomLeft -= bottomLeft * AOFactor;
//
//	char bottomRight = 0;
//	v = getVoxel(x, y + 1, z - 1, data, right, left, back, front);
//	if (v && v->id != AIR) bottomRight++;
//	v = getVoxel(x + 1, y + 1, z, data, right, left, back, front);
//	if (v && v->id != AIR) bottomRight++;
//	v = getVoxel(x + 1, y + 1, z - 1, data, right, left, back, front);
//	if (v && v->id != AIR) bottomRight++;
//	cf.lvTop_BottomRight-= bottomRight * AOFactor;
//
//	char topRight = 0;
//	v = getVoxel(x, y + 1, z + 1, data, right, left, back, front);
//	if (v && v->id != AIR) topRight++;
//	v = getVoxel(x + 1, y + 1, z, data, right, left, back, front);
//	if (v && v->id != AIR) topRight++;
//	v = getVoxel(x + 1, y + 1, z + 1, data, right, left, back, front);
//	if (v && v->id != AIR) topRight++;
//	cf.lvTop_TopRight -= topRight * AOFactor;
//
//	char topLeft = 0;
//	v = getVoxel(x, y + 1, z + 1, data, right, left, back, front);
//	if (v && v->id != AIR) topLeft++;
//	v = getVoxel(x - 1, y + 1, z, data, right, left, back, front);
//	if (v && v->id != AIR) topLeft++;
//	v = getVoxel(x - 1, y + 1, z + 1, data, right, left, back, front);
//	if (v && v->id != AIR) topLeft++;
//	cf.lvTop_TopLeft -= topLeft * AOFactor;
//}
//
//void GraphicalChunk::doAOBottom(CubeFaceData &cf, int x, int y, int z,
//vector<vector<vector<Voxel>>>&data,
//vector<vector<vector<Voxel>>> *right,
//vector<vector<vector<Voxel>>> *left,
//vector<vector<vector<Voxel>>> *back,
//vector<vector<vector<Voxel>>> *front) {
//
//	char bottomLeft = 0;
//	Voxel *v = getVoxel(x, y - 1, z - 1, data, right, left, back, front);
//	if (v && v->id != AIR) bottomLeft++;
//	v = getVoxel(x - 1, y - 1, z, data, right, left, back, front);
//	if (v && v->id != AIR) bottomLeft++;
//	v = getVoxel(x - 1, y - 1, z - 1, data, right, left, back, front);
//	if (v && v->id != AIR) bottomLeft++;
//	cf.lvBottom_BottomLeft -= bottomLeft * AOFactor;
//
//	char bottomRight = 0;
//	v = getVoxel(x, y - 1, z - 1, data, right, left, back, front);
//	if (v && v->id != AIR) bottomRight++;
//	v = getVoxel(x + 1, y - 1, z, data, right, left, back, front);
//	if (v && v->id != AIR) bottomRight++;
//	v = getVoxel(x + 1, y - 1, z - 1, data, right, left, back, front);
//	if (v && v->id != AIR) bottomRight++;
//	cf.lvBottom_BottomRight-= bottomRight * AOFactor;
//
//	char topRight = 0;
//	v = getVoxel(x, y - 1, z + 1, data, right, left, back, front);
//	if (v && v->id != AIR) topRight++;
//	v = getVoxel(x + 1, y - 1, z, data, right, left, back, front);
//	if (v && v->id != AIR) topRight++;
//	v = getVoxel(x + 1, y - 1, z + 1, data, right, left, back, front);
//	if (v && v->id != AIR) topRight++;
//	cf.lvBottom_TopRight -= topRight * AOFactor;
//
//	char topLeft = 0;
//	v = getVoxel(x, y - 1, z + 1, data, right, left, back, front);
//	if (v && v->id != AIR) topLeft++;
//	v = getVoxel(x - 1, y - 1, z, data, right, left, back, front);
//	if (v && v->id != AIR) topLeft++;
//	v = getVoxel(x - 1, y - 1, z + 1, data, right, left, back, front);
//	if (v && v->id != AIR) topLeft++;
//	cf.lvBottom_TopLeft -= topLeft * AOFactor;
//}
//
//void GraphicalChunk::computeAverageRight(int x, int y, int z, float &bottomLeft,
//	float &bottomRight, float &topRight, float &topLeft,
//	vector<vector<vector<Voxel>>> &data,
//	vector<vector<vector<Voxel>>> *right,
//	vector<vector<vector<Voxel>>> *left,
//	vector<vector<vector<Voxel>>> *back,
//	vector<vector<vector<Voxel>>> *front,
//	vector<vector<vector<CubeFaceData>>> &faceData) {
//
//	Voxel *cBottomLeft {getVoxel(x + 1, y - 1, z - 1, data, right, left, back, front)};
//	Voxel *cBottomMiddle {getVoxel(x + 1, y - 1, z, data, right, left, back, front)};
//	Voxel *cBottomRight {getVoxel(x + 1, y - 1, z + 1, data, right, left, back, front)};
//
//	Voxel *cRightRight {getVoxel(x + 1, y, z + 1, data, right, left, back, front)};
//
//	Voxel *cTopRight {getVoxel(x + 1, y + 1, z + 1, data, right, left, back, front)};
//	Voxel *cTopMiddle {getVoxel(x + 1, y + 1, z, data, right, left, back, front)};
//	Voxel *cTopLeft {getVoxel(x + 1, y + 1, z - 1, data, right, left, back, front)};
//
//	Voxel *cLeftLeft {getVoxel(x + 1, y, z, data, right, left, back, front)};;
//
//	float counter {0};
//
//	float accBottomLeft {0};
//	if (cLeftLeft) {
//		accBottomLeft += faceData[x + 1][y][z].lightValue;
//		++counter;
//	}
//	if (cBottomLeft) {
//		accBottomLeft += faceData[x + 1][y - 1][z].lightValue;
//		++counter;
//	}
//
//
//}
