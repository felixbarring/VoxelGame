
#include "chunkManager.h"

#include "../../../voxel.h"

// ########################################################
// Constructor/Destructor #################################
// ########################################################

ChunkManager::ChunkManager()
{
	int xMax = 1;
	int yMax = 1;
	int zMax = 1;
	for (int x = 0; x < xMax; x++ ) {
		for (int y = 0; y < yMax; y++) {
			for (int z = 0; z < zMax; z++) {
				chunks[x][y][z] = std::unique_ptr<Chunk> ( new Chunk{x * chunk_data::CHUNK_WIDHT, y * chunk_data::CHUNK_HEIGHT, z * chunk_data::CHUNK_DEPTH});
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
	return 'l';
}

void ChunkManager::removeCube(int x, int y, int z)
{
	int chunkX = x / chunk_data::CHUNK_WIDHT;
	int chunkY = y / chunk_data::CHUNK_HEIGHT;
	int chunkZ = z / chunk_data::CHUNK_DEPTH;

	int localX = x % chunk_data::CHUNK_WIDHT;
	int localY = y % chunk_data::CHUNK_HEIGHT;
	int localZ = z % chunk_data::CHUNK_DEPTH;

	chunks[chunkX][chunkY][chunkZ]->removeCube(localX, localY, localZ);

}

void ChunkManager::addCube(int x, int y, int z, char id)
{

}
