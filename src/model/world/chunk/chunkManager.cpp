
#include "chunkManager.h"

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

	return 'l';
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

}
