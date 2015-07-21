
#include "chunkManager.h"

#include "../../../voxel.h"

// ########################################################
// Constructor/Destructor #################################
// ########################################################

ChunkManager::ChunkManager()
{
	int counter = 1;
	const int maxCount = 4;

	Voxel data[16][16][16];
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			for (int k = 0; k < 16; k++) {
				Voxel v;
				v.id = counter++;
				data[i][j][k] = v;
				if (counter == maxCount) {
					counter = 1;
				}
			}
		}
	}

	for (int i = 0; i < chunk_data::NUMBER_OF_CHUNKS_X; i++) {
		for (int j = 0; j < chunk_data::NUMBER_OF_CHUNKS_Y; j++) {
			chunks[i][j] = Chunk{i, j, 0,data};
		}
	}
}

ChunkManager::~ChunkManager()
{
}

// ########################################################
// Member Functions########################################
// ########################################################
