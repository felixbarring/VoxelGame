
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

	for (int i = 0; i < config::chunk_data::CHUNK_WIDHT; i++) {
		vec.push_back(std::vector<std::vector<Voxel>>());

		//counter++;
		if (counter == maxCount)
			counter = 0;

		for (int j = 0; j < config::chunk_data::CHUNK_HEIGHT; j++) {
			vec[i].push_back(std::vector<Voxel>());

			for (int k = 0; k < config::chunk_data::CHUNK_DEPTH; k++) {
				Voxel v;
				v.lightValue = 0;
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

char Chunk::getCubeId(int x, int y, int z)
{
	return vec.at(x).at(y).at(z).id;
}

void Chunk::setCube(int x, int y, int z, char id)
{

	// TODO Consider: Optimize depending on if a cube was removed or added.

	Voxel &voxel = vec[x][y][z];
	voxel.id = id;

	// TODO Move all this to a different function

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

	if (rightNeighbor.get() != nullptr) {
		rightNeighbor->doSunLightning(lightPropagateRight);

		if (rightNeighbor->backNeighbor.get() != nullptr) {
			rightNeighbor->backNeighbor->doSunLightning(lightPropagateRightBack);
		}

		if (rightNeighbor->frontNeighbor.get() != nullptr) {
			rightNeighbor->frontNeighbor->doSunLightning(lightPropagateRightFront);
		}

	}

	if (leftNeighbor.get() != nullptr) {
		leftNeighbor->doSunLightning(lightPropagateLeft);

		if (leftNeighbor->backNeighbor.get() != nullptr) {
			leftNeighbor->backNeighbor->doSunLightning(lightPropagateLeftBack);
		}

		if (leftNeighbor->frontNeighbor.get() != nullptr) {
			leftNeighbor->frontNeighbor->doSunLightning(lightPropagateLeftFront);
		}

	}

	if (backNeighbor.get() != nullptr)
		backNeighbor->doSunLightning(lightPropagateBack);

	if (frontNeighbor.get() != nullptr)
		frontNeighbor->doSunLightning(lightPropagateFront);



	// Propagate light ##############################################


	for (glm::vec3 vec : lightPropagate)
		propagateLight(vec.x, vec.y, vec.z);

	if (rightNeighbor.get() != nullptr) {
		rightNeighbor->collectLightFromRightNeighbor(lightPropagateRight);
		for (glm::vec3 vec : lightPropagateRight)
			rightNeighbor->propagateLight(vec.x, vec.y, vec.z);

		if (rightNeighbor->backNeighbor.get() != nullptr) {
			rightNeighbor->backNeighbor->collectLightFromRightNeighbor(lightPropagateRightBack);
			rightNeighbor->backNeighbor->collectLightFromBackNeighbor(lightPropagateRightBack);

			for (glm::vec3 vec : lightPropagateRightBack)
				rightNeighbor->backNeighbor->propagateLight(vec.x, vec.y, vec.z);

		}

		if (rightNeighbor->frontNeighbor.get() != nullptr) {
			rightNeighbor->frontNeighbor->collectLightFromRightNeighbor(lightPropagateRightFront);
			rightNeighbor->frontNeighbor->collectLightFromFrontNeighbor(lightPropagateRightFront);

			for (glm::vec3 vec : lightPropagateRightFront)
				rightNeighbor->frontNeighbor->propagateLight(vec.x, vec.y, vec.z);

		}
	}

	if (leftNeighbor.get() != nullptr) {
		leftNeighbor->collectLightFromLeftNeighbor(lightPropagateLeft);
		for (glm::vec3 vec : lightPropagateLeft)
			leftNeighbor->propagateLight(vec.x, vec.y, vec.z);

		if (leftNeighbor->backNeighbor.get() != nullptr) {
			leftNeighbor->backNeighbor->collectLightFromLeftNeighbor(lightPropagateLeftBack);
			leftNeighbor->backNeighbor->collectLightFromBackNeighbor(lightPropagateLeftBack);

			for (glm::vec3 vec : lightPropagateLeftBack)
				leftNeighbor->backNeighbor->propagateLight(vec.x, vec.y, vec.z);

		}

		if (leftNeighbor->frontNeighbor.get() != nullptr) {
			leftNeighbor->frontNeighbor->collectLightFromLeftNeighbor(lightPropagateLeftFront);
			leftNeighbor->frontNeighbor->collectLightFromFrontNeighbor(lightPropagateLeftFront);

			for (glm::vec3 vec : lightPropagateLeftFront)
				leftNeighbor->frontNeighbor->propagateLight(vec.x, vec.y, vec.z);

		}
	}

	if (backNeighbor.get() != nullptr) {
		backNeighbor->collectLightFromBackNeighbor(lightPropagateBack);
		for (glm::vec3 vec : lightPropagateBack)
			backNeighbor->propagateLight(vec.x, vec.y, vec.z);

	}

	if (frontNeighbor.get() != nullptr) {
		frontNeighbor->collectLightFromFrontNeighbor(lightPropagateFront);
		for (glm::vec3 vec : lightPropagateFront) {
			frontNeighbor->propagateLight(vec.x, vec.y, vec.z);
		}

	}

	updateGraphics();

	// More shit to update here ;)

	if (rightNeighbor.get() != nullptr) {
		rightNeighbor->updateGraphics();

		//std::cout << "has right neighbor \n";

		if (rightNeighbor->backNeighbor.get() != nullptr)
			rightNeighbor->backNeighbor->updateGraphics();

		if (rightNeighbor->frontNeighbor.get() != nullptr)
			rightNeighbor->frontNeighbor->updateGraphics();

	}

	if (leftNeighbor.get() != nullptr) {
		leftNeighbor->updateGraphics();

		//std::cout << "has left neighbor \n";

		if (leftNeighbor->backNeighbor.get() != nullptr)
			leftNeighbor->backNeighbor->updateGraphics();

		if (leftNeighbor->frontNeighbor.get() != nullptr)
			leftNeighbor->frontNeighbor->updateGraphics();

	}

	if (backNeighbor.get() != nullptr) {
		//std::cout << "has back neighbor \n";

		backNeighbor->updateGraphics();
	}

	if (frontNeighbor.get() != nullptr) {
		//std::cout << "has front neighbor \n";

		frontNeighbor->updateGraphics();
	}

}

void Chunk::updateGraphics()
{

	std::vector<std::vector<std::vector<Voxel>>> *right = nullptr;
	std::vector<std::vector<std::vector<Voxel>>> *left = nullptr;
	std::vector<std::vector<std::vector<Voxel>>> *front = nullptr;
	std::vector<std::vector<std::vector<Voxel>>> *back = nullptr;

	if (rightNeighbor.get() != nullptr)
		right = &(rightNeighbor->vec);

	if (leftNeighbor.get() != nullptr)
		left = &(leftNeighbor->vec);

	if (frontNeighbor.get() != nullptr)
		front = &(frontNeighbor->vec);

	if (backNeighbor.get() != nullptr)
		back = &(backNeighbor->vec);

	graphics::ChunkBatcher::getInstance().removeBatch(graphicalChunk);
	graphicalChunk.reset(new graphics::GraphicalChunk(xLocation, yLocation, zLocation, vec,	right, left, back, front));
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

void Chunk::doSunLightning(std::vector<glm::vec3> &lightPropagate)
{
	// TODO Not correct k/j problem

	bool foundSolid = false;
	// Sun lightning, only air gets light
	for (int i = 0; i < config::chunk_data::CHUNK_WIDHT; i++) {
		for (int j = 0; j < config::chunk_data::CHUNK_DEPTH; j++) {
			foundSolid = false;
			for (int k = config::chunk_data::CHUNK_HEIGHT-1; k >= 0; k--) {
				if (vec[i][k][j].id == config::cube_data::AIR) {

					if (foundSolid) {
						vec[i][k][j].lightValue = 0;
					} else {
						vec[i][k][j].lightValue = 15;
						lightPropagate.push_back(glm::vec3(i,k,j));
					}
				} else {
					foundSolid = true;
				}
			}
		}
	}

}

void Chunk::collectLightFromRightNeighbor(std::vector<glm::vec3> &lightPropagate)
{
	if (rightNeighbor.get() != nullptr) {
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
	if (leftNeighbor.get() != nullptr) {
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



	if (backNeighbor.get() != nullptr) {

		for (int i = 0; i < config::chunk_data::CHUNK_WIDHT; i++) {
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
	if (frontNeighbor.get() != nullptr) {
		for (int i = 0; i < config::chunk_data::CHUNK_WIDHT; i++) {
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

	// TODO Remove all the hard coded values

	// ##################################################################################
	// Traverse right
	int lv = lvInitial;
	for (int i = x + 1; lv > 0; i++) {

		if (i < config::chunk_data::CHUNK_WIDHT) {
			Voxel &v = vec[i][y][z];
			if (v.id == config::cube_data::AIR && v.lightValue < lv) {
				v.lightValue = lv;
				newPropagates.push_back(glm::vec3(i, y, z));
				lv--;
			} else {
				break;
			}
		} else {
			if (rightNeighbor.get() != nullptr) {
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
			if (leftNeighbor.get() != nullptr) {
				if (leftNeighbor->vec[15][y][z].id == config::cube_data::AIR && leftNeighbor->vec[15][y][z].lightValue < lv) {
					leftNeighbor->vec[15][y][z].lightValue = lv;
					leftNeighbor->propagateLight(15, y, z);
				}
			}
			break;
		}
	}

	// ##################################################################################

	// Traverse up
	lv = lvInitial;
	for (int i = y + 1; i < 16; i++) {
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

		if (i < 16) {
			Voxel &v = vec[x][y][i];
			if (v.id == config::cube_data::AIR && v.lightValue < lv) {
				v.lightValue = lv;
				newPropagates.push_back(glm::vec3(x, y, i));
				lv--;
			} else {
				break;
			}
		} else {
			if (backNeighbor.get() != nullptr) {
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
			if (frontNeighbor.get() != nullptr) {
				if (frontNeighbor->vec[x][y][15].id == config::cube_data::AIR && frontNeighbor->vec[x][y][15].lightValue < lv) {
					frontNeighbor->vec[x][y][15].lightValue = lv;
					frontNeighbor->propagateLight(x, y, 15);
				}
			}
			break;
		}
	}

	for (glm::vec3 vec : newPropagates)
		propagateLight(vec.x, vec.y, vec.z);

}

}
