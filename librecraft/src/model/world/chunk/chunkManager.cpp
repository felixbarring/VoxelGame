#include "chunkManager.h"

#include <math.h>
#include <algorithm>
#include <chrono>
#include <limits>
#include <iostream>
#include <thread>

#include "../../../util/voxel.h"

using namespace std;
using namespace glm;

using namespace config::chunk_data;
using namespace config::cube_data;

namespace chunk {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

// ########################################################
// Member Functions########################################
// ########################################################

void ChunkManager::createWorld(string worldName) {

	m_worldName = worldName;

	const int xMax = NUMBER_OF_CHUNKS_FROM_MIDDLE_TO_BORDER * 2 + 1;
	const int yMax = NUMBER_OF_CHUNKS_Y;
	const int zMax = NUMBER_OF_CHUNKS_FROM_MIDDLE_TO_BORDER * 2 + 1;

	vector<future<void>> chunkCreationFutures;

	// Create the Chunks
	for (int x = 0; x < xMax; ++x) {
		for (int z = 0; z < zMax; ++z) {
			auto chunk = new Chunk{worldName, x * CHUNK_WIDTH_AND_DEPTH, z * CHUNK_WIDTH_AND_DEPTH};

			chunkCreationFutures.push_back(m_threadPool.enqueue([chunk]
			{
				chunk->create();
				chunk->doSunLightning();
			} ));
			chunks[x][0][z].reset(chunk);
		}
	}

	connectChunks();

	// Wait for all the chunks to be generated
	for_each(chunkCreationFutures.begin(), chunkCreationFutures.end(), [](future<void> &f){ f.get(); });

	for (int x = 0; x < xMax; ++x) {
		for (int z = 0; z < zMax; ++z)
			chunks[x][0][z]->collectLightFromAllNeighbors();
	}

	for (int x = 0; x < xMax; ++x) {
		for (int z = 0; z < zMax; ++z) {
			// This could also be done in parallel if we are clever
			chunks[x][0][z]->propagateLights();
			chunks[x][0][z]->updateGraphics();
		}
	}

}

void ChunkManager::saveWorld() {
	const int xMax = NUMBER_OF_CHUNKS_FROM_MIDDLE_TO_BORDER;
	const int yMax = NUMBER_OF_CHUNKS_Y;
	const int zMax = NUMBER_OF_CHUNKS_FROM_MIDDLE_TO_BORDER;

	for (int x = 0; x < xMax; x++) {
		for (int z = 0; z < zMax; z++) {
			chunks[x][0][z]->storeChunk(m_worldName);
		}
	}
}

Voxel ChunkManager::getVoxel(int x, int y, int z) {
	// Used to avoid Division every time the function is called.
	static float xD = 1.0 / CHUNK_WIDTH_AND_DEPTH;
	static float yD = 1.0 / CHUNK_HEIGHT;
	static float zD = 1.0 / CHUNK_WIDTH_AND_DEPTH;

	x += m_xOffset;
	z += m_zOffset;

	int chunkX = x * xD;
	int chunkY = y * yD;
	int chunkZ = z * zD;

	int localX = x % CHUNK_WIDTH_AND_DEPTH;
	int localY = y % CHUNK_HEIGHT;
	int localZ = z % CHUNK_WIDTH_AND_DEPTH;

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
	static float xD = 1.0 / CHUNK_WIDTH_AND_DEPTH;
	static float yD = 1.0 / CHUNK_HEIGHT;
	static float zD = 1.0 / CHUNK_WIDTH_AND_DEPTH;

	x += m_xOffset;
	z += m_zOffset;

	int chunkX = x * xD;
	int chunkY = y * yD;
	int chunkZ = z * zD;

	int localX = x % CHUNK_WIDTH_AND_DEPTH;
	int localY = y % CHUNK_HEIGHT;
	int localZ = z % CHUNK_WIDTH_AND_DEPTH;

	chunks[chunkX][chunkY][chunkZ]->setCube(localX, localY, localZ, id);
}

void ChunkManager::setCenter(float x, float z)
{

	if (x < NUMBER_OF_CHUNKS_FROM_MIDDLE_TO_BORDER * 16 - m_xOffset)
		moveChunksRight();

	if (x > NUMBER_OF_CHUNKS_FROM_MIDDLE_TO_BORDER * 16 - m_xOffset + 16)
		moveChunksLeft();

	if (z < NUMBER_OF_CHUNKS_FROM_MIDDLE_TO_BORDER * 16 - m_zOffset)
		moveChunksUp();

	if (z > NUMBER_OF_CHUNKS_FROM_MIDDLE_TO_BORDER * 16 - m_zOffset + 16)
		moveChunksDown();

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

	if (direction.x == 0)
		xL = 100000000; //signXDirection * std::numeric_limits<float>::infinity();
	else
		xL = 1.0 / direction.x;

	if (direction.y == 0)
		yL = 1000000000; //signYDirection * std::numeric_limits<float>::infinity();
	else
		yL = 1.0 / direction.y;

	if (direction.z == 0)
		zL = 1000000000; //signZDirection * std::numeric_limits<float>::infinity();
	else
		zL = 1.0 / direction.z;

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


void ChunkManager::connectChunks() {

	const int xMax = m_lenghtAcrossMatrix;
	const int yMax = m_lenghtAcrossMatrix;
	const int zMax = m_lenghtAcrossMatrix;

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
		}
	}
}

// TODO Connect the chunks when they have been loaded

void ChunkManager::moveChunksRight() {
	m_xOffset += CHUNK_WIDTH_AND_DEPTH;

	// First disconnect all the rightmost chunks and store them to disk
	for (int i = 0; i < m_lenghtAcrossMatrix; ++i) {
		auto chunk = chunks[m_lenghtAcrossMatrix - 1][0][i];
		chunk->removeAllNeighbors();
		chunk->storeChunk(m_worldName);
	}

	// Then move all chunks one step to the right
	for (int i = m_lenghtAcrossMatrix - 1; i > 0; --i) {
		for (int j = 0; j < m_lenghtAcrossMatrix; ++j)
			chunks[i][0][j] = chunks[i - 1][0][j];
	}

	m_threadPool2.enqueue([this]{

		vector<future<void>> chunkCreationFutures;

		// Create new chunks for the left row
		for (int i = 0; i < m_lenghtAcrossMatrix; ++i) {
			auto ch = chunks[0 + 1][0][i];
			auto chunk = std::make_shared<Chunk>(m_worldName, ch->getXLocation() - CHUNK_WIDTH_AND_DEPTH,
					ch->getZLocation());

			chunkCreationFutures.push_back(m_threadPool.enqueue([chunk]
			{
				chunk->create();
				chunk->doSunLightning();
			} ));
			chunks[0][0][i] = chunk;
		}

		connectChunks();

		for_each(chunkCreationFutures.begin(), chunkCreationFutures.end(), [](future<void> &f) { f.get(); });

		for (int i = 0; i < m_lenghtAcrossMatrix; ++i)
			chunks[0][0][i]->collectLightFromRightNeighbor();

		for (int i = 0; i < m_lenghtAcrossMatrix; ++i) {
			// TODO Do this in threadpools...
			auto chunk = chunks[0][0][i];
			chunk->propagateLights();
			chunk->updateGraphics();
		}

	});
}

void ChunkManager::moveChunksLeft() {
	m_xOffset -= CHUNK_WIDTH_AND_DEPTH;

	// First disconnect all the leftmost chunks
	// and store them to disk
	for (int i = 0; i < m_lenghtAcrossMatrix; ++i) {
		auto chunk = chunks[0][0][i];
		chunk->removeAllNeighbors();
		chunk->storeChunk(m_worldName);
	}

	// Then move all chunks one step to the left
	for (int i = 0; i < m_lenghtAcrossMatrix - 1; ++i) {
		for (int j = 0; j < m_lenghtAcrossMatrix; ++j)
			chunks[i][0][j] = chunks[i + 1][0][j];
	}

	m_threadPool.enqueue([this]{

		vector<future<void>> chunkCreationFutures;

		// Create new chunks for the right row
		for (int i = 0; i < m_lenghtAcrossMatrix; ++i) {
			auto ch = chunks[m_lenghtAcrossMatrix - 2][0][i];
			auto chunk = std::make_shared<Chunk>(m_worldName, ch->getXLocation() + CHUNK_WIDTH_AND_DEPTH,
					ch->getZLocation());
			chunkCreationFutures.push_back(m_threadPool.enqueue([chunk]
			{
				chunk->create();
				chunk->doSunLightning();
			} ));
			chunks[m_lenghtAcrossMatrix - 1][0][i] = chunk;
		}

		connectChunks();

		for_each(chunkCreationFutures.begin(), chunkCreationFutures.end(), [] (future<void> &f) { f.get(); });

		for (int i = 0; i < m_lenghtAcrossMatrix; ++i)
			chunks[m_lenghtAcrossMatrix - 1][0][i]->collectLightFromLeftNeighbor();

		for (int i = 0; i < m_lenghtAcrossMatrix; ++i) {
			auto chunk = chunks[m_lenghtAcrossMatrix - 1][0][i];
			chunk->propagateLights();
			chunk->updateGraphics();
		}

	});
}

void ChunkManager::moveChunksUp() {
	m_zOffset += CHUNK_WIDTH_AND_DEPTH;

	// First disconnect all the top chunks
	// and store them to disk
	for (int i = 0; i < m_lenghtAcrossMatrix; ++i) {
		auto chunk = chunks[i][0][m_lenghtAcrossMatrix - 1];
		chunk->removeAllNeighbors();
		chunk->storeChunk(m_worldName);
	}

	// Then move all chunks one step up
	for (int i = 0; i < m_lenghtAcrossMatrix; ++i) {
		for (int j = m_lenghtAcrossMatrix - 1; j > 0; --j) {
			chunks[i][0][j] = chunks[i][0][j - 1];
		}
	}

	m_threadPool.enqueue([this] {

		vector<future<void>> chunkCreationFutures;

		// Create / load new chunks for the bottom row
		for (int i = 0; i < m_lenghtAcrossMatrix; ++i) {
			auto ch = chunks[i][0][0 + 1];
			auto chunk = std::make_shared<Chunk>(m_worldName, ch->getXLocation(), ch->getZLocation() -
					CHUNK_WIDTH_AND_DEPTH);
			chunkCreationFutures.push_back(m_threadPool.enqueue([chunk]
			{
				chunk->create();
				chunk->doSunLightning();
			} ));
			chunks[i][0][0] = chunk;
		}

		connectChunks();

		for_each(chunkCreationFutures.begin(), chunkCreationFutures.end(), [] (future<void> &f) { f.get(); });

		for (int i = 0; i < m_lenghtAcrossMatrix; ++i)
			chunks[i][0][0]->collectLightFromBackNeighbor();

		for (int i = 0; i < m_lenghtAcrossMatrix; ++i) {
			auto chunk = chunks[i][0][0];
			chunk->propagateLights();
			chunk->updateGraphics();
		}
	});
}

void ChunkManager::moveChunksDown() {
	m_zOffset -= CHUNK_WIDTH_AND_DEPTH;

	// First disconnect all the bottom chunks
	// and store them to disk
	for (int i = 0; i < m_lenghtAcrossMatrix; ++i) {
		auto chunk = chunks[i][0][0];
		chunk->removeAllNeighbors();
		chunk->storeChunk(m_worldName);
	}

	// Then move all chunks one step down
	for (int i = 0; i < m_lenghtAcrossMatrix; ++i) {
		for (int j = 0; j < m_lenghtAcrossMatrix - 1; ++j)
			chunks[i][0][j] = chunks[i][0][j + 1];
	}

	m_threadPool.enqueue([this] {

		vector<future<void>> chunkCreationFutures;

		// Create / load new chunks for the top row
		for (int i = 0; i < m_lenghtAcrossMatrix; ++i) {
			auto ch = chunks[i][0][m_lenghtAcrossMatrix - 2];
			auto chunk = std::make_shared<Chunk>(m_worldName, ch->getXLocation(), ch->getZLocation() +
					CHUNK_WIDTH_AND_DEPTH);
			chunkCreationFutures.push_back(m_threadPool.enqueue([chunk]
			{
				chunk->create();
				chunk->doSunLightning();
			} ));
			chunks[i][0][m_lenghtAcrossMatrix - 1] = chunk;
		}

		connectChunks();

		for_each(chunkCreationFutures.begin(), chunkCreationFutures.end(), [] (future<void> &f) { f.get(); });

		for (int i = 0; i < m_lenghtAcrossMatrix; ++i)
			chunks[i][0][m_lenghtAcrossMatrix - 1]->collectLightFromFrontNeighbor();

		for (int i = 0; i < m_lenghtAcrossMatrix; ++i) {
			auto chunk = chunks[i][0][m_lenghtAcrossMatrix - 1];
			chunk->propagateLights();
			chunk->updateGraphics();
		}
	});

}

}

