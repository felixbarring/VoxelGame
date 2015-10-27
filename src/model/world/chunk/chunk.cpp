
#include "chunk.h"

#include "../../../graphics/chunkBatcher.h"

namespace chunk
{

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Chunk::Chunk(int x, int y, int z):
	xLocation{x},
	yLocation{y},
	zLocation{z}
{
	int counter = 1;
	const int maxCount = 7;

	for (int i = 0; i < config::chunk_data::CHUNK_WIDHT; i++) {
		for (int j = 0; j < config::chunk_data::CHUNK_HEIGHT; j++) {
			for (int k = 0; k < config::chunk_data::CHUNK_DEPTH; k++) {
				Voxel v;
				v.lightValue = 0;
				if (j < 5) {
					v.id = counter++;
					voxels[i][j][k] = v;
					if (counter == maxCount) {
						counter = 0;
					}
				} else {
					v.id = config::cube_data::AIR;
					voxels[i][j][k] = v;
				}
			}
		}
	}

	doSunLightning();

	graphicalChunk.reset(new graphics::GraphicalChunk(x,y,z, voxels));
	graphics::ChunkBatcher::getInstance().addBatch(graphicalChunk);
}


// ########################################################
// Member Functions########################################
// ########################################################

char Chunk::getCubeId(int x, int y, int z)
{
	return voxels[x][y][z].id;
}

void Chunk::setCube(int x, int y, int z, char id)
{
	Voxel &voxel = voxels[x][y][z];
	voxel.id = id;

	doSunLightning();

	graphics::ChunkBatcher::getInstance().removeBatch(graphicalChunk);
	graphicalChunk.reset(new graphics::GraphicalChunk(xLocation, yLocation, zLocation, voxels));
	graphics::ChunkBatcher::getInstance().addBatch(graphicalChunk);
}

void Chunk::doSunLightning()
{

	int lv = 15;

	// Sun lightning
	for (int i = 0; i < config::chunk_data::CHUNK_WIDHT; i++) {
		for (int j = 0; j < config::chunk_data::CHUNK_DEPTH; j++) {
			lv = 15;
			for (int k = config::chunk_data::CHUNK_HEIGHT-1; k >= 0; k--) {
				if (voxels[i][k][j].id == config::cube_data::AIR) {
					voxels[i][k][j].lightValue = lv;
				} else {
					lv = 0;
					//break;
				}
			}
		}
	}
}

}
