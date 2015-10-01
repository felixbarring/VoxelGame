
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
		Voxel data[config::chunk_data::GRAPHICAL_CHUNK_WIDTH][config::chunk_data::GRAPHICAL_CHUNK_HEIGHT][config::chunk_data::GRAPHICAL_CHUNK_DEPTH]):
xLocation{x},
yLocation{y},
zLocation{z},
transform{
	x + width / 2 + 0.5f,
	y + height / 2 + 0.5f,
	z + depth / 2 + 0.5f}
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

				CubeFaceData& current = faceData[i][j][k];

				if (current.id == config::cube_data::AIR) {
					current.front = false;
					current.back = false;
					current.left = false;
					current.right = false;
					current.top = false;
					current.bottom = false;
					continue;
				}

				if (i != width - 1) {
					CubeFaceData& right = faceData[i + 1][j][k];
					if (right.id != config::cube_data::AIR) {
						current.right = false;
						right.left = false;
					}
				}

				if (j != height - 1) {
					CubeFaceData& up = faceData[i][j + 1][k];
					if (up.id != config::cube_data::AIR) {
						current.top = false;
						up.bottom = false;
					}
				}

				if (k != depth - 1) {
					CubeFaceData& back = faceData[i][j][k + 1];
					if (back.id != config::cube_data::AIR) {
						current.back = false;
						back.front = false;
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

	float dx = -width / 2;
	float dy = -height / 2;
	float dz = -depth / 2;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			for (int k = 0; k < depth; k++) {

				CubeFaceData fd = faceData[i][j][k];

				int id = fd.id;

				GLfloat sideTexture = config::cube_data::BLOCK_TEXTURES[id][config::cube_data::SIDE_TEXTURE];
				GLfloat topTexture =config::cube_data::BLOCK_TEXTURES[id][config::cube_data::TOP_TEXTURE];
				GLfloat bottomTexture = config::cube_data::BLOCK_TEXTURES[id][config::cube_data::BOTTOM_TEXTURE];

				if (fd.front) {

					std::vector<GLfloat> vertex {
						-0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz,

						0.5f + i + dx, 0.5f + j+ dy, -0.5f + k + dz,
						0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz,



						-0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz,
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
						0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz,


						-0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz,
						-0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz,


						0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz,
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
						-0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz,

						-0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz,
						-0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz,

						-0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz,
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
						0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz,

						0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz,
						0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz,

						0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz,


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
						-0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz,

						0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz,
						0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz,


						-0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz,


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
						-0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz,

						0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz,
						0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz,

						-0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz,
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

	/*

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			for (int k = 0; k < depth; k++) {

				CubeFaceData fd = faceData[i][j][k];

				int id = fd.id;

				GLfloat sideTexture = config::cube_data::BLOCK_TEXTURES[id][config::cube_data::SIDE_TEXTURE];
				GLfloat topTexture =config::cube_data::BLOCK_TEXTURES[id][config::cube_data::TOP_TEXTURE];
				GLfloat bottomTexture = config::cube_data::BLOCK_TEXTURES[id][config::cube_data::BOTTOM_TEXTURE];

				if (fd.front) {

					std::vector<GLfloat> vertex {
						-0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz,
						0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz,
						0.5f + i + dx, 0.5f + j+ dy, -0.5f + k + dz,
						-0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz
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
						0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz,
						-0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz,
						-0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz,
						0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz,
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
						-0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz,
						-0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz,
						-0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz,
						-0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz,
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
						0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz,
						0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz,
						0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz,
						0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz,
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
						-0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz,
						0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz,
						0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz,
						-0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz
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
						-0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz,
						0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz,
						0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz,
						-0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz
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

	*/

	mesh.reset(new mesh::MeshElement(vertexData, 3, normals, 3, UV, 3, elementData));
	//std::cout<<"Total number of faces: "<<totalNumberOfFaces<<"\n";
}

GraphicalChunk::~GraphicalChunk()
{
}

// ########################################################
// Member Functions########################################
// ########################################################

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

