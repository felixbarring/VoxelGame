
#include "graphicalChunk.h"

#include <vector>
#include <iostream>

#include "texturedCube.h"
#include "transform.h"

namespace graphics
{

// ########################################################
// Constructor/Destructor #################################
// ########################################################

GraphicalChunk::GraphicalChunk(float x, float y, float z,
	std::vector<std::vector<std::vector<Voxel>>> &data,
	std::vector<std::vector<std::vector<Voxel>>> *right,
	std::vector<std::vector<std::vector<Voxel>>> *left,
	std::vector<std::vector<std::vector<Voxel>>> *back,
	std::vector<std::vector<std::vector<Voxel>>> *front

	):
xLocation{x},
yLocation{y},
zLocation{z},
transform{
	x + width / 2 + 0.5f,
	y + height / 2 + 0.5f,
	z + depth / 2 + 0.5f
}
{

	CubeFaceData faceData[width][height][depth];

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			for (int k = 0; k < depth; k++) {
				CubeFaceData cube;
				cube.id = data[i][j][k].id;
				cube.lightValue = data[i][j][k].lightValue;

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

	// Do smooth and AO here?
	// Remove faces
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			for (int k = 0; k < depth; k++) {

				CubeFaceData &current = faceData[i][j][k];

				if (current.id == config::cube_data::AIR) {
					current.front = false;
					current.back = false;
					current.left = false;
					current.right = false;
					current.top = false;
					current.bottom = false;
					continue;
				}

				//bool lol = getVoxel(16, j, k, data, right, left, back, front) == nullptr;
				//std::cout << " --- " << lol << "\n";

				// X ##########################################################

				Voxel *voxel = getVoxel(i + 1, j, k, data, right, left, back, front);
				if (voxel) {
					if (voxel->id != config::cube_data::AIR) {
						current.right = false;
					} else {
						current.lvRight_BottomLeft = voxel->lightValue;
						current.lvRight_BottomRight = voxel->lightValue;
						current.lvRight_TopRight = voxel->lightValue;
						current.lvRight_TopLeft = voxel->lightValue;
					}
				}

				voxel = getVoxel(i - 1, j, k, data, right, left, back, front);
				if (voxel) {
					if (voxel->id != config::cube_data::AIR) {
						current.left = false;
					} else {
						current.lvLeft_BottomLeft = voxel->lightValue;
						current.lvLeft_BottomRight = voxel->lightValue;
						current.lvLeft_TopRight = voxel->lightValue;
						current.lvLeft_TopLeft = voxel->lightValue;
					}
				}

				// Y ##########################################################

				voxel = getVoxel(i, j + 1, k, data, right, left, back, front);
				if (voxel) {
					if (voxel->id != config::cube_data::AIR) {
						current.top = false;
					} else {
						current.lvTop_BottomLeft = voxel->lightValue;
						current.lvTop_BottomRight = voxel->lightValue;
						current.lvTop_TopRight = voxel->lightValue;
						current.lvTop_TopLeft = voxel->lightValue;
					}
				}

				voxel = getVoxel(i, j - 1, k, data, right, left, back, front);
				if (voxel) {
					if (voxel->id != config::cube_data::AIR) {
						current.bottom = false;
					} else {
						current.lvBottom_BottomLeft = voxel->lightValue;
						current.lvBottom_BottomRight = voxel->lightValue;
						current.lvBottom_TopRight = voxel->lightValue;
						current.lvBottom_TopLeft = voxel->lightValue;
					}
				}

				// Z ##########################################################

				voxel = getVoxel(i, j, k + 1, data, right, left, back, front);
				if (voxel) {
					if (voxel->id != config::cube_data::AIR) {
						current.back = false;
					} else {
						current.lvBack_BottomLeft = voxel->lightValue;
						current.lvBack_BottomRight = voxel->lightValue;
						current.lvBack_TopRight = voxel->lightValue;
						current.lvBack_TopLeft = voxel->lightValue;
					}
				}

				voxel = getVoxel(i, j, k - 1, data, right, left, back, front);
				if (voxel) {
					if (voxel->id != config::cube_data::AIR) {
						current.front = false;
					} else {
						current.lvFront_BottomLeft = voxel->lightValue;
						current.lvFront_BottomRight = voxel->lightValue;
						current.lvFront_TopRight = voxel->lightValue;
						current.lvFront_TopLeft = voxel->lightValue;
					}
				}

			}
		}
	}

	// The fourth element of the vertex data is the light value.
	std::vector<GLfloat> vertexData;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> UV;
	std::vector<short> elementData;

	short elementOffset = 0;
	int totalNumberOfFaces = 0;

	float dx = -width / 2;
	float dy = -height / 2;
	float dz = -depth / 2;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			for (int k = 0; k < depth; k++) {

				CubeFaceData fd = faceData[i][j][k];

				int id = fd.id;
				if (id == config::cube_data::AIR)
					continue;

				GLfloat sideTexture = config::cube_data::BLOCK_TEXTURES[id][config::cube_data::SIDE_TEXTURE];
				GLfloat topTexture = config::cube_data::BLOCK_TEXTURES[id][config::cube_data::TOP_TEXTURE];
				GLfloat bottomTexture = config::cube_data::BLOCK_TEXTURES[id][config::cube_data::BOTTOM_TEXTURE];

				if (fd.front) {

					std::vector<GLfloat> vertex {
						-0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz, fd.lvFront_BottomLeft,
						0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz, fd.lvFront_BottomRight,
						0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz, fd.lvFront_TopRight,
						-0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz, fd.lvFront_TopLeft,
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
						static_cast<short>(0 + elementOffset), static_cast<short>(1 + elementOffset), static_cast<short>(2 + elementOffset),
						static_cast<short>(0 + elementOffset), static_cast<short>(2 + elementOffset), static_cast<short>(3 + elementOffset)
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
						0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz, fd.lvBack_BottomLeft,
						-0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz, fd.lvBack_BottomRight,
						-0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz, fd.lvBack_TopRight,
						0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz, fd.lvBack_TopLeft,

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
						static_cast<short>(0 + elementOffset), static_cast<short>(1 + elementOffset), static_cast<short>(2 + elementOffset),
						static_cast<short>(0 + elementOffset), static_cast<short>(2 + elementOffset), static_cast<short>(3 + elementOffset)
					};

					for (auto v : vertex) { vertexData.push_back(v);}
					for (auto n : nor) { normals.push_back(n);}
					for (auto u : uv) { UV.push_back(u);}
					for (auto e : el) { elementData.push_back(e);}

					elementOffset += 4;
					totalNumberOfFaces++;
				}

				if (fd.left) {

					std::vector<GLfloat> vertex {
						-0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz, fd.lvLeft_BottomLeft,
						-0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz, fd.lvLeft_BottomRight,
						-0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz, fd.lvLeft_TopRight,
						-0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz, fd.lvLeft_TopLeft,
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
						static_cast<short>(0 + elementOffset), static_cast<short>(1 + elementOffset), static_cast<short>(2 + elementOffset),
						static_cast<short>(0 + elementOffset), static_cast<short>(2 + elementOffset), static_cast<short>(3 + elementOffset)
					};

					for (auto v : vertex) { vertexData.push_back(v);}
					for (auto n : nor) { normals.push_back(n);}
					for (auto u : uv) { UV.push_back(u);}
					for (auto e : el) { elementData.push_back(e);}

					elementOffset += 4;
					totalNumberOfFaces++;
				}

				if (fd.right) {

					std::vector<GLfloat> vertex {
						0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz, fd.lvRight_BottomLeft,
						0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz, fd.lvRight_BottomRight,
						0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz, fd.lvRight_TopRight,
						0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz, fd.lvRight_TopLeft,
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
						static_cast<short>(0 + elementOffset), static_cast<short>(1 + elementOffset), static_cast<short>(2 + elementOffset),
						static_cast<short>(0 + elementOffset), static_cast<short>(2 + elementOffset), static_cast<short>(3 + elementOffset)
					};

					for (auto v : vertex) { vertexData.push_back(v);}
					for (auto n : nor) { normals.push_back(n);}
					for (auto u : uv) { UV.push_back(u);}
					for (auto e : el) { elementData.push_back(e);}

					elementOffset += 4;
					totalNumberOfFaces++;
				}


				if (fd.top) {

					std::vector<GLfloat> vertex {
						-0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz, fd.lvTop_BottomLeft,
						0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz, fd.lvTop_BottomRight,
						0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz, fd.lvTop_TopRight,
						-0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz, fd.lvTop_TopLeft,
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
						static_cast<short>(0 + elementOffset), static_cast<short>(1 + elementOffset), static_cast<short>(2 + elementOffset),
						static_cast<short>(0 + elementOffset), static_cast<short>(2 + elementOffset), static_cast<short>(3 + elementOffset)
					};

					for (auto v : vertex) { vertexData.push_back(v);}
					for (auto n : nor) { normals.push_back(n);}
					for (auto u : uv) { UV.push_back(u);}
					for (auto e : el) { elementData.push_back(e);}

					elementOffset += 4;
					totalNumberOfFaces++;
				}

				if (fd.bottom) {

					std::vector<GLfloat> vertex {
						-0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz, fd.lvBottom_BottomRight,
						0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz, fd.lvBottom_BottomRight,
						0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz, fd.lvBottom_TopRight,
						-0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz, fd.lvBottom_TopLeft,
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
						static_cast<short>(0 + elementOffset), static_cast<short>(1 + elementOffset), static_cast<short>(2 + elementOffset),
						static_cast<short>(0 + elementOffset), static_cast<short>(2 + elementOffset), static_cast<short>(3 + elementOffset)
					};

					for (auto v : vertex) { vertexData.push_back(v);}
					for (auto n : nor) { normals.push_back(n);}
					for (auto u : uv) { UV.push_back(u);}
					for (auto e : el) { elementData.push_back(e);}

					elementOffset += 4;
					totalNumberOfFaces++;
				}
			}
		}
	}

	mesh.reset(new mesh::MeshElement(vertexData, 4, normals, 3, UV, 3, elementData));
	//std::cout<<"Total number of faces: "<<totalNumberOfFaces<<"\n";

}

// ########################################################
// Member Functions########################################
// ########################################################

// TODO Remove the hardcoded value later

// Function for getting voxels, can collect from neighbor chunks data
// If there is no neighobor, nullptr will be returned
// Trying to get a voxel that is not adjacent to this chunk is an error
Voxel* GraphicalChunk::getVoxel(int x, int y, int z,
		std::vector<std::vector<std::vector<Voxel>>> &data,
		std::vector<std::vector<std::vector<Voxel>>> *right,
		std::vector<std::vector<std::vector<Voxel>>> *left,
		std::vector<std::vector<std::vector<Voxel>>> *back,
		std::vector<std::vector<std::vector<Voxel>>> *front)
{

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

void GraphicalChunk::draw()
{
	mesh->draw();
}

Transform& GraphicalChunk::getTransform()
{
	return transform;
}

float GraphicalChunk::getxLocation()
{
	return xLocation;
}

float GraphicalChunk::getyLocation()
{
	return yLocation;
}

}
