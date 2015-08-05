
#include "chunkManager.h"

#include <math.h>
#include <iostream>

#include "../../../util/voxel.h"

namespace chunk
{

// ########################################################
// Constructor/Destructor #################################
// ########################################################

ChunkManager::ChunkManager()
{
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
	int chunkX = x / config::chunk_data::CHUNK_WIDHT;
	int chunkY = y / config::chunk_data::CHUNK_HEIGHT;
	int chunkZ = z / config::chunk_data::CHUNK_DEPTH;

	int localX = x % config::chunk_data::CHUNK_WIDHT;
	int localY = y % config::chunk_data::CHUNK_HEIGHT;
	int localZ = z % config::chunk_data::CHUNK_DEPTH;

	//std::cout<<" : " << x << " : " << y << " : "<< z << "\n";

	/*
	std::cout << chunkX;
	std::cout << chunkY;
	std::cout << chunkZ;
	std::cout << "\n";
	*/

	return chunks[0][0][0]->getCubeId(localX, localY, localZ);
}

bool ChunkManager::isSolid(int x, int y, int z)
{
	int chunkX = x / config::chunk_data::CHUNK_WIDHT;
	int chunkY = y / config::chunk_data::CHUNK_HEIGHT;
	int chunkZ = z / config::chunk_data::CHUNK_DEPTH;

	int localX = x % config::chunk_data::CHUNK_WIDHT;
	int localY = y % config::chunk_data::CHUNK_HEIGHT;
	int localZ = z % config::chunk_data::CHUNK_DEPTH;

	return chunks[chunkX][chunkY][chunkZ]->isSolid(localX, localY, localZ);
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

// Require that direction is normalized!

glm::vec3 ChunkManager::getLocationOfFirstNoneAirCube(glm::vec3 origin, glm::vec3 direction, int searchLength)
{
	int signXDirection = (direction.x > 0) - (direction.x < 0);
	int signYDirection = (direction.y > 0) - (direction.y < 0);
	int signZDirection = (direction.z > 0) - (direction.z < 0);

	int currentCubeX = std::floor(origin.x);
	int currentCubeY = std::floor(origin.y);
	int currentCubeZ = std::floor(origin.z);

	int xAxis = signXDirection >= 0 ? std::ceil(origin.x) : std::floor(origin.x);
	int yAxis = signYDirection >= 0 ? std::ceil(origin.y) : std::floor(origin.y);
	int zAxis = signZDirection >= 0 ? std::ceil(origin.z) : std::floor(origin.z);

	for (int i = 0; i < searchLength; i++) {

		if (!isAir(currentCubeX, currentCubeY, currentCubeZ)) {
			std::cout << "--- Found Solid Block " << currentCubeX << " : " << currentCubeY << " : " << currentCubeZ << "\n";
			break;
		}

		/*
		std::cout << currentCubeX << " : " << currentCubeY << " : " << currentCubeZ << "\n";
		std::cout << signXDirection << " : " << signYDirection << " : " << signZDirection << "\n";
		std::cout << origin.x << " : " << origin.y << " : " << origin.z << "\n";
		std::cout << " X AXIS " << xAxis << "\n";
		std::cout << " Y AXIS " << yAxis << "\n";
		std::cout << " Z AXIS " << zAxis << "\n \n";
		*/

		float distanceToXAxis = xAxis - origin.x;
		float distanceToYAxis = yAxis - origin.y;
		float distanceToZAxis = zAxis - origin.z;

		/*
		std::cout << " TO PASS X AXIS " << distanceToXAxis << "\n";
		std::cout << " TO PASS Y AXIS " << distanceToYAxis << "\n";
		std::cout << " TO PASS Z AXIS " << distanceToZAxis << "\n\n";
		*/

		float multiplyerToReachXAxis = distanceToXAxis / direction.x;
		float multiplyerToReachYAxis = distanceToYAxis / direction.y;
		float multiplyerToReachZAxis = distanceToZAxis / direction.z;

		/*
		std::cout << " Multi X AXIS " << multiplyerToReachXAxis << "\n";
		std::cout << " Multi Y AXIS " << multiplyerToReachYAxis << "\n";
		std::cout << " Multi Z AXIS " << multiplyerToReachZAxis << "\n";
		*/

		if (multiplyerToReachXAxis < multiplyerToReachYAxis || multiplyerToReachXAxis < multiplyerToReachZAxis) {
			// Go the the right
			currentCubeX += signXDirection;
			xAxis += signXDirection;

		} else if (multiplyerToReachYAxis > multiplyerToReachZAxis) {
			// Go up
			currentCubeY += signYDirection;
			yAxis += signYDirection;
		} else {
			// Go in depth
			currentCubeZ += signZDirection;
			zAxis += signZDirection;
		}

	}

	return glm::vec3{1,0,1};
}

}

