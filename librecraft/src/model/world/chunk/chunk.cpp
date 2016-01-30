
#include "chunk.h"

#include <iostream>
#include <fstream>

#include "../../../graphics/chunkBatcher.h"

using namespace std;
using namespace glm;

using namespace graphics;
using namespace config::cube_data;
using namespace config::chunk_data;

namespace chunk
{

// ########################################################
// Constructor/Destructor #################################
// ########################################################

static 	int counter = 1;
static const int maxCount = LAST_CUBE;

Chunk::Chunk(int x,int z):
	xLocation{x},
	zLocation{z}
{

	counter++;

	for (int i = 0; i < width; i++) {
		vec.push_back(vector<vector<Voxel>>());

		if (counter > maxCount)
			counter = 0;

		for (int j = 0; j < height; j++) {
			vec[i].push_back(vector<Voxel>());

			for (int k = 0; k < depth; k++) {
				Voxel v;
				v.lightValue = 0;

				if (j == 0) {
					v.id = BED_ROCK;
					vec[i][j].push_back(v);
					continue;
				}

				if (j < 5) {
					v.id = counter;
					vec[i][j].push_back(v);
				} else {
					v.id = AIR;
					vec[i][j].push_back(v);
				}
			}
		}
	}

	vector<vec3> lightPropagate;
	doSunLightning(lightPropagate);

	for (vec3 vec : lightPropagate)
		propagateLight(vec.x, vec.y, vec.z);


}

Chunk::Chunk(std::string name, int x, int z):
	xLocation{x},
	zLocation{z}
{
	vector<string> list;
	ifstream inStream;
	string line;

	string file = config::dataFolder + name + "_" +
			std::to_string(x) + "_" + std::to_string(z) + ".chunk";
	cout << file << "\n";

	inStream.open(file);

	// Add all lines to the vector
	while (getline(inStream, line))
		list.push_back(line);

	inStream.close();

	int counter = 0;
	for (int i = 0; i < width; i++) {
		vec.push_back(vector<vector<Voxel>>());

		for (int j = 0; j < height; j++) {
			vec[i].push_back(vector<Voxel>());

			for (int k = 0; k < depth; k++) {
				char voxelId = std::stoi(list[counter].substr(1, list[counter].length()-1));
				counter++;
				vec[i][j].push_back(Voxel{voxelId, 0});
			}
		}
	}

	vector<vec3> lightPropagate;
	doSunLightning(lightPropagate);

	for (vec3 vec : lightPropagate)
		propagateLight(vec.x, vec.y, vec.z);


}

Chunk::~Chunk()
{
	ChunkBatcher::getInstance().removeBatch(graphicalChunk);
};

// ########################################################
// Member Functions########################################
// ########################################################

Voxel& Chunk::getVoxel(int x, int y, int z)
{
	return vec.at(x).at(y).at(z);
}

// TODO Duplicated from graphical chunk
// TODO Change the nullptr checks
Voxel* Chunk::getVoxel2(int x, int y, int z)
{
	if (x < width && x >= 0 && y < height && y >= 0 && z < depth && z >= 0) {
		return &vec[x][y][z];
	} else if (x == width && (y < height && y >= 0 && z < depth && z >= 0)) {
		if (rightNeighbor) {
			return &(rightNeighbor->vec[0][y][z]);
		} else {
			return nullptr;
		}
	} else if (x == -1 && (y < height && y >= 0 && z < depth && z >= 0)) {
		if (leftNeighbor) {
			return &(leftNeighbor->vec[width - 1][y][z]);
		} else {
			return nullptr;
		}
	} else if (z == depth && (x < width && x >= 0 && y < height && y >= 0)) {
		if (backNeighbor) {
			return &(backNeighbor->vec[x][y][0]);
		} else {
			return nullptr;
		}
	} else if (z == -1 && (x < width && x >= 0 && y < height && y >= 0)) {
		if (frontNeighbor) {
			return &(frontNeighbor->vec[x][y][depth - 1]);
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

	// We can not change bedrock!
	if (vec[x][y][z].id == BED_ROCK)
		return;

	Voxel &voxel = vec[x][y][z];
	voxel.id = id;

	// If we removed a cube
	if (id == AIR) {
		updateLightningCubeRemoved(voxel, x, y, z);
	} else { // We added a cube
		updateLightningCubeAdded(x, y, z);
		voxel.lightValue = 0;
	}

}

void Chunk::updateLightningCubeRemoved(Voxel& voxel, int x, int y, int z)
{

	if (isInDirectSunlight(x, y, z)) {

		vector<vec3> lightPropagate;
		doSunLightning(lightPropagate, x, y, z);
		for (auto v : lightPropagate)
			propagateLight(v.x, v.y, v.z);


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
	updateLightning();
	updateGraphics();
	// TODO Do more accurate check of which neighbors (if any) that needs to be updated
	updateNeighborGraphics();
}

void Chunk::updateGraphics()
{

	vector<vector<vector<Voxel>>> *right = nullptr;
	vector<vector<vector<Voxel>>> *left = nullptr;
	vector<vector<vector<Voxel>>> *front = nullptr;
	vector<vector<vector<Voxel>>> *back = nullptr;

	if (rightNeighbor.get())
		right = &(rightNeighbor->vec);

	if (leftNeighbor.get())
		left = &(leftNeighbor->vec);

	if (frontNeighbor.get())
		front = &(frontNeighbor->vec);

	if (backNeighbor.get())
		back = &(backNeighbor->vec);

	ChunkBatcher::getInstance().removeBatch(graphicalChunk);
	graphicalChunk.reset(new GraphicalChunk(xLocation, 0, zLocation, vec, right, left, back, front));
	ChunkBatcher::getInstance().addBatch(graphicalChunk);

}

void Chunk::updateNeighborGraphics()
{
	if (rightNeighbor) {
		rightNeighbor->updateGraphics();

		if (rightNeighbor->backNeighbor)
			rightNeighbor->backNeighbor->updateGraphics();

		if (rightNeighbor->frontNeighbor)
			rightNeighbor->frontNeighbor->updateGraphics();

	}

	if (leftNeighbor) {
		leftNeighbor->updateGraphics();

		if (leftNeighbor->backNeighbor)
			leftNeighbor->backNeighbor->updateGraphics();

		if (leftNeighbor->frontNeighbor)
			leftNeighbor->frontNeighbor->updateGraphics();

	}

	if (backNeighbor)
		backNeighbor->updateGraphics();


	if (frontNeighbor)
		frontNeighbor->updateGraphics();
}

void Chunk::setLeftNeighbor(shared_ptr<Chunk> chunk)
{
	leftNeighbor = chunk;
}

void Chunk::setRightNeighbor(shared_ptr<Chunk> chunk)
{
	rightNeighbor = chunk;
}

void Chunk::setFrontNeighbor(shared_ptr<Chunk> chunk)
{
	frontNeighbor = chunk;
}

void Chunk::setBackNeighbor(shared_ptr<Chunk> chunk)
{
	backNeighbor = chunk;
}

void Chunk::doSunLightning(vector<vec3> &lightPropagate)
{

	// TODO Not correct k/j problem
	// Sun lightning, only air gets light
	for (int i = 0; i < CHUNK_WIDTH; i++) {
		for (int j = 0; j < CHUNK_DEPTH; j++) {
			bool foundSolid = false;
			for (int k = CHUNK_HEIGHT-1; k >= 0; k--) {
				if (vec[i][k][j].id == AIR) {

					if (foundSolid) {
						vec[i][k][j].lightValue = 0;
					} else {
						vec[i][k][j].lightValue = directSunlight;
						lightPropagate.push_back(vec3(i,k,j));
					}
				} else {
					foundSolid = true;
				}
			}
		}
	}

}

void Chunk::doSunLightning(vector<vec3> &lightPropagate, int x, int y, int z)
{
	for (int i = y; y >= 0; i--) {
		if (vec[x][i][z].id == AIR) {
			vec[x][i][z].lightValue = directSunlight;
			lightPropagate.push_back(vec3(x, i, z));
		} else {
			break;
		}
	}
}

void Chunk::updateLightning()
{

	vector<vec3> lightPropagate;

	vector<vec3> lightPropagateRight;
	vector<vec3> lightPropagateRightBack;
	vector<vec3> lightPropagateRightFront;

	vector<vec3> lightPropagateLeft;
	vector<vec3> lightPropagateLeftBack;
	vector<vec3> lightPropagateLeftFront;

	vector<vec3> lightPropagateBack;
	vector<vec3> lightPropagateFront;

	doSunLightning(lightPropagate);

	// Sun light ####################################################

	if (rightNeighbor) {
		rightNeighbor->doSunLightning(lightPropagateRight);

		if (rightNeighbor->backNeighbor)
			rightNeighbor->backNeighbor->doSunLightning(lightPropagateRightBack);

		if (rightNeighbor->frontNeighbor)
			rightNeighbor->frontNeighbor->doSunLightning(lightPropagateRightFront);

	}

	if (leftNeighbor) {
		leftNeighbor->doSunLightning(lightPropagateLeft);

		if (leftNeighbor->backNeighbor)
			leftNeighbor->backNeighbor->doSunLightning(lightPropagateLeftBack);

		if (leftNeighbor->frontNeighbor)
			leftNeighbor->frontNeighbor->doSunLightning(lightPropagateLeftFront);

	}

	if (backNeighbor)
		backNeighbor->doSunLightning(lightPropagateBack);

	if (frontNeighbor)
		frontNeighbor->doSunLightning(lightPropagateFront);



	// Propagate light ##############################################

	for (vec3 vec : lightPropagate)
		propagateLight(vec.x, vec.y, vec.z);

	if (rightNeighbor) {
		rightNeighbor->collectLightFromRightNeighbor(lightPropagateRight);
		for (vec3 vec : lightPropagateRight)
			rightNeighbor->propagateLight(vec.x, vec.y, vec.z);

		if (rightNeighbor->backNeighbor.get()) {
			rightNeighbor->backNeighbor->collectLightFromRightNeighbor(lightPropagateRightBack);
			rightNeighbor->backNeighbor->collectLightFromBackNeighbor(lightPropagateRightBack);

			for (vec3 vec : lightPropagateRightBack)
				rightNeighbor->backNeighbor->propagateLight(vec.x, vec.y, vec.z);

		}

		if (rightNeighbor->frontNeighbor) {
			rightNeighbor->frontNeighbor->collectLightFromRightNeighbor(lightPropagateRightFront);
			rightNeighbor->frontNeighbor->collectLightFromFrontNeighbor(lightPropagateRightFront);

			for (vec3 vec : lightPropagateRightFront)
				rightNeighbor->frontNeighbor->propagateLight(vec.x, vec.y, vec.z);

		}
	}

	if (leftNeighbor) {
		leftNeighbor->collectLightFromLeftNeighbor(lightPropagateLeft);
		for (vec3 vec : lightPropagateLeft)
			leftNeighbor->propagateLight(vec.x, vec.y, vec.z);

		if (leftNeighbor->backNeighbor.get()) {
			leftNeighbor->backNeighbor->collectLightFromLeftNeighbor(lightPropagateLeftBack);
			leftNeighbor->backNeighbor->collectLightFromBackNeighbor(lightPropagateLeftBack);

			for (vec3 vec : lightPropagateLeftBack)
				leftNeighbor->backNeighbor->propagateLight(vec.x, vec.y, vec.z);

		}

		if (leftNeighbor->frontNeighbor) {
			leftNeighbor->frontNeighbor->collectLightFromLeftNeighbor(lightPropagateLeftFront);
			leftNeighbor->frontNeighbor->collectLightFromFrontNeighbor(lightPropagateLeftFront);

			for (vec3 vec : lightPropagateLeftFront)
				leftNeighbor->frontNeighbor->propagateLight(vec.x, vec.y, vec.z);

		}
	}

	if (backNeighbor) {
		backNeighbor->collectLightFromBackNeighbor(lightPropagateBack);
		for (vec3 vec : lightPropagateBack)
			backNeighbor->propagateLight(vec.x, vec.y, vec.z);

	}

	if (frontNeighbor) {
		frontNeighbor->collectLightFromFrontNeighbor(lightPropagateFront);
		for (vec3 vec : lightPropagateFront)
			frontNeighbor->propagateLight(vec.x, vec.y, vec.z);

	}

}

void Chunk::updateLightning2()
{
	vector<vec3> lightPropagate;
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

void Chunk::collectLightFromRightNeighbor(vector<vec3> &lightPropagate)
{
	if (rightNeighbor.get()) {
		for (int j = 0; j < CHUNK_HEIGHT; j++) {
			for (int k = 0; k < CHUNK_DEPTH; k++) {
				char lv = rightNeighbor->vec[0][j][k].lightValue - 1;

				if (rightNeighbor->vec[0][j][k].id == AIR &&
						vec[15][j][k].id == AIR &&
						lv > vec[15][j][k].lightValue) {
					vec[15][j][k].lightValue = lv;
					lightPropagate.push_back(vec3(15, j, k));
				}
			}
		}
	}
}

void Chunk::collectLightFromLeftNeighbor(vector<vec3> &lightPropagate)
{
	if (leftNeighbor.get()) {
		for (int j = 0; j < CHUNK_HEIGHT; j++) {
			for (int k = 0; k < CHUNK_DEPTH; k++) {
				char lv = leftNeighbor->vec[15][j][k].lightValue - 1;

				if (leftNeighbor->vec[15][j][k].id == AIR &&
						vec[0][j][k].id == AIR &&
						lv > vec[0][j][k].lightValue) {
					vec[0][j][k].lightValue = lv;
					lightPropagate.push_back(vec3(0, j, k));
				}
			}
		}
	}
}

void Chunk::collectLightFromBackNeighbor(vector<vec3> &lightPropagate)
{

	if (backNeighbor.get()) {

		for (int i = 0; i < CHUNK_WIDTH; i++) {
			for (int j = 0; j < CHUNK_HEIGHT; j++) {
				char lv = backNeighbor->vec[i][j][0].lightValue - 1;

				if (backNeighbor->vec[i][j][0].id == AIR &&
						vec[i][j][15].id == AIR
						&& lv > vec[i][j][15].lightValue) {

					vec[i][j][15].lightValue = lv;
					lightPropagate.push_back(vec3(i, j, 15));
				}
			}
		}
	}
}

void Chunk::collectLightFromFrontNeighbor(vector<vec3> &lightPropagate)
{
	if (frontNeighbor.get()) {
		for (int i = 0; i < CHUNK_WIDTH; i++) {
			for (int j = 0; j < CHUNK_HEIGHT; j++) {
				char lv = frontNeighbor->vec[i][j][15].lightValue - 1;

				if (frontNeighbor->vec[i][j][15].id == AIR &&
						vec[i][j][0].id == AIR &&
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

	vector<vec3> newPropagates;

	// ##################################################################################

	// Traverse right
	int lv = lvInitial;
	for (int i = x + 1; lv > 0; i++) {

		if (i < width) {
			Voxel &v = vec[i][y][z];
			if (v.id == AIR && v.lightValue < lv) {
				v.lightValue = lv;
				newPropagates.push_back(vec3(i, y, z));
				lv--;
			} else {
				break;
			}
		} else {
			if (rightNeighbor.get()) {
				if (rightNeighbor->vec[0][y][z].id == AIR &&
						rightNeighbor->vec[0][y][z].lightValue < lv) {
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
			if (v.id == AIR && v.lightValue < lv) {
				v.lightValue = lv;
				newPropagates.push_back(vec3(i, y, z));
				lv--;
			} else {
				break;
			}
		} else {
			if (leftNeighbor.get()) {
				if (leftNeighbor->vec[width - 1][y][z].id == AIR
						&& leftNeighbor->vec[width - 1][y][z].lightValue < lv) {
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
		if (v.id == AIR && v.lightValue < lv) {
			v.lightValue = lv;
			newPropagates.push_back(vec3(x, i, z));
			lv--;
		} else {
			break;
		}
	}

	// Treaverse down
	lv = lvInitial;
	for (int i = y - 1; i >= 0; i--) {
		Voxel &v = vec[x][i][z];
		if (v.id == AIR && v.lightValue < lv) {
			v.lightValue = lv;
			newPropagates.push_back(vec3(x, i, z));
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
			if (v.id == AIR && v.lightValue < lv) {
				v.lightValue = lv;
				newPropagates.push_back(vec3(x, y, i));
				lv--;
			} else {
				break;
			}
		} else {
			if (backNeighbor.get()) {
				if (backNeighbor->vec[x][y][0].id == AIR &&
						backNeighbor->vec[x][y][0].lightValue < lv) {
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
			if (v.id == AIR && v.lightValue < lv) {
				v.lightValue = lv;
				newPropagates.push_back(vec3(x, y, i));
				lv--;
			} else {
				break;
			}
		} else {
			if (frontNeighbor.get()) {
				if (frontNeighbor->vec[x][y][depth - 1].id == AIR &&
						frontNeighbor->vec[x][y][depth - 1].lightValue < lv) {
					frontNeighbor->vec[x][y][depth - 1].lightValue = lv;
					frontNeighbor->propagateLight(x, y, depth - 1);
				}
			}
			break;
		}
	}

	for (vec3 vec : newPropagates)
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
	for (int i = height - 1; i > y; i--)
		if (vec[x][i][z].id != AIR)
			return false;

	return true;
}


// ############################################################################

//Chunk loadChunk(std::string worldName, int x, int z);

// Helper function
bool fileExists(string& fileName) {
    ifstream infile(fileName);
    return infile.good();
}

void Chunk::storeChunk(string worldName, int x, int z)
{
	string fileName = config::dataFolder + worldName + "_" + to_string(x) + "_" + to_string(z) + ".chunk";

	cout << "The file name is: " << fileName << " \n";

	ofstream outStream(fileName);
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			for (int z = 0; z < depth; z++) {
				outStream << "(" << to_string(vec[x][y][z].id) << ") \n";
			}
		}
	}

	outStream.close();

}

}
