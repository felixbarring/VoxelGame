#include "chunkManager.h"

#include <math.h>
#include <limits>
#include <iostream>
#include "../../../util/voxel.h"

using namespace std;
using namespace glm;

using namespace config::chunk_data;
using namespace config::cube_data;

namespace chunk {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

ChunkManager::ChunkManager() {

}

// ########################################################
// Member Functions########################################
// ########################################################

void ChunkManager::createNewWorld() {
	const int xMax = NUMBER_OF_CHUNKS_X;
	const int yMax = NUMBER_OF_CHUNKS_Y;
	const int zMax = NUMBER_OF_CHUNKS_Z;

	// Create the Chunks
	for (int x = 0; x < xMax; ++x) {
		for (int z = 0; z < zMax; ++z) {
			chunks[x][0][z].reset(new Chunk {x * CHUNK_WIDTH, z * CHUNK_DEPTH});
		}
	}

	// Connect the Chunks
	for (int x = 0; x < xMax; ++x) {
		for (int z = 0; z < zMax; ++z) {
			shared_ptr<Chunk> current = chunks[x][0][z];
			if (x != xMax - 1) {
				shared_ptr<Chunk> right = chunks[x + 1][0][z];
				current->setRightNeighbor(right);
				right->setLeftNeighbor(current);
			}
			if (z != zMax - 1) {
				shared_ptr<Chunk> back = chunks[x][0][z + 1];
				current->setBackNeighbor(back);
				back->setFrontNeighbor(current);
			}
			current->updateGraphics();
		}
	}

	for (int x = 0; x < xMax; ++x) {
		for (int z = 0; z < zMax; ++z) {
			chunks[x][0][z]->updateLightning();
			chunks[x][0][z]->updateGraphics();
		}
	}

}

void ChunkManager::loadWorld(std::string& worldName) {

	const int xMax = NUMBER_OF_CHUNKS_X;
	const int yMax = NUMBER_OF_CHUNKS_Y;
	const int zMax = NUMBER_OF_CHUNKS_Z;

	for (int x = 0; x < xMax; x++) {
		for (int z = 0; z < zMax; z++) {
			if (chunks[x][0][z])
				chunks[x][0][z]->removeAllNeighbors();
			chunks[x][0][z].reset(
					new Chunk {worldName, x * CHUNK_WIDTH, z * CHUNK_DEPTH});
		}
	}

	// Connect the Chunks
	for (int x = 0; x < xMax; ++x) {
		for (int z = 0; z < zMax; ++z) {
			shared_ptr<Chunk> current = chunks[x][0][z];
			if (x != xMax - 1) {
				shared_ptr<Chunk> right = chunks[x + 1][0][z];
				current->setRightNeighbor(right);
				right->setLeftNeighbor(current);
			}
			if (z != zMax - 1) {
				shared_ptr<Chunk> back = chunks[x][0][z + 1];
				current->setBackNeighbor(back);
				back->setFrontNeighbor(current);
			}
			current->updateGraphics();
		}
	}

	for (int x = 0; x < xMax; x++) {
		for (int z = 0; z < zMax; z++) {
			chunks[x][0][z]->updateLightning();
			chunks[x][0][z]->updateGraphics();
		}
	}

}

void ChunkManager::saveWorld(std::string& worldName) {

	const int xMax = NUMBER_OF_CHUNKS_X;
	const int yMax = NUMBER_OF_CHUNKS_Y;
	const int zMax = NUMBER_OF_CHUNKS_Z;

	for (int x = 0; x < xMax; x++) {
		for (int z = 0; z < zMax; z++) {
			chunks[x][0][z]->storeChunk(worldName, x * CHUNK_WIDTH,
					z * CHUNK_DEPTH);
		}
	}
}

Voxel ChunkManager::getVoxel(int x, int y, int z) {
	// Used to avoid Division every time the function is called.
	static float xD = 1.0 / CHUNK_WIDTH;
	static float yD = 1.0 / CHUNK_HEIGHT;
	static float zD = 1.0 / CHUNK_DEPTH;

	int chunkX = x * xD;
	int chunkY = y * yD;
	int chunkZ = z * zD;

	int localX = x % CHUNK_WIDTH;
	int localY = y % CHUNK_HEIGHT;
	int localZ = z % CHUNK_DEPTH;

	return chunks[chunkX][chunkY][chunkZ]->getVoxel(localX, localY, localZ);
}

char ChunkManager::getCubeId(int x, int y, int z) {
	return getVoxel(x, y, z).id;
}

bool ChunkManager::isSolid(int x, int y, int z) {
	// TODO Implement
	return false;
}

bool ChunkManager::isAir(int x, int y, int z) {
	return getCubeId(x, y, z) == AIR;
}

void ChunkManager::removeCube(int x, int y, int z) {
	setCube(x, y, z, AIR);
}

void ChunkManager::setCube(int x, int y, int z, char id) {
	static float xD = 1.0 / CHUNK_WIDTH;
	static float yD = 1.0 / CHUNK_HEIGHT;
	static float zD = 1.0 / CHUNK_DEPTH;

	int chunkX = x * xD;
	int chunkY = y * yD;
	int chunkZ = z * zD;

	int localX = x % CHUNK_WIDTH;
	int localY = y % CHUNK_HEIGHT;
	int localZ = z % CHUNK_DEPTH;

	chunks[chunkX][chunkY][chunkZ]->setCube(localX, localY, localZ, id);
}

// Requires that direction is normalized!
// Has one bugg, when the player is exactly located at an integer position
// the selection will be wrong!
bool ChunkManager::intersectWithSolidCube(vec3 origin, vec3 direction,
		vec3 &intersected, vec3 &previous, float searchLength) {

	// Get the sign of the directions
	int signXDirection = (direction.x > 0) - (direction.x < 0);
	int signYDirection = (direction.y > 0) - (direction.y < 0);
	int signZDirection = (direction.z > 0) - (direction.z < 0);

	// Check if any one is zero
	float xL;
	float yL;
	float zL;

	if (direction.x == 0) {
		xL = 100000000; //signXDirection * std::numeric_limits<float>::infinity();
	} else {
		xL = 1.0 / direction.x;
	}

	if (direction.y == 0) {
		yL = 1000000000; //signYDirection * std::numeric_limits<float>::infinity();
	} else {
		yL = 1.0 / direction.y;
	}

	if (direction.z == 0) {
		zL = 1000000000; //signZDirection * std::numeric_limits<float>::infinity();
	} else {
		zL = 1.0 / direction.z;
	}

	int currentCubeX = floor(origin.x);
	int currentCubeY = floor(origin.y);
	int currentCubeZ = floor(origin.z);

	// Problem here :s
	// When the player is located exactly at an integer value ie 7.0 we get errors
	int xAxis = signXDirection > 0 ? ceil(origin.x) : floor(origin.x);
	int yAxis = signYDirection > 0 ? ceil(origin.y) : floor(origin.y);
	int zAxis = signZDirection > 0 ? ceil(origin.z) : floor(origin.z);

	float distanceSearched = 0;

	previous = vec3(currentCubeX, currentCubeY, currentCubeZ);

	while (distanceSearched < searchLength) {

		previous = vec3(currentCubeX, currentCubeY, currentCubeZ);

		// Multi means how much along an axis we need to move to pass into the next cube
		// Step into the next cube by going one step in the direction of the axis with the lowest multi.
		float multiX = (xAxis - origin.x) * xL;
		float multiY = (yAxis - origin.y) * yL;
		float multiZ = (zAxis - origin.z) * zL;

		if (multiX < multiY && multiX < multiZ) {

			currentCubeX += signXDirection;
			xAxis += signXDirection;
			distanceSearched = length(multiX * direction);

		} else if (multiY < multiZ) {

			currentCubeY += signYDirection;
			yAxis += signYDirection;
			distanceSearched = length(multiY * direction);

		} else {

			currentCubeZ += signZDirection;
			zAxis += signZDirection;
			distanceSearched = length(multiZ * direction);

		}

		if (!isAir(currentCubeX, currentCubeY, currentCubeZ)) {
			intersected = vec3(currentCubeX, currentCubeY, currentCubeZ);
			return true;
		}
	}

	return false;
}

}

