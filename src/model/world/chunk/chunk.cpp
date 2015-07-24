
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

	for (int i = 0; i < chunk_data::CHUNK_WIDHT; i++) {
		for (int j = 0; j < chunk_data::CHUNK_HEIGHT; j++) {
			for (int k = 0; k < chunk_data::CHUNK_DEPTH; k++) {
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

