
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

	updateGraphics();

	if (rightNeighbor.get() != nullptr)
		rightNeighbor->updateGraphics();

	if (leftNeighbor.get() != nullptr)
		leftNeighbor->updateGraphics();

	if (frontNeighbor.get() != nullptr)
		frontNeighbor->updateGraphics();

	if (backNeighbor.get() != nullptr)
		backNeighbor->updateGraphics();
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

	if (rightNeighbor.get() != nullptr)
		rightNeighbor->doSunLightning();

	if (leftNeighbor.get() != nullptr)
		leftNeighbor->doSunLightning();

	if (frontNeighbor.get() != nullptr)
		frontNeighbor->doSunLightning();

	if (backNeighbor.get() != nullptr)
		backNeighbor->doSunLightning();

	/*
	graphics::ChunkBatcher::getInstance().removeBatch(graphicalChunk);
	graphicalChunk.reset(new graphics::GraphicalChunk(xLocation, yLocation, zLocation, voxels));
	graphics::ChunkBatcher::getInstance().addBatch(graphicalChunk);
	*/

	updateGraphics();

	if (rightNeighbor.get() != nullptr)
		rightNeighbor->updateGraphics();

	if (leftNeighbor.get() != nullptr)
		leftNeighbor->updateGraphics();

	if (frontNeighbor.get() != nullptr)
		frontNeighbor->updateGraphics();

	if (backNeighbor.get() != nullptr)
		backNeighbor->updateGraphics();

}

void Chunk::updateGraphics()
{
	graphics::ChunkBatcher::getInstance().removeBatch(graphicalChunk);
	graphicalChunk.reset(new graphics::GraphicalChunk(xLocation, yLocation, zLocation, voxels));
	graphics::ChunkBatcher::getInstance().addBatch(graphicalChunk);
}


void Chunk::setLeftNeighbor(std::shared_ptr<Chunk> chunk)
{
	leftNeighbor = chunk;
}

void Chunk::setRightNeighbor(std::shared_ptr<Chunk> chunk)
{
	rightNeighbor = chunk;
}


void Chunk::setFrontNeighbor(std::shared_ptr<Chunk> chunk)
{
	frontNeighbor = chunk;
}

void Chunk::setBackNeighbor(std::shared_ptr<Chunk> chunk)
{
	backNeighbor = chunk;
}


void Chunk::doSunLightning()
{
	// Cubes that should propagate their light
	std::vector<glm::vec3> lightPropagate;

	bool foundSolid = false;
	// Sun lightning, only air gets light
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

	for (glm::vec3 vec : lightPropagate)
		propagateLight(vec.x, vec.y, vec.z);

	lightPropagate.clear();

}

void Chunk::propagateLight(int x, int y, int z)
{

	Voxel &voxel = voxels[x][y][z];
	int lvInitial = voxel.lightValue - 1;

	std::vector<glm::vec3> newPropagates;


	int lv = lvInitial;
	for (int i = x + 1; lv > 0; i++) {

		if (i < 16) {
			Voxel &v = voxels[i][y][z];
			if (v.id == config::cube_data::AIR && v.lightValue < lv) {
				v.lightValue = lv;
				newPropagates.push_back(glm::vec3(i, y, z));
				lv--;
			} else {
				break;
			}
		} else {
			if (rightNeighbor.get() != nullptr) {
				if (rightNeighbor->voxels[0][y][z].lightValue < lv) {
					rightNeighbor->voxels[0][y][z].lightValue = lv;
					rightNeighbor->propagateLight(0, y, z);
				}
			}
			break;
		}
	}

	lv = lvInitial;
	for (int i = x - 1; lv > 0; i--) {

		if (i >= 0) {
			Voxel &v = voxels[i][y][z];
			if (v.id == config::cube_data::AIR && v.lightValue < lv) {
				v.lightValue = lv;
				newPropagates.push_back(glm::vec3(i, y, z));
				lv--;
			} else {
				break;
			}
		} else {
			if (leftNeighbor.get() != nullptr) {
				if (leftNeighbor->voxels[15][y][z].lightValue < lv) {
					leftNeighbor->voxels[15][y][z].lightValue = lv;
					leftNeighbor->propagateLight(15, y, z);
				}
			}
			break;
		}
	}

	// ##################################################################################

	// Stoopid

	lv = lvInitial;
	for (int i = y + 1; i < 16; i++) {
		Voxel &v = voxels[x][i][z];
		if (v.id == config::cube_data::AIR && v.lightValue < lv) {
			v.lightValue = lv;
			newPropagates.push_back(glm::vec3(x, i, z));
			lv--;
		} else {
			break;
		}
	}

	lv = lvInitial;
	for (int i = y - 1; i >= 0; i--) {
		Voxel &v = voxels[x][i][z];
		if (v.id == config::cube_data::AIR && v.lightValue < lv) {
			v.lightValue = lv;
			newPropagates.push_back(glm::vec3(x, i, z));
			lv--;
		} else {
			break;
		}
	}

	// ##################################################################################


	lv = lvInitial;
	for (int i = z + 1; lv > 0; i++) {

		if (i < 16) {
			Voxel &v = voxels[x][y][i];
			if (v.id == config::cube_data::AIR && v.lightValue < lv) {
				v.lightValue = lv;
				newPropagates.push_back(glm::vec3(x, y, i));
				lv--;
			} else {
				break;
			}
		} else {
			if (frontNeighbor.get() != nullptr) {
				if (frontNeighbor->voxels[x][y][0].lightValue < lv) {
					frontNeighbor->voxels[x][y][0].lightValue = lv;
					frontNeighbor->propagateLight(x, y, 0);
				}
			}
			break;
		}
	}

	lv = lvInitial;
	for (int i = z - 1; lv > 0; i--) {

		if (i >= 0) {

			Voxel &v = voxels[x][y][i];
			if (v.id == config::cube_data::AIR && v.lightValue < lv) {
				v.lightValue = lv;
				newPropagates.push_back(glm::vec3(x, y, i));
				lv--;
			} else {
				break;
			}
		} else {
			if (backNeighbor.get() != nullptr) {
				if (backNeighbor->voxels[x][y][15].lightValue < lv) {
					backNeighbor->voxels[x][y][15].lightValue = lv;
					backNeighbor->propagateLight(x, y, 15);
				}
			}
			break;
		}
	}

	// Consider postponing this until the all the propagate calls generated
	// By the doSunLightning function has completed, to avoid repeating work
	for (glm::vec3 vec : newPropagates) {
		propagateLight(vec.x, vec.y, vec.z);
	}

}

}
