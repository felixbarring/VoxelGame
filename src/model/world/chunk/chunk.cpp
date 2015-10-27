
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

	bool foundSolid = false;
	// Sun lightning
	for (int i = 0; i < config::chunk_data::CHUNK_WIDHT; i++) {
		for (int j = 0; j < config::chunk_data::CHUNK_DEPTH; j++) {
			foundSolid = false;
			for (int k = config::chunk_data::CHUNK_HEIGHT-1; k >= 0; k--) {
				if (voxels[i][k][j].id == config::cube_data::AIR) {

					if (foundSolid) {
						voxels[i][k][j].lightValue = 0;
					} else {
						voxels[i][k][j].lightValue = 15;
						lightPropagate.push_back(glm::vec3(i,k,j));
					}
				} else {
					foundSolid = true;
				}
			}
		}
	}

	for (glm::vec3 vec : lightPropagate) {
		propagateLight(vec.x, vec.y, vec.z);
	}

}

void Chunk::propagateLight(int x, int y, int z)
{
	Voxel &voxel = voxels[x][y][z];
	int lvInitial = voxel.lightValue - 1;

	int lv = lvInitial;

	for (int i = x + 1; i < 16; i++) {
		Voxel &v = voxels[i][y][z];
		if (v.id == config::cube_data::AIR && v.lightValue < voxel.lightValue) {
			v.lightValue = lv;
			lv--;
		}
	}

	lv = lvInitial;

	for (int i = x - 1; i >= 0; i--) {
		Voxel &v = voxels[i][y][z];
		if (v.id == config::cube_data::AIR && v.lightValue < voxel.lightValue) {
			v.lightValue = lv;
			lv--;
		}
	}

	lv = lvInitial;

	for (int i = x + 1; i < 16; i++) {
		Voxel &v = voxels[x][y][i];
		if (v.id == config::cube_data::AIR && v.lightValue < voxel.lightValue) {
			v.lightValue = lv;
			lv--;
		}
	}

	lv = lvInitial;

	for (int i = x - 1; i >= 0; i--) {
		Voxel &v = voxels[x][y][i];
		if (v.id == config::cube_data::AIR && v.lightValue < voxel.lightValue) {
			v.lightValue = lv;
			lv--;
		}
	}

}

}
