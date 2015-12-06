
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

	const int xMax = config::chunk_data::NUMBER_OF_CHUNKS_X;
	const int yMax = config::chunk_data::NUMBER_OF_CHUNKS_Y;
	const int zMax = config::chunk_data::NUMBER_OF_CHUNKS_Z;

	// Create the Chunks
	for (int x = 0; x < xMax; x++) {
		for (int z = 0; z < zMax; z++) {
			chunks[x][0][z] = std::unique_ptr<chunk::Chunk> (
				new chunk::Chunk{
					x * config::chunk_data::CHUNK_WIDTH,
					0 * config::chunk_data::CHUNK_HEIGHT,
					z * config::chunk_data::CHUNK_DEPTH});
		}
	}

	// Connect the Chunks
	for (int x = 0; x < xMax-1; x++) {
		for (int z = 0; z < zMax-1; z++) {
			std::shared_ptr<Chunk> right = chunks[x + 1][0][z];
			std::shared_ptr<Chunk> back = chunks[x][0][z + 1];

			std::shared_ptr<Chunk> current = chunks[x][0][z];

			current->setRightNeighbor(right);
			right->setLeftNeighbor(current);

			current->setBackNeighbor(back);
			back->setFrontNeighbor(current);

			current->updateGraphics();
		}
	}

}

// ########################################################
// Member Functions########################################
// ########################################################

char ChunkManager::getCubeId(int x, int y, int z)
{
	// Used to avoid Division every time the function is called.
	static float xD = 1.0 / config::chunk_data::CHUNK_WIDTH;
	static float yD = 1.0 / config::chunk_data::CHUNK_HEIGHT;
	static float zD = 1.0 / config::chunk_data::CHUNK_DEPTH;

	int chunkX = x * xD;
	int chunkY = y * yD;
	int chunkZ = z * zD;

	int localX = x % config::chunk_data::CHUNK_WIDTH;
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
	int chunkX = x / config::chunk_data::CHUNK_WIDTH;
	int chunkY = y / config::chunk_data::CHUNK_HEIGHT;
	int chunkZ = z / config::chunk_data::CHUNK_DEPTH;

	int localX = x % config::chunk_data::CHUNK_WIDTH;
	int localY = y % config::chunk_data::CHUNK_HEIGHT;
	int localZ = z % config::chunk_data::CHUNK_DEPTH;

	chunks[chunkX][chunkY][chunkZ]->setCube(localX, localY, localZ, id);
}

// Requires that direction is normalized!
// Has one bugg, when the player is exactly located at an integer position
// the selection will be wrong!

bool ChunkManager::intersectWithSolidCube(glm::vec3 origin, glm::vec3 direction, glm::vec3 &intersected, glm::vec3 &previous, float searchLength)
{

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

	int currentCubeX = std::floor(origin.x);
	int currentCubeY = std::floor(origin.y);
	int currentCubeZ = std::floor(origin.z);

	// Problem here :s
	// When the player is located exactly at an integer value ie 7.0 we get errors
	int xAxis = signXDirection > 0 ? std::ceil(origin.x) : std::floor(origin.x);
	int yAxis = signYDirection > 0 ? std::ceil(origin.y) : std::floor(origin.y);
	int zAxis = signZDirection > 0 ? std::ceil(origin.z) : std::floor(origin.z);

	float distanceSearched = 0;

	previous = glm::vec3(currentCubeX, currentCubeY, currentCubeZ);

	while (distanceSearched < searchLength) {

		previous = glm::vec3(currentCubeX, currentCubeY, currentCubeZ);

		// Multi means how much along an axis we need to move to pass into the next cube
		// Step into the next cube by going one step in the direction of the axis with the lowest multi.
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

		if (!isAir(currentCubeX, currentCubeY, currentCubeZ)) {
			intersected = glm::vec3(currentCubeX, currentCubeY, currentCubeZ);
			return true;
		}
	}

	return false;

}

}

