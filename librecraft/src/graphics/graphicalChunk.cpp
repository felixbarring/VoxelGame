#include "graphicalChunk.h"

#include <vector>
#include <iostream>
#include <algorithm>

#include "texturedCube.h"
#include "transform.h"

using namespace std;
using namespace config::cube_data;

namespace graphics {

// ############################################################################
// Constructor/Destructor #####################################################
// ############################################################################

GraphicalChunk::GraphicalChunk(float _x, float _y, float _z,
	vector<vector<vector<Voxel>>> &data,
	vector<vector<vector<Voxel>>> *right,
	vector<vector<vector<Voxel>>> *left,
	vector<vector<vector<Voxel>>> *back,
	vector<vector<vector<Voxel>>> *front):

		m_xLocation {_x},
		m_yLocation {_y},
		m_zLocation {_z},
		transform {
			_x + m_width / 2 + 0.5f,
			_y + m_height / 2 + 0.5f,
			_z + m_depth / 2 + 0.5f
		}
{

// ############################################################################
// ### Generate Cubeface Data #################################################
// ############################################################################

	// The face data will be one bigger in each direction.
	// This is to make it possible to compare voxels with
	// neighbors in other chunks.
	for (int x = 0; x < m_width + 2; ++x) {
		m_faceData.push_back(vector<vector<CubeFaceData>>());
		for (int y = 0; y < m_height + 2; ++y) {
			m_faceData[x].push_back(vector<CubeFaceData>());
			for (int z = 0; z < m_depth + 2; z++) {
				CubeFaceData cube;

				auto *voxel = getVoxel(x - 1, y - 1 + m_yLocation, z - 1, data, right, left, back, front);
				if (voxel) {
					cube.id = voxel->id;
					cube.lightValue = voxel->lightValue;
				} else {
					cube.id = AIR;
					cube.lightValue = 0;
				}

				cube.front = false;
				cube.back = false;
				cube.left = false;
				cube.right = false;
				cube.top = false;
				cube.bottom = false;

				m_faceData[x][y].push_back(cube);
			}
		}
	}

	// Remove faces and compute lightning
	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_height; y++) {
			for (int k = 0; k < m_depth; k++) {

				CubeFaceData &current = m_faceData[x + 1][y + 1][k + 1];
				if (current.id == AIR)
					continue;

				if (current.id == WATER) {
					if (m_faceData[x + 1][y + 2][k + 1].id == AIR) {
						current.top = true;
						current.lvTop_BottomLeft = 5;
						current.lvTop_BottomRight = 5;
						current.lvTop_TopRight = 5;
						current.lvTop_TopLeft = 5;
					}
					continue;
				}

// X ##########################################################################
				CubeFaceData cd;
				cd = m_faceData[x + 2][y + 1][k + 1];
				if (cd.id == AIR || cd.id == WATER) {

					current.right = true;

//					current.lvRight_BottomLeft = cd.lightValue;
//					current.lvRight_BottomRight = cd.lightValue;
//					current.lvRight_TopRight = cd.lightValue;
//					current.lvRight_TopLeft = cd.lightValue;

					computeAverageRight(cd.lightValue, x + 1, y + 1, k + 1,
						current.lvRight_BottomLeft,
						current.lvRight_BottomRight,
						current.lvRight_TopRight,
						current.lvRight_TopLeft,
						m_faceData);

					doAORight(current, x + 1, y + 1, k + 1,m_faceData);
				}

				cd = m_faceData[x][y + 1][k + 1];
				if (cd.id == AIR || cd.id == WATER) {
					current.left = true;

//					current.lvLeft_BottomLeft = cd.lightValue;
//					current.lvLeft_BottomRight = cd.lightValue;
//					current.lvLeft_TopRight = cd.lightValue;
//					current.lvLeft_TopLeft = cd.lightValue;

					computeAverageLeft(cd.lightValue, x + 1, y + 1, k + 1,
						current.lvLeft_BottomLeft,
						current.lvLeft_BottomRight,
						current.lvLeft_TopRight,
						current.lvLeft_TopLeft,
						m_faceData);

					doAOLeft(current, x + 1, y + 1, k + 1, m_faceData);
				}

// Z ##########################################################################

				cd = m_faceData[x + 1][y + 1][k + 2];
				if (cd.id == AIR || cd.id == WATER) {
					current.front = true;

//					current.lvFront_BottomLeft = cd.lightValue;
//					current.lvFront_BottomRight = cd.lightValue;
//					current.lvFront_TopRight = cd.lightValue;
//					current.lvFront_TopLeft = cd.lightValue;

					computeAverageFront(cd.lightValue, x + 1, y + 1, k + 1,
						current.lvFront_BottomLeft,
						current.lvFront_BottomRight,
						current.lvFront_TopRight,
						current.lvFront_TopLeft, m_faceData);

					doAOFront(current, x + 1, y + 1, k + 1, m_faceData);
				}

				cd = m_faceData[x + 1][y + 1][k];
				if (cd.id == AIR || cd.id == WATER) {
					current.back = true;

//					current.lvBack_BottomLeft = cd.lightValue;
//					current.lvBack_BottomRight = cd.lightValue;
//					current.lvBack_TopRight = cd.lightValue;
//					current.lvBack_TopLeft = cd.lightValue;

					computeAverageBack(cd.lightValue, x + 1, y + 1, k + 1,
							current.lvBack_BottomLeft,
							current.lvBack_BottomRight,
							current.lvBack_TopRight,
							current.lvBack_TopLeft, m_faceData);

					doAOBack(current, x + 1, y + 1, k + 1, m_faceData);
				}
// Y ##########################################################################

				cd = m_faceData[x + 1][y + 2][k + 1];
				if (cd.id == AIR || cd.id == WATER) {
					current.top = true;

//					current.lvTop_BottomLeft = cd.lightValue;
//					current.lvTop_BottomRight = cd.lightValue;
//					current.lvTop_TopRight = cd.lightValue;
//					current.lvTop_TopLeft = cd.lightValue;

					computeAverageTop(cd.lightValue, x + 1, y + 1, k + 1,
						current.lvTop_BottomLeft,
						current.lvTop_BottomRight,
						current.lvTop_TopRight,
						current.lvTop_TopLeft, m_faceData);

					doAOTop(current, x + 1, y + 1, k + 1, m_faceData);
				}

				cd = m_faceData[x + 1][y][k + 1];
				if (cd.id == AIR || cd.id == WATER) {
					current.bottom = true;

//					current.lvBottom_BottomLeft = cd.lightValue;
//					current.lvBottom_BottomRight = cd.lightValue;
//					current.lvBottom_TopRight = cd.lightValue;
//					current.lvBottom_TopLeft = cd.lightValue;

					computeAverageBottom(cd.lightValue, x + 1, y + 1, k + 1,
						current.lvBottom_BottomLeft,
						current.lvBottom_BottomRight,
						current.lvBottom_TopRight,
						current.lvBottom_TopLeft, m_faceData);

					doAOBottom(current, x + 1, y + 1, k + 1, m_faceData);
				}

			}
		}
	}

}

// ########################################################
// Member Functions########################################
// ########################################################

void GraphicalChunk::draw() {

	// This should be run by the main thread
	// If there was another thread creating the data, we lazily
	// upload the data to opengl since only 1 thread is allowed to
	// do that.
	if (!m_mesh) {
		vector<GLfloat> vertexData;
		vector<GLfloat> normals;
		vector<GLfloat> UV;
		vector<short> elementData;

		createMeshData(false, m_faceData, vertexData, normals, UV, elementData);
		m_mesh.reset(new mesh::MeshElement(vertexData, 4, normals, 3, UV, 3, elementData));

		vertexData.clear();
		normals.clear();
		UV.clear();
		elementData.clear();

		createMeshData(true, m_faceData, vertexData, normals, UV, elementData);
		m_waterMesh.reset(new mesh::MeshElement(vertexData, 4, normals, 3, UV, 3, elementData));

		m_faceData.clear();
	}
	m_mesh->draw();
	m_waterMesh->draw();
}

Transform& GraphicalChunk::getTransform() {
	return transform;
}

float GraphicalChunk::getxLocation() {
	return m_xLocation;
}

float GraphicalChunk::getyLocation() {
	return m_yLocation;
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

	if (y >= config::chunk_data::CHUNK_HEIGHT || y < 0)
		return nullptr;

	if (x < m_width && x >= 0 && z < m_depth && z >= 0) {
		return &data[x][y][z];
	} else if (x == m_width && (z < m_depth && z >= 0)) {
		if (right != nullptr) {
			return &((*right)[0][y][z]);
		} else {
			return nullptr;
		}
	} else if (x == -1 && (z < m_depth && z >= 0)) {
		if (left != nullptr) {
			return &((*left)[m_width - 1][y][z]);
		} else {
			return nullptr;
		}
	} else if (z == m_depth && (x < m_width && x >= 0)) {
		if (back != nullptr) {
			return &((*back)[x][y][0]);
		} else {
			return nullptr;
		}
	} else if (z == -1 && (x < m_width && x >= 0)) {
		if (front != nullptr) {
			return &((*front)[x][y][m_depth - 1]);
		} else {
			return nullptr;
		}
	}
	return nullptr;
}

void GraphicalChunk::createMeshData(
		bool transparent,
		const vector<vector<vector<CubeFaceData>>> &faceData,
		vector<GLfloat> &vertexData,
		vector<GLfloat> &normals,
		vector<GLfloat> &UV,
		vector<short> &elementData) {

	short elementOffset = 0;
	int totalNumberOfFaces = 0;

	float dx = -m_width / 2;
	float dy = -m_height / 2;
	float dz = -m_depth / 2;

	for (int i = 0; i < m_width; i++) {
		for (int j = 0; j < m_height; j++) {
			for (int k = 0; k < m_depth; k++) {

				CubeFaceData fd = faceData[i + 1][j + 1][k + 1];

				int id = fd.id;
				if (id == AIR || (id == WATER && !transparent) || (id != WATER && transparent))
					continue;

				GLfloat sideTexture = BLOCK_TEXTURES[id][SIDE_TEXTURE];
				GLfloat topTexture = BLOCK_TEXTURES[id][TOP_TEXTURE];
				GLfloat bottomTexture = BLOCK_TEXTURES[id][BOTTOM_TEXTURE];

				if (fd.right) {

					vector<GLfloat> vertex {
						0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz,
							fd.lvRight_BottomLeft,

						0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz,
							fd.lvRight_BottomRight,

						0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz,
							fd.lvRight_TopRight,

						0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz,
							fd.lvRight_TopLeft,
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
						-0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz,
							fd.lvLeft_BottomLeft,
						-0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz,
							fd.lvLeft_BottomRight,
						-0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz,
							fd.lvLeft_TopRight ,
						-0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz,
							fd.lvLeft_TopLeft,
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
						0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz,
							fd.lvBack_BottomLeft,
						-0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz,
							fd.lvBack_BottomRight,
						-0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz,
							fd.lvBack_TopRight,
						0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz,
							fd.lvBack_TopLeft,
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
						-0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz,
							fd.lvFront_BottomLeft,
						0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz,
							fd.lvFront_BottomRight,
						0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz,
							fd.lvFront_TopRight,
						-0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz,
							fd.lvFront_TopLeft,
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
							fd.lvTop_BottomLeft,
						0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz,
							fd.lvTop_BottomRight,
						0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz,
							fd.lvTop_TopRight,
						-0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz,
							fd.lvTop_TopLeft,
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
}

int AOFactor = 1;

void GraphicalChunk::doAORight(CubeFaceData &cf, int x, int y, int z,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData) {

	CubeFaceData v;

	float bottomLeft = 0;
	v = faceData[x + 1][y][z + 1];
	if (v.id != AIR) ++bottomLeft;
	v = faceData[x + 1][y - 1][z + 1];
	if (v.id != AIR) ++bottomLeft;
	v = faceData[x + 1][y - 1][z];
	if (v.id != AIR) ++bottomLeft;
	cf.lvRight_BottomLeft -= min<float>(2.0, bottomLeft) * AOFactor;

	float bottomRight = 0;
	v = faceData[x + 1][y - 1][z];
	if (v.id != AIR) ++bottomRight;
	v = faceData[x + 1][y - 1][z - 1];
	if (v.id != AIR) ++bottomRight;
	v = faceData[x + 1][y][z - 1];
	if ( v.id != AIR) ++bottomRight;
	cf.lvRight_BottomRight -= min<float>(2.0, bottomRight) * AOFactor;

	float topRight = 0;
	v = faceData[x + 1][y][z - 1];
	if (v.id != AIR) ++topRight;
	v = faceData[x + 1][y + 1][z - 1];
	if (v.id != AIR) ++topRight;
	v = faceData[x + 1][y + 1][z];
	if (v.id != AIR) ++topRight;
	cf.lvRight_TopRight -= min<float>(2.0, topRight) * AOFactor;

	float topLeft = 0;
	v = faceData[x + 1][y + 1][z];
	if (v.id != AIR) ++topLeft;
	v = faceData[x + 1][y + 1][z + 1];
	if (v.id != AIR) ++topLeft;
	v = faceData[x + 1][y][z + 1];
	if (v.id != AIR) ++topLeft;
	cf.lvRight_TopLeft -= min<float>(2.0, topLeft) * AOFactor;
}

void GraphicalChunk::doAOLeft(CubeFaceData &cf, int x, int y, int z,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData) {

	CubeFaceData v;

	float bottomLeft = 0;
	v = faceData[x - 1][y][z - 1];
	if (v.id != AIR) ++bottomLeft;
	v = faceData[x - 1][y - 1][z - 1];
	if (v.id != AIR) ++bottomLeft;
	v = faceData[x - 1][y - 1][z];
	if (v.id != AIR) ++bottomLeft;
	cf.lvLeft_BottomLeft -= min<float>(2.0,bottomLeft) * AOFactor;

	float bottomRight = 0;
	v = faceData[x - 1][y - 1][z];
	if (v.id != AIR) ++bottomRight;
	v = faceData[x - 1][y - 1][z + 1];
	if (v.id != AIR) ++bottomRight;
	v = faceData[x - 1][y][z + 1];
	if (v.id != AIR) ++bottomRight;
	cf.lvLeft_BottomRight -= min<float>(2.0, bottomRight) * AOFactor;

	float topRight = 0;
	v = faceData[x - 1][y][z + 1];
	if (v.id != AIR) ++topRight;
	v = faceData[x - 1][y + 1][z + 1];
	if (v.id != AIR) ++topRight;
	v = faceData[x - 1][y + 1][z];
	if (v.id != AIR) ++topRight;
	cf.lvLeft_TopRight -= min<float>(2.0, topRight) * AOFactor;

	char topLeft = 0;
	v = faceData[x - 1][y + 1][z];
	if (v.id != AIR) ++topLeft;
	v = faceData[x - 1][y + 1][z - 1];
	if (v.id != AIR) ++topLeft;
	v = faceData[x - 1][y][z - 1];
	if (v.id != AIR) ++topLeft;
	cf.lvLeft_TopLeft -= topLeft * AOFactor;
}

void GraphicalChunk::doAOFront(CubeFaceData &cf, int x, int y, int z,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData) {

	CubeFaceData v;

	float bottomLeft = 0;
	v = faceData[x - 1][y][z + 1];
	if (v.id != AIR) ++bottomLeft;
	v = faceData[x - 1][y - 1][z + 1];
	if (v.id != AIR) ++bottomLeft;
	v = faceData[x][y - 1][z + 1];
	if ( v.id != AIR) ++bottomLeft;
	cf.lvFront_BottomLeft -= min<float>(2.0, bottomLeft) * AOFactor;

	float bottomRight = 0;
	v = faceData[x][y - 1][z + 1];
	if (v.id != AIR) ++bottomRight;
	v = faceData[x + 1][y - 1][z + 1];
	if (v.id != AIR) ++bottomRight;
	v = faceData[x + 1][y][z + 1];
	if (v.id != AIR) ++bottomRight;
	cf.lvFront_BottomRight -= min<float>(2.0, bottomRight) * AOFactor;

	float topRight = 0;
	v = faceData[x + 1][y][z + 1];
	if (v.id != AIR) ++topRight;
	v = faceData[x + 1][y + 1][z + 1];
	if (v.id != AIR) ++topRight;
	v = faceData[x][y + 1][z + 1];
	if (v.id != AIR) ++topRight;
	cf.lvFront_TopRight -= min<float>(2.0, topRight) * AOFactor;

	float topLeft = 0;
	v = faceData[x][y + 1][z + 1];
	if (v.id != AIR) ++topLeft;
	v = faceData[x - 1][y + 1][z + 1];
	if (v.id != AIR) ++topLeft;
	v = faceData[x - 1][y][z + 1];
	if (v.id != AIR) ++topLeft;
	cf.lvFront_TopLeft -= min<float>(2.0, topLeft) * AOFactor;
}

void GraphicalChunk::doAOBack(CubeFaceData &cf, int x, int y, int z,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData) {

	CubeFaceData v;

	float bottomLeft = 0;
	v = faceData[x + 1][y][z - 1];
	if (v.id != AIR) ++bottomLeft;
	v = faceData[x + 1][y - 1][z - 1];
	if (v.id != AIR) ++bottomLeft;
	v = faceData[x][y - 1][z - 1];
	if (v.id != AIR) ++bottomLeft;
	cf.lvBack_BottomLeft -= min<float>(2.0, bottomLeft) * AOFactor;

	float bottomRight = 0;
	v = faceData[x][y - 1][z - 1];
	if (v.id != AIR) ++bottomRight;
	v = faceData[x - 1][y - 1][z - 1];
	if (v.id != AIR) ++bottomRight;
	v = faceData[x - 1][y][z - 1];
	if (v.id != AIR) ++bottomRight;
	cf.lvBack_BottomRight -= min<float>(2.0, bottomRight) * AOFactor;

	float topRight = 0;
	v = faceData[x - 1][y][z - 1];
	if (v.id != AIR) ++topRight;
	v = faceData[x - 1][y + 1][z - 1];
	if (v.id != AIR) ++topRight;
	v = faceData[x][y + 1][z - 1];
	if (v.id != AIR) ++topRight;
	cf.lvBack_TopRight -= min<float>(2.0, topRight) * AOFactor;

	float topLeft = 0;
	v = faceData[x][y + 1][z - 1];
	if (v.id != AIR) ++topLeft;
	v = faceData[x + 1][y + 1][z - 1];
	if (v.id != AIR) ++topLeft;
	v = faceData[x + 1][y][z - 1];
	if (v.id != AIR) ++topLeft;
	cf.lvBack_TopLeft -= min<float>(2.0, topLeft) * AOFactor;
}

void GraphicalChunk::doAOTop(CubeFaceData &cf, int x, int y, int z,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData) {

	CubeFaceData v;

	float bottomLeft = 0;
	v = faceData[x - 1][y + 1][z];
	if (v.id != AIR) ++bottomLeft;
	v = faceData[x - 1][y + 1][z + 1];
	if (v.id != AIR) ++bottomLeft;
	v = faceData[x][y + 1][z + 1];
	if (v.id != AIR) ++bottomLeft;
	cf.lvTop_BottomLeft -= min<float>(2.0, bottomLeft) * AOFactor;

	float bottomRight = 0;
	v = faceData[x][y + 1][z + 1];
	if (v.id != AIR) ++bottomRight;
	v = faceData[x + 1][y + 1][z + 1];
	if (v.id != AIR) ++bottomRight;
	v = faceData[x + 1][y + 1][z];
	if (v.id != AIR) ++bottomRight;
	cf.lvTop_BottomRight-= min<float>(2.0, bottomRight) * AOFactor;

	float topRight = 0;
	v = faceData[x + 1][y + 1][z];
	if (v.id != AIR) ++topRight;
	v = faceData[x + 1][y + 1][z - 1];
	if (v.id != AIR) ++topRight;
	v = faceData[x][y + 1][z - 1];
	if (v.id != AIR) ++topRight;
	cf.lvTop_TopRight -= min<float>(2.0, topRight) * AOFactor;

	float topLeft = 0;
	v = faceData[x][y + 1][z - 1];
	if (v.id != AIR) ++topLeft;
	v = faceData[x - 1][y + 1][z -1];
	if ( v.id != AIR) ++topLeft;
	v = faceData[x - 1][y + 1][z];
	if (v.id != AIR) ++topLeft;
	cf.lvTop_TopLeft -= min<float>(2.0, topLeft) * AOFactor;
}

void GraphicalChunk::doAOBottom(CubeFaceData &cf, int x, int y, int z,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData) {

	float bottomLeft = 0;
	CubeFaceData v = faceData[x - 1][y - 1][z];
	if (v.id != AIR) ++bottomLeft;
	v = faceData[x - 1][y - 1][z - 1];
	if (v.id != AIR) ++bottomLeft;
	v = faceData[x][y - 1][z - 1];
	if (v.id != AIR) ++bottomLeft;
	cf.lvBottom_BottomLeft -= min<float>(2.0, bottomLeft) * AOFactor;

	float bottomRight = 0;
	v = faceData[x][y - 1][z - 1];
	if (v.id != AIR) ++bottomRight;
	v = faceData[x + 1][y - 1][z - 1];
	if (v.id != AIR) ++bottomRight;
	v = faceData[x + 1][y - 1][z];
	if (v.id != AIR) ++bottomRight;
	cf.lvBottom_BottomRight-= min<float>(2.0, bottomRight) * AOFactor;

	float topRight = 0;
	v = faceData[x + 1][y - 1][z];
	if (v.id != AIR) ++topRight;
	v = faceData[x + 1][y - 1][z + 1];
	if (v.id != AIR) ++topRight;
	v = faceData[x][y - 1][z + 1];
	if (v.id != AIR) ++topRight;
	cf.lvBottom_TopRight -= min<float>(2.0, topRight) * AOFactor;

	float topLeft = 0;
	v = faceData[x][y - 1][z + 1];
	if (v.id != AIR) ++topLeft;
	v = faceData[x - 1][y - 1][z + 1];
	if (v.id != AIR) ++topLeft;
	v = faceData[x - 1][y - 1][z];
	if (v.id != AIR) ++topLeft;
	cf.lvBottom_TopLeft -= min<float>(2.0, topLeft) * AOFactor;
}

inline void GraphicalChunk::computeAverageHelper(
		int lightValue,
		const CubeFaceData &cLeftLeft,
		const CubeFaceData &cLeftLeft_Opposite,
		const CubeFaceData &cBottomLeft,
		const CubeFaceData &cBottomLeft_Opposite,
		const CubeFaceData &cBottomMiddle,
		const CubeFaceData &cBottomMiddle_Opposite,
		const CubeFaceData &cBottomRight,
		const CubeFaceData &cBottomRight_Opposite,
		const CubeFaceData &cRightRight,
		const CubeFaceData &cRightRight_Opposite,
		const CubeFaceData &cTopRight,
		const CubeFaceData &cTopRight_Opposite,
		const CubeFaceData &cTopMiddle,
		const CubeFaceData &cTopMiddle_Opposite,
		const CubeFaceData &cTopLeft,
		const CubeFaceData &cTopLeft_Opposite,
		float &bottomLeft,
		float &bottomRight,
		float &topRight,
		float &topLeft) {

// ############################################################################

	float counter {1};
	float acc = lightValue;
	if (cLeftLeft.id != AIR && cLeftLeft_Opposite.id == AIR) {
		acc += cLeftLeft_Opposite.lightValue;
		++counter;
	}
	if (cBottomLeft.id != AIR && cBottomLeft_Opposite.id == AIR &&
			(cLeftLeft_Opposite.id == AIR ||
					cBottomMiddle_Opposite.id == AIR)) {
		acc += cBottomLeft_Opposite.lightValue;
		++counter;
	}
	if (cBottomMiddle.id != AIR && cBottomMiddle_Opposite.id == AIR) {
		acc += cBottomMiddle_Opposite.lightValue;
		++counter;
	}
	bottomLeft = acc / counter;

// ############################################################################

	counter = 1;
	acc = lightValue;
	if (cBottomMiddle.id != AIR && cBottomMiddle_Opposite.id == AIR) {
		acc += cBottomMiddle_Opposite.lightValue;
		++counter;
	}
	if (cBottomRight.id != AIR && cBottomRight_Opposite.id == AIR &&
			(cBottomMiddle_Opposite.id == AIR ||
					cRightRight_Opposite.id == AIR)) {
		acc += cBottomRight_Opposite.lightValue;
		++counter;
	}
	if (cRightRight.id != AIR && cRightRight_Opposite.id == AIR) {
		acc += cRightRight_Opposite.lightValue;
		++counter;
	}
	bottomRight = acc / counter;

// ############################################################################

	counter = 1;
	acc = lightValue;
	if (cRightRight.id != AIR && cRightRight_Opposite.id == AIR) {
		acc += cRightRight_Opposite.lightValue;
		++counter;
	}
	if (cTopRight.id != AIR && cTopRight_Opposite.id == AIR &&
			(cRightRight_Opposite.id == AIR ||
					cTopMiddle.id == AIR)) {
		acc += cTopRight_Opposite.lightValue;
		++counter;
	}
	if (cTopMiddle.id != AIR && cTopMiddle_Opposite.id == AIR) {
		acc += cTopMiddle_Opposite.lightValue;
		++counter;
	}
	topRight = acc / counter;

// ############################################################################

	counter = 1;
	acc = lightValue;
	if (cTopMiddle.id != AIR && cTopMiddle_Opposite.id == AIR) {
		acc += cTopMiddle_Opposite.lightValue;
		++counter;
	}
	if (cTopLeft.id != AIR && cTopLeft_Opposite.id == AIR &&
			(cTopMiddle_Opposite.id == AIR ||
					cLeftLeft_Opposite.id == AIR)) {
		acc += cTopLeft_Opposite.lightValue;
		++counter;
	}
	if (cLeftLeft.id != AIR && cLeftLeft_Opposite.id == AIR) {
		acc += cLeftLeft_Opposite.lightValue;
		++counter;
	}
	topLeft = acc / counter;

// ############################################################################

}

void GraphicalChunk::computeAverageRight(int lightValue, int x, int y, int z,
		float &bottomLeft, float &bottomRight, float &topRight, float &topLeft,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData) {

	CubeFaceData &cBottomLeft = faceData[x][y - 1][z + 1];
	CubeFaceData &cBottomLeft_Right = faceData[x + 1][y - 1][z + 1];

	CubeFaceData &cBottomMiddle = faceData[x][y - 1][z];
	CubeFaceData &cBottomMiddle_Right = faceData[x + 1][y - 1][z];

	CubeFaceData &cBottomRight = faceData[x][y - 1][z - 1];
	CubeFaceData &cBottomRight_Right = faceData[x + 1][y - 1][z - 1];

	CubeFaceData &cRightRight = faceData[x][y][z - 1];
	CubeFaceData &cRightRight_Right = faceData[x + 1][y][z - 1];

	CubeFaceData &cTopRight = faceData[x][y + 1][z - 1];
	CubeFaceData &cTopRight_Right = faceData[x + 1][y + 1][z - 1];

	CubeFaceData &cTopMiddle = faceData[x][y + 1][z];
	CubeFaceData &cTopMiddle_Right = faceData[x + 1][y + 1][z];

	CubeFaceData &cTopLeft = faceData[x][y + 1][z + 1];
	CubeFaceData &cTopLeft_Right = faceData[x + 1][y + 1][z + 1];

	CubeFaceData &cLeftLeft = faceData[x][y][z + 1];
	CubeFaceData &cLeftLeft_Right = faceData[x + 1][y][z + 1];

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

	CubeFaceData &cBottomLeft = faceData[x][y - 1][z - 1];
	CubeFaceData &cBottomLeft_Left = faceData[x - 1][y - 1][z - 1];

	CubeFaceData &cBottomMiddle = faceData[x][y - 1][z];
	CubeFaceData &cBottomMiddle_Left = faceData[x - 1][y - 1][z];

	CubeFaceData &cBottomRight = faceData[x][y - 1][z + 1];
	CubeFaceData &cBottomRight_Left = faceData[x - 1][y - 1][z + 1];

	CubeFaceData &cRightRight = faceData[x][y][z + 1];
	CubeFaceData &cRightRight_Left = faceData[x - 1][y][z + 1];

	CubeFaceData &cTopRight = faceData[x][y + 1][z + 1];
	CubeFaceData &cTopRight_Left = faceData[x - 1][y + 1][z + 1];

	CubeFaceData &cTopMiddle = faceData[x][y + 1][z];
	CubeFaceData &cTopMiddle_Left = faceData[x - 1][y + 1][z];

	CubeFaceData &cTopLeft = faceData[x][y + 1][z - 1];
	CubeFaceData &cTopLeft_Left = faceData[x - 1][y + 1][z - 1];

	CubeFaceData &cLeftLeft = faceData[x][y][z - 1];
	CubeFaceData &cLeftLeft_Left = faceData[x - 1][y][z - 1];

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

	CubeFaceData &cBottomLeft = faceData[x - 1][y][z + 1];
	CubeFaceData &cBottomLeft_Top = faceData[x - 1][y + 1][z + 1];

	CubeFaceData &cBottomMiddle = faceData[x][y][z + 1];
	CubeFaceData &cBottomMiddle_Top = faceData[x][y + 1][z + 1];

	CubeFaceData &cBottomRight = faceData[x + 1][y][z + 1];
	CubeFaceData &cBottomRight_Top = faceData[x + 1][y + 1][z + 1];

	CubeFaceData &cRightRight = faceData[x + 1][y][z];
	CubeFaceData &cRightRight_Top = faceData[x + 1][y + 1][z];

	CubeFaceData &cTopRight = faceData[x + 1][y][z - 1];
	CubeFaceData &cTopRight_Top = faceData[x + 1][y + 1][z - 1];

	CubeFaceData &cTopMiddle = faceData[x][y][z - 1];
	CubeFaceData &cTopMiddle_Top = faceData[x][y + 1][z - 1];

	CubeFaceData &cTopLeft = faceData[x - 1][y][z - 1];
	CubeFaceData &cTopLeft_Top = faceData[x - 1][y + 1][z - 1];

	CubeFaceData &cLeftLeft = faceData[x - 1][y][z];
	CubeFaceData &cLeftLeft_Top = faceData[x - 1][y + 1][z];

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

	CubeFaceData &cBottomLeft = faceData[x - 1][y][z - 1];
	CubeFaceData &cBottomLeft_Bottom = faceData[x - 1][y - 1][z - 1];

	CubeFaceData &cBottomMiddle = faceData[x][y][z - 1];
	CubeFaceData &cBottomMiddle_Bottom = faceData[x][y - 1][z - 1];

	CubeFaceData &cBottomRight = faceData[x + 1][y][z - 1];
	CubeFaceData &cBottomRight_Bottom = faceData[x + 1][y - 1][z - 1];

	CubeFaceData &cRightRight = faceData[x + 1][y][z];
	CubeFaceData &cRightRight_Bottom = faceData[x + 1][y - 1][z];

	CubeFaceData &cTopRight = faceData[x + 1][y][z + 1];
	CubeFaceData &cTopRight_Bottom = faceData[x + 1][y - 1][z + 1];

	CubeFaceData &cTopMiddle = faceData[x][y][z + 1];
	CubeFaceData &cTopMiddle_Bottom = faceData[x][y - 1][z + 1];

	CubeFaceData &cTopLeft = faceData[x - 1][y][z + 1];
	CubeFaceData &cTopLeft_Bottom = faceData[x - 1][y - 1][z + 1];

	CubeFaceData &cLeftLeft = faceData[x - 1][y][z];
	CubeFaceData &cLeftLeft_Bottom = faceData[x - 1][y - 1][z];

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

	CubeFaceData &cBottomLeft = faceData[x + 1][y - 1][z];
	CubeFaceData &cBottomLeft_Back = faceData[x + 1][y - 1][z - 1];

	CubeFaceData &cBottomMiddle = faceData[x][y - 1][z];
	CubeFaceData &cBottomMiddle_Back = faceData[x][y - 1][z - 1];

	CubeFaceData &cBottomRight = faceData[x - 1][y - 1][z];
	CubeFaceData &cBottomRight_Back = faceData[x - 1][y - 1][z - 1];

	CubeFaceData &cRightRight = faceData[x - 1][y][z];
	CubeFaceData &cRightRight_Back = faceData[x - 1][y][z - 1];

	CubeFaceData &cTopRight = faceData[x - 1][y + 1][z];
	CubeFaceData &cTopRight_Back = faceData[x - 1][y + 1][z - 1];

	CubeFaceData &cTopMiddle = faceData[x][y + 1][z];
	CubeFaceData &cTopMiddle_Back = faceData[x][y + 1][z - 1];

	CubeFaceData &cTopLeft = faceData[x + 1][y + 1][z];
	CubeFaceData &cTopLeft_Back = faceData[x + 1][y + 1][z - 1];

	CubeFaceData &cLeftLeft = faceData[x + 1][y][z];
	CubeFaceData &cLeftLeft_Back = faceData[x + 1][y][z - 1];

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

	CubeFaceData &cBottomLeft = faceData[x - 1][y - 1][z];
	CubeFaceData &cBottomLeft_Front = faceData[x - 1][y - 1][z + 1];

	CubeFaceData &cBottomMiddle = faceData[x][y - 1][z];
	CubeFaceData &cBottomMiddle_Front = faceData[x][y - 1][z + 1];

	CubeFaceData &cBottomRight = faceData[x + 1][y - 1][z];
	CubeFaceData &cBottomRight_Front = faceData[x + 1][y - 1][z + 1];

	CubeFaceData &cRightRight = faceData[x + 1][y][z];
	CubeFaceData &cRightRight_Front = faceData[x + 1][y][z + 1];

	CubeFaceData &cTopRight = faceData[x + 1][y + 1][z];
	CubeFaceData &cTopRight_Front = faceData[x + 1][y + 1][z + 1];

	CubeFaceData &cTopMiddle = faceData[x][y + 1][z];
	CubeFaceData &cTopMiddle_Front = faceData[x][y + 1][z + 1];

	CubeFaceData &cTopLeft = faceData[x - 1][y + 1][z];
	CubeFaceData &cTopLeft_Front = faceData[x - 1][y + 1][z + 1];

	CubeFaceData &cLeftLeft = faceData[x - 1][y][z];
	CubeFaceData &cLeftLeft_Front = faceData[x - 1][y][z + 1];

	computeAverageHelper(lightValue, cLeftLeft, cLeftLeft_Front, cBottomLeft,
			cBottomLeft_Front, cBottomMiddle, cBottomMiddle_Front, cBottomRight,
			cBottomRight_Front, cRightRight, cRightRight_Front, cTopRight,
			cTopRight_Front, cTopMiddle, cTopMiddle_Front, cTopLeft,
			cTopLeft_Front, bottomLeft, bottomRight, topRight, topLeft);
}

}
