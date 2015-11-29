
#include "chunk.h"

#include <iostream>

#include "../../../graphics/chunkBatcher.h"

namespace chunk
{

// ########################################################
// Constructor/Destructor #################################
// ########################################################

static 	int counter = 1;
static const int maxCount = 4;

Chunk::Chunk(int x, int y, int z):
	xLocation{x},
	yLocation{y},
	zLocation{z}
{

	counter++;

	for (int i = 0; i < config::chunk_data::CHUNK_WIDTH; i++) {
		vec.push_back(std::vector<std::vector<Voxel>>());

		if (counter == maxCount)
			counter = 0;

		for (int j = 0; j < config::chunk_data::CHUNK_HEIGHT; j++) {
			vec[i].push_back(std::vector<Voxel>());

			for (int k = 0; k < config::chunk_data::CHUNK_DEPTH; k++) {
				Voxel v;
				v.lightValue = 0;

				if (j == 0) {
					v.id = config::cube_data::BED_ROCK;
					vec[i][j].push_back(v);
					continue;
				}

				if (j < 5) {
					v.id = counter;
					vec[i][j].push_back(v);
				} else {
					v.id = config::cube_data::AIR;
					vec[i][j].push_back(v);
				}
			}
		}
	}

	std::vector<glm::vec3> lightPropagate;
	doSunLightning(lightPropagate);
	for (glm::vec3 vec : lightPropagate)
		propagateLight(vec.x, vec.y, vec.z);

}

// ########################################################
// Member Functions########################################
// ########################################################

Voxel& Chunk::getVoxel(int x, int y, int z)
{
	return vec.at(x).at(y).at(z);
}

// TODO Duplicated from graphical chunk
Voxel* Chunk::getVoxel2(int x, int y, int z)
{
	if (x < width && x >= 0 && y < height && y >= 0 && z < depth && z >= 0) {
		return &vec[x][y][z];
	} else if (x == width && (y < height && y >= 0 && z < depth && z >= 0)) {
		if (rightNeighbor != nullptr) {
			return &(rightNeighbor.get()->vec[0][y][z]);
		} else {
			return nullptr;
		}
	} else if (x == -1 && (y < height && y >= 0 && z < depth && z >= 0)) {
		if (leftNeighbor != nullptr) {
			return &(leftNeighbor.get()->vec[width - 1][y][z]);
		} else {
			return nullptr;
		}
	} else if (z == depth && (x < width && x >= 0 && y < height && y >= 0)) {
		if (backNeighbor != nullptr) {
			return &(backNeighbor.get()->vec[x][y][0]);
		} else {
			return nullptr;
		}
	} else if (z == -1 && (x < width && x >= 0 && y < height && y >= 0)) {
		if (frontNeighbor != nullptr) {
			return &(frontNeighbor.get()->vec[x][y][depth - 1]);
		} else {
			return nullptr;
		}
	}

	return nullptr;
}

char Chunk::getCubeId(int x, int y, int z)
{
	return vec.at(x).at(y).at(z).id;
}

void Chunk::setCube(int x, int y, int z, char id)
{

	if (vec[x][y][z].id == config::cube_data::BED_ROCK)
		return;

	Voxel &voxel = vec[x][y][z];
	voxel.id = id;

	// If we remove a cube
	// Get light from neighbor an propagate
	if (id == config::cube_data::AIR) {
		updateLightningCubeRemoved(voxel, x, y, z);

	} else { // We added a cube

		updateLightningCubeAdded(x, y, z);
	}

}

void Chunk::updateLightningCubeRemoved(Voxel& voxel, int x, int y, int z)
{

	if (isInDirectSunlight(x, y, z)) {

		std::vector<glm::vec3> lightPropagate;
		doSunLightning(lightPropagate, x, y, z);
		for (auto x : lightPropagate)
			propagateLight(x.x, x.y, x.z);

	} else {

		int highestLightValue = 0;
		highestLightValue = highestLightValueFromNeighbors(x, y, z) - 1;
		if (highestLightValue < 0)
			highestLightValue = 0;

		voxel.lightValue = highestLightValue;
		propagateLight(x, y, z);
	}

	updateGraphics();
	// TODO Do more accurate check of which neighbors (if any) that needs to be updated
	updateNeighborGraphics();
}

void Chunk::updateLightningCubeAdded(int x, int y, int z)
{
	updateLightning2();
	updateGraphics();
	// TODO Do more accurate check of which neighbors (if any) that needs to be updated
	updateNeighborGraphics();
}

void Chunk::updateGraphics()
{

	std::vector<std::vector<std::vector<Voxel>>> *right = nullptr;
	std::vector<std::vector<std::vector<Voxel>>> *left = nullptr;
	std::vector<std::vector<std::vector<Voxel>>> *front = nullptr;
	std::vector<std::vector<std::vector<Voxel>>> *back = nullptr;

	if (rightNeighbor.get())
		right = &(rightNeighbor->vec);

	if (leftNeighbor.get())
		left = &(leftNeighbor->vec);

	if (frontNeighbor.get())
		front = &(frontNeighbor->vec);

	if (backNeighbor.get())
		back = &(backNeighbor->vec);

	graphics::ChunkBatcher::getInstance().removeBatch(graphicalChunk);
	graphicalChunk.reset(new graphics::GraphicalChunk(xLocation, yLocation, zLocation, vec,	right, left, back, front));
	graphics::ChunkBatcher::getInstance().addBatch(graphicalChunk);

}

void Chunk::updateNeighborGraphics()
{
	if (rightNeighbor.get()) {
		rightNeighbor->updateGraphics();

		if (rightNeighbor->backNeighbor.get())
			rightNeighbor->backNeighbor->updateGraphics();

		if (rightNeighbor->frontNeighbor.get())
			rightNeighbor->frontNeighbor->updateGraphics();

	}

	if (leftNeighbor.get()) {
		leftNeighbor->updateGraphics();

		if (leftNeighbor->backNeighbor.get())
			leftNeighbor->backNeighbor->updateGraphics();

		if (leftNeighbor->frontNeighbor.get())
			leftNeighbor->frontNeighbor->updateGraphics();

	}

	if (backNeighbor.get())
		backNeighbor->updateGraphics();


	if (frontNeighbor.get())
		frontNeighbor->updateGraphics();
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

void Chunk::doSunLightning(std::vector<glm::vec3> &lightPropagate)
{

	// TODO Not correct k/j problem
	// Sun lightning, only air gets light
	for (int i = 0; i < config::chunk_data::CHUNK_WIDTH; i++) {
		for (int j = 0; j < config::chunk_data::CHUNK_DEPTH; j++) {
			bool foundSolid = false;
			for (int k = config::chunk_data::CHUNK_HEIGHT-1; k >= 0; k--) {
				if (vec[i][k][j].id == config::cube_data::AIR) {

					if (foundSolid) {
						vec[i][k][j].lightValue = 0;
					} else {
						vec[i][k][j].lightValue = directSunlight;
						lightPropagate.push_back(glm::vec3(i,k,j));
					}
				} else {
					foundSolid = true;
				}
			}
		}
	}

}

void Chunk::doSunLightning(std::vector<glm::vec3> &lightPropagate, int x, int y, int z)
{
	for (int i = y; y >= 0; i--) {
		if (vec[x][i][z].id == config::cube_data::AIR) {
			vec[x][i][z].lightValue = directSunlight;
			lightPropagate.push_back(glm::vec3(x, i, z));
		} else {
			break;
		}
	}
}

void Chunk::updateLightning()
{

	std::vector<glm::vec3> lightPropagate;

	std::vector<glm::vec3> lightPropagateRight;
	std::vector<glm::vec3> lightPropagateRightBack;
	std::vector<glm::vec3> lightPropagateRightFront;

	std::vector<glm::vec3> lightPropagateLeft;
	std::vector<glm::vec3> lightPropagateLeftBack;
	std::vector<glm::vec3> lightPropagateLeftFront;

	std::vector<glm::vec3> lightPropagateBack;
	std::vector<glm::vec3> lightPropagateFront;

	doSunLightning(lightPropagate);

	// Sun light ####################################################

	if (rightNeighbor.get()) {
		rightNeighbor->doSunLightning(lightPropagateRight);

		if (rightNeighbor->backNeighbor.get())
			rightNeighbor->backNeighbor->doSunLightning(lightPropagateRightBack);

		if (rightNeighbor->frontNeighbor.get())
			rightNeighbor->frontNeighbor->doSunLightning(lightPropagateRightFront);

	}

	if (leftNeighbor.get()) {
		leftNeighbor->doSunLightning(lightPropagateLeft);

		if (leftNeighbor->backNeighbor.get())
			leftNeighbor->backNeighbor->doSunLightning(lightPropagateLeftBack);

		if (leftNeighbor->frontNeighbor.get())
			leftNeighbor->frontNeighbor->doSunLightning(lightPropagateLeftFront);

	}

	if (backNeighbor.get())
		backNeighbor->doSunLightning(lightPropagateBack);

	if (frontNeighbor.get())
		frontNeighbor->doSunLightning(lightPropagateFront);



	// Propagate light ##############################################

	for (glm::vec3 vec : lightPropagate)
		propagateLight(vec.x, vec.y, vec.z);

	if (rightNeighbor.get()) {
		rightNeighbor->collectLightFromRightNeighbor(lightPropagateRight);
		for (glm::vec3 vec : lightPropagateRight)
			rightNeighbor->propagateLight(vec.x, vec.y, vec.z);

		if (rightNeighbor->backNeighbor.get()) {
			rightNeighbor->backNeighbor->collectLightFromRightNeighbor(lightPropagateRightBack);
			rightNeighbor->backNeighbor->collectLightFromBackNeighbor(lightPropagateRightBack);

			for (glm::vec3 vec : lightPropagateRightBack)
				rightNeighbor->backNeighbor->propagateLight(vec.x, vec.y, vec.z);

		}

		if (rightNeighbor->frontNeighbor.get()) {
			rightNeighbor->frontNeighbor->collectLightFromRightNeighbor(lightPropagateRightFront);
			rightNeighbor->frontNeighbor->collectLightFromFrontNeighbor(lightPropagateRightFront);

			for (glm::vec3 vec : lightPropagateRightFront)
				rightNeighbor->frontNeighbor->propagateLight(vec.x, vec.y, vec.z);

		}
	}

	if (leftNeighbor.get()) {
		leftNeighbor->collectLightFromLeftNeighbor(lightPropagateLeft);
		for (glm::vec3 vec : lightPropagateLeft)
			leftNeighbor->propagateLight(vec.x, vec.y, vec.z);

		if (leftNeighbor->backNeighbor.get()) {
			leftNeighbor->backNeighbor->collectLightFromLeftNeighbor(lightPropagateLeftBack);
			leftNeighbor->backNeighbor->collectLightFromBackNeighbor(lightPropagateLeftBack);

			for (glm::vec3 vec : lightPropagateLeftBack)
				leftNeighbor->backNeighbor->propagateLight(vec.x, vec.y, vec.z);

		}

		if (leftNeighbor->frontNeighbor.get()) {
			leftNeighbor->frontNeighbor->collectLightFromLeftNeighbor(lightPropagateLeftFront);
			leftNeighbor->frontNeighbor->collectLightFromFrontNeighbor(lightPropagateLeftFront);

			for (glm::vec3 vec : lightPropagateLeftFront)
				leftNeighbor->frontNeighbor->propagateLight(vec.x, vec.y, vec.z);

		}
	}

	if (backNeighbor.get()) {
		backNeighbor->collectLightFromBackNeighbor(lightPropagateBack);
		for (glm::vec3 vec : lightPropagateBack)
			backNeighbor->propagateLight(vec.x, vec.y, vec.z);

	}

	if (frontNeighbor.get()) {
		frontNeighbor->collectLightFromFrontNeighbor(lightPropagateFront);
		for (glm::vec3 vec : lightPropagateFront)
			frontNeighbor->propagateLight(vec.x, vec.y, vec.z);

	}

	//updateGraphics();
	//updateNeighborGraphics();

}

void Chunk::updateLightning2()
{
	std::vector<glm::vec3> lightPropagate;
	doSunLightning(lightPropagate);

	if (rightNeighbor.get())
		collectLightFromRightNeighbor(lightPropagate);

	if (leftNeighbor.get())
		collectLightFromLeftNeighbor(lightPropagate);

	if (backNeighbor.get())
		collectLightFromBackNeighbor(lightPropagate);

	if (frontNeighbor.get())
		collectLightFromFrontNeighbor(lightPropagate);

	for (glm::vec3 vec : lightPropagate)
		propagateLight(vec.x, vec.y, vec.z);

}

void Chunk::collectLightFromRightNeighbor(std::vector<glm::vec3> &lightPropagate)
{
	if (rightNeighbor.get()) {
		for (int j = 0; j < config::chunk_data::CHUNK_HEIGHT; j++) {
			for (int k = 0; k < config::chunk_data::CHUNK_DEPTH; k++) {
				char lv = rightNeighbor->vec[0][j][k].lightValue - 1;

				if (rightNeighbor->vec[0][j][k].id == config::cube_data::AIR && vec[15][j][k].id == config::cube_data::AIR &&
						lv > vec[15][j][k].lightValue) {
					vec[15][j][k].lightValue = lv;
					lightPropagate.push_back(glm::vec3(15, j, k));
				}
			}
		}
	}
}

void Chunk::collectLightFromLeftNeighbor(std::vector<glm::vec3> &lightPropagate)
{
	if (leftNeighbor.get()) {
		for (int j = 0; j < config::chunk_data::CHUNK_HEIGHT; j++) {
			for (int k = 0; k < config::chunk_data::CHUNK_DEPTH; k++) {
				char lv = leftNeighbor->vec[15][j][k].lightValue - 1;

				if (leftNeighbor->vec[15][j][k].id == config::cube_data::AIR && vec[0][j][k].id == config::cube_data::AIR &&
						lv > vec[0][j][k].lightValue) {
					vec[0][j][k].lightValue = lv;
					lightPropagate.push_back(glm::vec3(0, j, k));
				}
			}
		}
	}
}

void Chunk::collectLightFromBackNeighbor(std::vector<glm::vec3> &lightPropagate)
{

	if (backNeighbor.get()) {

		for (int i = 0; i < config::chunk_data::CHUNK_WIDTH; i++) {
			for (int j = 0; j < config::chunk_data::CHUNK_HEIGHT; j++) {
				char lv = backNeighbor->vec[i][j][0].lightValue - 1;

				if (backNeighbor->vec[i][j][0].id == config::cube_data::AIR && vec[i][j][15].id == config::cube_data::AIR
						&& lv > vec[i][j][15].lightValue) {

					vec[i][j][15].lightValue = lv;
					lightPropagate.push_back(glm::vec3(i, j, 15));
				}
			}
		}
	}
}

void Chunk::collectLightFromFrontNeighbor(std::vector<glm::vec3> &lightPropagate)
{
	if (frontNeighbor.get()) {
		for (int i = 0; i < config::chunk_data::CHUNK_WIDTH; i++) {
			for (int j = 0; j < config::chunk_data::CHUNK_HEIGHT; j++) {
				char lv = frontNeighbor->vec[i][j][15].lightValue - 1;

				if (frontNeighbor->vec[i][j][15].id == config::cube_data::AIR && vec[i][j][0].id == config::cube_data::AIR &&
						lv > vec[i][j][0].lightValue) {

					vec[i][j][0].lightValue = lv;
					propagateLight(i, j, 0);

				}
			}
		}
	}
}

void Chunk::propagateLight(int x, int y, int z)
{

	Voxel &voxel = vec[x][y][z];
	int lvInitial = voxel.lightValue - 1;

	std::vector<glm::vec3> newPropagates;

	// ##################################################################################

	// Traverse right
	int lv = lvInitial;
	for (int i = x + 1; lv > 0; i++) {

		if (i < width) {
			Voxel &v = vec[i][y][z];
			if (v.id == config::cube_data::AIR && v.lightValue < lv) {
				v.lightValue = lv;
				newPropagates.push_back(glm::vec3(i, y, z));
				lv--;
			} else {
				break;
			}
		} else {
			if (rightNeighbor.get()) {
				if (rightNeighbor->vec[0][y][z].id == config::cube_data::AIR && rightNeighbor->vec[0][y][z].lightValue < lv) {
					rightNeighbor->vec[0][y][z].lightValue = lv;
					rightNeighbor->propagateLight(0, y, z);
				}
			}
			break;
		}
	}

	// Travers left
	lv = lvInitial;
	for (int i = x - 1; lv > 0; i--) {

		if (i >= 0) {
			Voxel &v = vec[i][y][z];
			if (v.id == config::cube_data::AIR && v.lightValue < lv) {
				v.lightValue = lv;
				newPropagates.push_back(glm::vec3(i, y, z));
				lv--;
			} else {
				break;
			}
		} else {
			if (leftNeighbor.get()) {
				if (leftNeighbor->vec[width - 1][y][z].id == config::cube_data::AIR && leftNeighbor->vec[width - 1][y][z].lightValue < lv) {
					leftNeighbor->vec[width - 1][y][z].lightValue = lv;
					leftNeighbor->propagateLight(width - 1, y, z);
				}
			}
			break;
		}
	}

	// ##################################################################################

	// Traverse up
	lv = lvInitial;
	for (int i = y + 1; i < height; i++) {
		Voxel &v = vec[x][i][z];
		if (v.id == config::cube_data::AIR && v.lightValue < lv) {
			v.lightValue = lv;
			newPropagates.push_back(glm::vec3(x, i, z));
			lv--;
		} else {
			break;
		}
	}

	// Treaverse down
	lv = lvInitial;
	for (int i = y - 1; i >= 0; i--) {
		Voxel &v = vec[x][i][z];
		if (v.id == config::cube_data::AIR && v.lightValue < lv) {
			v.lightValue = lv;
			newPropagates.push_back(glm::vec3(x, i, z));
			lv--;
		} else {
			break;
		}
	}

	// ##################################################################################

	// Traverse backwards
	lv = lvInitial;
	for (int i = z + 1; lv > 0; i++) {

		if (i < depth) {
			Voxel &v = vec[x][y][i];
			if (v.id == config::cube_data::AIR && v.lightValue < lv) {
				v.lightValue = lv;
				newPropagates.push_back(glm::vec3(x, y, i));
				lv--;
			} else {
				break;
			}
		} else {
			if (backNeighbor.get()) {
				if (backNeighbor->vec[x][y][0].id == config::cube_data::AIR && backNeighbor->vec[x][y][0].lightValue < lv) {
					backNeighbor->vec[x][y][0].lightValue = lv;
					backNeighbor->propagateLight(x, y, 0);
				}
			}
			break;
		}
	}

	// Traverse forwards
	lv = lvInitial;
	for (int i = z - 1; lv > 0; i--) {

		if (i >= 0) {

			Voxel &v = vec[x][y][i];
			if (v.id == config::cube_data::AIR && v.lightValue < lv) {
				v.lightValue = lv;
				newPropagates.push_back(glm::vec3(x, y, i));
				lv--;
			} else {
				break;
			}
		} else {
			if (frontNeighbor.get()) {
				if (frontNeighbor->vec[x][y][depth - 1].id == config::cube_data::AIR && frontNeighbor->vec[x][y][depth - 1].lightValue < lv) {
					frontNeighbor->vec[x][y][depth - 1].lightValue = lv;
					frontNeighbor->propagateLight(x, y, depth - 1);
				}
			}
			break;
		}
	}

	for (glm::vec3 vec : newPropagates)
		propagateLight(vec.x, vec.y, vec.z);

}

int Chunk::highestLightValueFromNeighbors(int x, int y, int z)
{
	int highestValue = -1;

	Voxel* v = getVoxel2(x + 1, y, z);
	if (v && v->lightValue > highestValue)
		highestValue = v->lightValue;

	v = getVoxel2(x - 1, y, z);
	if (v && v->lightValue > highestValue)
		highestValue = v->lightValue;

	v = getVoxel2(x , y + 1, z);
	if (v && v->lightValue > highestValue)
		highestValue = v->lightValue;

	v = getVoxel2(x, y - 1, z);
	if (v && v->lightValue > highestValue)
		highestValue = v->lightValue;

	v = getVoxel2(x, y, z + 1);
	if (v && v->lightValue > highestValue)
		highestValue = v->lightValue;

	v = getVoxel2(x, y, z - 1);
	if (v && v->lightValue > highestValue)
		highestValue = v->lightValue;

	return highestValue;
}

bool Chunk::isInDirectSunlight(int x, int y, int z)
{
	for (int i = height - 1; i >= y; i--)
		if (vec[x][i][z].id != config::cube_data::AIR)
			return false;

	return true;
}

}
