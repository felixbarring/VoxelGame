
#include "chunk.h"

#include <iostream>

#include "../../../graphics/chunkBatcher.h"

namespace chunk
{

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Chunk::Chunk(int x, int y, int z):
	xLocation{x},
	yLocation{y},
	zLocation{z}
{
	int counter = 1;
	const int maxCount = 7;

	for (int i = 0; i < config::chunk_data::CHUNK_WIDHT; i++) {
		vec.push_back(std::vector<std::vector<Voxel>>());

		for (int j = 0; j < config::chunk_data::CHUNK_HEIGHT; j++) {
			vec[i].push_back(std::vector<Voxel>());

			for (int k = 0; k < config::chunk_data::CHUNK_DEPTH; k++) {
				Voxel v;
				v.lightValue = 0;
				if (j < 5) {
					v.id = counter++;
					vec[i][j].push_back(v);

					if (counter == maxCount)
						counter = 0;

				} else {
					v.id = config::cube_data::AIR;
					vec[i][j].push_back(v);
				}
			}

		}
	}


	doSunLightning();

}


// ########################################################
// Member Functions########################################
// ########################################################

char Chunk::getCubeId(int x, int y, int z)
{
	return vec.at(x).at(y).at(z).id;
}

void Chunk::setCube(int x, int y, int z, char id)
{
	Voxel &voxel = vec[x][y][z];
	voxel.id = id;

	doSunLightning();

	if (rightNeighbor.get() != nullptr)
		rightNeighbor->doSunLightning();

	if (leftNeighbor.get() != nullptr)
		leftNeighbor->doSunLightning();

	if (frontNeighbor.get() != nullptr)
		frontNeighbor->doSunLightning();

	if (backNeighbor.get() != nullptr)
		backNeighbor->doSunLightning();

	updateGraphics();

	if (rightNeighbor.get() != nullptr)
		rightNeighbor->updateGraphics();

	if (leftNeighbor.get() != nullptr)
		leftNeighbor->updateGraphics();

	if (frontNeighbor.get() != nullptr)
		frontNeighbor->updateGraphics();

	if (backNeighbor.get() != nullptr)
		backNeighbor->updateGraphics();

}

void Chunk::updateGraphics()
{

	std::vector<std::vector<std::vector<Voxel>>> *right = nullptr;
	std::vector<std::vector<std::vector<Voxel>>> *left = nullptr;
	std::vector<std::vector<std::vector<Voxel>>> *front = nullptr;
	std::vector<std::vector<std::vector<Voxel>>> *back = nullptr;

	if (rightNeighbor.get() != nullptr)
		right = &(rightNeighbor->vec);

	if (leftNeighbor.get() != nullptr)
		left = &(leftNeighbor->vec);

	if (frontNeighbor.get() != nullptr)
		front = &(frontNeighbor->vec);

	if (backNeighbor.get() != nullptr)
		back = &(backNeighbor->vec);

	graphics::ChunkBatcher::getInstance().removeBatch(graphicalChunk);
	graphicalChunk.reset(new graphics::GraphicalChunk(xLocation, yLocation, zLocation, vec,	right, left, front, back));
	graphics::ChunkBatcher::getInstance().addBatch(graphicalChunk);

}


void Chunk::setLeftNeighbor(std::shared_ptr<Chunk> chunk)
{
	leftNeighbor = chunk;
}

void Chunk::setRightNeighbor(std::shared_ptr<Chunk> chunk)
{
	rightNeighbor = chunk;
}


void Chunk::setFrontNeighbor(std::shared_ptr<Chunk> chunk)
{
	frontNeighbor = chunk;
}

void Chunk::setBackNeighbor(std::shared_ptr<Chunk> chunk)
{
	backNeighbor = chunk;
}


void Chunk::doSunLightning()
{
	// Cubes that should propagate their light
	std::vector<glm::vec3> lightPropagate;

	bool foundSolid = false;
	// Sun lightning, only air gets light
	for (int i = 0; i < config::chunk_data::CHUNK_WIDHT; i++) {
		for (int j = 0; j < config::chunk_data::CHUNK_DEPTH; j++) {
			foundSolid = false;
			for (int k = config::chunk_data::CHUNK_HEIGHT-1; k >= 0; k--) {
				if (vec[i][k][j].id == config::cube_data::AIR) {

					if (foundSolid) {
						vec[i][k][j].lightValue = 0;
					} else {
						vec[i][k][j].lightValue = 15;
						lightPropagate.push_back(glm::vec3(i,k,j));
					}
				} else {
					foundSolid = true;
				}
			}
		}
	}

	for (glm::vec3 vec : lightPropagate)
		propagateLight(vec.x, vec.y, vec.z);

	// Should not be necessary.
	//lightPropagate.clear();

}

void Chunk::propagateLight(int x, int y, int z)
{

	Voxel &voxel = vec[x][y][z];
	int lvInitial = voxel.lightValue - 1;

	std::vector<glm::vec3> newPropagates;

	// TODO Remove all the hard coded values

	// Traverse right
	int lv = lvInitial;
	for (int i = x + 1; lv > 0; i++) {

		if (i < 16) {
			Voxel &v = vec[i][y][z];
			if (v.id == config::cube_data::AIR && v.lightValue < lv) {
				v.lightValue = lv;
				newPropagates.push_back(glm::vec3(i, y, z));
				lv--;
			} else {
				break;
			}
		} else {
			if (rightNeighbor.get() != nullptr) {
				if (rightNeighbor->vec[0][y][z].id == config::cube_data::AIR && rightNeighbor->vec[0][y][z].lightValue < lv) {
					rightNeighbor->vec[0][y][z].lightValue = lv;
					rightNeighbor->propagateLight(0, y, z);
				}
			}
			break;
		}
	}

	// Travers left
	lv = lvInitial;
	for (int i = x - 1; lv > 0; i--) {

		if (i >= 0) {
			Voxel &v = vec[i][y][z];
			if (v.id == config::cube_data::AIR && v.lightValue < lv) {
				v.lightValue = lv;
				newPropagates.push_back(glm::vec3(i, y, z));
				lv--;
			} else {
				break;
			}
		} else {
			if (leftNeighbor.get() != nullptr) {
				if (leftNeighbor->vec[15][y][z].id == config::cube_data::AIR && leftNeighbor->vec[15][y][z].lightValue < lv) {
					leftNeighbor->vec[15][y][z].lightValue = lv;
					leftNeighbor->propagateLight(15, y, z);
				}
			}
			break;
		}
	}

	// ##################################################################################

	// Traverse up
	lv = lvInitial;
	for (int i = y + 1; i < 16; i++) {
		Voxel &v = vec[x][i][z];
		if (v.id == config::cube_data::AIR && v.lightValue < lv) {
			v.lightValue = lv;
			newPropagates.push_back(glm::vec3(x, i, z));
			lv--;
		} else {
			break;
		}
	}

	// Treaverse down
	lv = lvInitial;
	for (int i = y - 1; i >= 0; i--) {
		Voxel &v = vec[x][i][z];
		if (v.id == config::cube_data::AIR && v.lightValue < lv) {
			v.lightValue = lv;
			newPropagates.push_back(glm::vec3(x, i, z));
			lv--;
		} else {
			break;
		}
	}

	// ##################################################################################


	// Traverse backwards
	lv = lvInitial;
	for (int i = z + 1; lv > 0; i++) {

		if (i < 16) {
			Voxel &v = vec[x][y][i];
			if (v.id == config::cube_data::AIR && v.lightValue < lv) {
				v.lightValue = lv;
				newPropagates.push_back(glm::vec3(x, y, i));
				lv--;
			} else {
				break;
			}
		} else {
			if (backNeighbor.get() != nullptr) {
				if (backNeighbor->vec[x][y][0].id == config::cube_data::AIR && backNeighbor->vec[x][y][0].lightValue < lv) {
					backNeighbor->vec[x][y][0].lightValue = lv;
					backNeighbor->propagateLight(x, y, 0);
				}
			}
			break;
		}
	}

	// Traverse forwards
	lv = lvInitial;
	for (int i = z - 1; lv > 0; i--) {

		if (i >= 0) {

			Voxel &v = vec[x][y][i];
			if (v.id == config::cube_data::AIR && v.lightValue < lv) {
				v.lightValue = lv;
				newPropagates.push_back(glm::vec3(x, y, i));
				lv--;
			} else {
				break;
			}
		} else {
			if (frontNeighbor.get() != nullptr) {
				if (frontNeighbor->vec[x][y][15].id == config::cube_data::AIR && frontNeighbor->vec[x][y][15].lightValue < lv) {
					frontNeighbor->vec[x][y][15].lightValue = lv;
					frontNeighbor->propagateLight(x, y, 15);
				}
			}
			break;
		}
	}

	// Consider postponing this until the all the propagate calls generated
	// By the doSunLightning function has completed, to avoid repeating work
	for (glm::vec3 vec : newPropagates)
		propagateLight(vec.x, vec.y, vec.z);

}

}
