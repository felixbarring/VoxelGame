
#include "chunk.h"
//#include "../../../config/data.h"

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Chunk::Chunk(int x, int y, int z, Voxel voxelData[chunk_data::CHUNK_WIDHT][chunk_data::CHUNK_WIDHT][chunk_data::CHUNK_WIDHT]):
	xLocation{x},
	yLocation{y},
	zLocation{z},
	graphicalChunk{x, y, z, voxelData}
{
	for (int i = 0; i < chunk_data::CHUNK_WIDHT; i++) {
		for (int j = 0; j < chunk_data::CHUNK_HEIGHT; j++) {
			for (int k = 0; k < chunk_data::CHUNK_DEPTH; k++) {
				Voxel v;
				v.id = cube_data::GRASS;
			}
		}
	}

}

Chunk::~Chunk()
{
}

