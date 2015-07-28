
#include "chunk.h"

#include "../../../graphics/chunkBatcher.h"

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Chunk::Chunk(int x, int y, int z):
	xLocation{x},
	yLocation{y},
	zLocation{z}
{
	int counter = 1;
	const int maxCount = 4;

	for (int i = 0; i < config::chunk_data::CHUNK_WIDHT; i++) {
		for (int j = 0; j < config::chunk_data::CHUNK_HEIGHT; j++) {
			for (int k = 0; k < config::chunk_data::CHUNK_DEPTH; k++) {
				Voxel v;
				v.id = counter++;
				voxels[i][j][k] = v;
				if (counter == maxCount) {
					counter = 1;
				}
			}
		}
	}

	graphicalChunk.reset(new GraphicalChunk(x,y,z, voxels));
	ChunkBatcher::getInstance().addBatch(graphicalChunk);
}

Chunk::~Chunk()
{
}

// ########################################################
// Member Functions########################################
// ########################################################

char Chunk::getCubeId(int x, int y, int z)
{
	return voxels[x][y][z].id;
}


bool Chunk::isSolid(int x, int y, int z)
{
	// TODO Implement correctly
	return true;
}

void Chunk::setCube(int x, int y, int z, char id)
{
	Voxel &voxel = voxels[x][y][z];
	if (voxel.id != id) {
		ChunkBatcher::getInstance().removeBatch(graphicalChunk);
		voxel.id = id;
		graphicalChunk.reset(new GraphicalChunk(xLocation, yLocation, zLocation, voxels));
		ChunkBatcher::getInstance().addBatch(graphicalChunk);
	}
}
