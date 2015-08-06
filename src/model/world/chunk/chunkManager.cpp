
#include "chunkManager.h"

#include <math.h>
#include <limits>
#include <iostream>

#include "../../../util/voxel.h"

namespace chunk
{

// ########################################################
// Constructor/Destructor #################################
// ########################################################

ChunkManager::ChunkManager()
{

	// Currently creates only one chunk.

	int xMax = 1;
	int yMax = 1;
	int zMax = 1;
	for (int x = 0; x < xMax; x++) {
		for (int y = 0; y < yMax; y++) {
			for (int z = 0; z < zMax; z++) {
				chunks[x][y][z] = std::unique_ptr<chunk::Chunk> ( new chunk::Chunk{
					x * config::chunk_data::CHUNK_WIDHT, y * config::chunk_data::CHUNK_HEIGHT, z * config::chunk_data::CHUNK_DEPTH});
			}
		}
	}
}

ChunkManager::~ChunkManager()
{
}

// ########################################################
// Member Functions########################################
// ########################################################

char ChunkManager::getCubeId(int x, int y, int z)
{
	// Used to avoid Division every time the function is called.
	static float xD = 1.0 / config::chunk_data::CHUNK_WIDHT;
	static float yD = 1.0 / config::chunk_data::CHUNK_HEIGHT;
	static float zD = 1.0 / config::chunk_data::CHUNK_DEPTH;

	int chunkX = x * xD;
	int chunkY = y * yD;
	int chunkZ = z * zD;

	int localX = x % config::chunk_data::CHUNK_WIDHT;
	int localY = y % config::chunk_data::CHUNK_HEIGHT;
	int localZ = z % config::chunk_data::CHUNK_DEPTH;

	return chunks[chunkX][chunkY][chunkZ]->getCubeId(localX, localY, localZ);
}

bool ChunkManager::isSolid(int x, int y, int z)
{
	// TODO Implement
	return false;
}

bool ChunkManager::isAir(int x, int y, int z)
{
	return getCubeId(x,y,z) == config::cube_data::AIR;
}

void ChunkManager::removeCube(int x, int y, int z)
{
	setCube(x, y, z, config::cube_data::AIR);
}

void ChunkManager::setCube(int x, int y, int z, char id)
{
	int chunkX = x / config::chunk_data::CHUNK_WIDHT;
	int chunkY = y / config::chunk_data::CHUNK_HEIGHT;
	int chunkZ = z / config::chunk_data::CHUNK_DEPTH;

	int localX = x % config::chunk_data::CHUNK_WIDHT;
	int localY = y % config::chunk_data::CHUNK_HEIGHT;
	int localZ = z % config::chunk_data::CHUNK_DEPTH;

	chunks[chunkX][chunkY][chunkZ]->setCube(localX, localY, localZ, id);
}

// Requires that direction is normalized!

bool ChunkManager::intersectWithSolidCube(glm::vec3 origin, glm::vec3 direction, float searchLength)
{

	int signXDirection = (direction.x > 0) - (direction.x < 0);
	int signYDirection = (direction.y > 0) - (direction.y < 0);
	int signZDirection = (direction.z > 0) - (direction.z < 0);

	// Check if any one is zero, set to infinity to avoid zero division!
	float xL;
	float yL;
	float zL;
	if (direction.x == 0) {
		xL = signXDirection * std::numeric_limits<float>::infinity();
	} else {
		xL = 1.0 / direction.x;
	}

	if (direction.y == 0) {
		yL = signYDirection * std::numeric_limits<float>::infinity();
	} else {
		yL = 1.0 / direction.y;
	}

	if (direction.z == 0) {
		zL = signZDirection * std::numeric_limits<float>::infinity();
	} else {
		zL = 1.0 / direction.z;
	}

	int currentCubeX = std::floor(origin.x);
	int currentCubeY = std::floor(origin.y);
	int currentCubeZ = std::floor(origin.z);

	int xAxis = signXDirection >= 0 ? std::ceil(origin.x) : std::floor(origin.x);
	int yAxis = signYDirection >= 0 ? std::ceil(origin.y) : std::floor(origin.y);
	int zAxis = signZDirection >= 0 ? std::ceil(origin.z) : std::floor(origin.z);

	float distanceSearched = 0;

	while ( distanceSearched < searchLength) {

		//std::cout << "Search lenght = " << distanceSearched << "\n";

		if (!isAir(currentCubeX, currentCubeY, currentCubeZ)) {
			//std::cout << "--- Found None Air CUbe" << currentCubeX << " : " << currentCubeY << " : " << currentCubeZ << "\n";
			intersectedCube = glm::vec3(currentCubeX, currentCubeY, currentCubeZ);
			return true;
		}

		float multiX = (xAxis - origin.x) * xL;
		float multiY = (yAxis - origin.y) * yL;
		float multiZ = (zAxis - origin.z) * zL;

		if (multiX < multiY && multiX < multiZ){

			currentCubeX += signXDirection;
			xAxis += signXDirection;
			distanceSearched = glm::length(multiX * direction);

		} else if (multiY < multiZ) {

			currentCubeY += signYDirection;
			yAxis += signYDirection;
			distanceSearched = glm::length(multiY * direction);

		} else {

			currentCubeZ += signZDirection;
			zAxis += signZDirection;
			distanceSearched = glm::length(multiZ * direction);

		}
	}

	return false;
}

glm::vec3 ChunkManager::getLocationOfInteresectedCube()
{
	return intersectedCube;
}

}

