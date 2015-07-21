
#include "chunk.h"

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Chunk::Chunk(int x, int y, int z):
	xLocation{x},
	yLocation{y},
	zLocation{z}
{
	//voxels = voxelData;

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

