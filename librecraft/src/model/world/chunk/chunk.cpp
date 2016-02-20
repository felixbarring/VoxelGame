#include "chunk.h"

#include <iostream>
#include <fstream>

#include "../../../graphics/chunkBatcher.h"

using namespace std;
using namespace glm;

using namespace graphics;
using namespace config::cube_data;
using namespace config::chunk_data;

namespace chunk {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

static int counter = 1;
static const int maxCount = LAST_CUBE;

Chunk::Chunk(int _x, int _z)
		: m_xLocation {_x}, m_zLocation {_z}, m_isDirty {true} {

	counter++;

	for (int x = 0; x < m_width; ++x) {
		m_vec.push_back(vector<vector<Voxel>>());

		if (counter > maxCount)
			counter = 0;

		for (int z = 0; z < m_height; ++z) {
			m_vec[x].push_back(vector<Voxel>());

			for (int y = 0; y < m_depth; ++y) {
				Voxel v;
				v.lightValue = 0;

				if (z == 0) {
					v.id = BED_ROCK;
					m_vec[x][z].push_back(v);
					continue;
				}

				if (z < 5) {
					v.id = counter;
					m_vec[x][z].push_back(v);
				} else {
					v.id = AIR;
					m_vec[x][z].push_back(v);
				}
			}
		}
	}

	vector<vec3> lightPropagate;
	doSunLightning(lightPropagate);

	for (vec3 vec : lightPropagate)
		propagateLight(vec.x, vec.y, vec.z);

}

Chunk::Chunk(std::string name, int x, int z)
		: m_xLocation {x}, m_zLocation {z}, m_isDirty {false} {
	vector<string> list;
	ifstream inStream;
	string line;

	string file = config::dataFolder + name + "_"
			+ std::to_string(x) + "_"
			+ std::to_string(z) + ".chunk";
	inStream.open(file);

	// Add all lines to the vector
	while (getline(inStream, line))
		list.push_back(line);

	inStream.close();

	int counter = 0;
	for (int i = 0; i < m_width; i++) {
		m_vec.push_back(vector<vector<Voxel>>());

		for (int j = 0; j < m_height; j++) {
			m_vec[i].push_back(vector<Voxel>());

			for (int k = 0; k < m_depth; k++) {
				char voxelId = std::stoi(list[counter]);
				counter++;
				m_vec[i][j].push_back(Voxel {voxelId, 0});
			}
		}
	}

	vector<vec3> lightPropagate;
	doSunLightning(lightPropagate);

	for (vec3 vec : lightPropagate)
		propagateLight(vec.x, vec.y, vec.z);

}

Chunk::~Chunk() {
	cout << "Removing chunk " << m_xLocation << " " << m_zLocation << "\n";
	ChunkBatcher::getInstance().removeBatch(m_graphicalChunk);
}

// ########################################################
// Member Functions########################################
// ########################################################

Voxel Chunk::getVoxel(int x, int y, int z) {
	return m_vec[x][y][z];
}

// TODO Duplicated from graphical chunk
// TODO Change the nullptr checks
Voxel* Chunk::getVoxel2(int x, int y, int z) {
	if (x < m_width && x >= 0 && y < m_height && y >= 0 && z < m_depth && z >= 0) {
		return &m_vec[x][y][z];
	} else if (x == m_width && (y < m_height && y >= 0 && z < m_depth && z >= 0)) {
		if (m_rightNeighbor) {
			return &(m_rightNeighbor->m_vec[0][y][z]);
		} else {
			return nullptr;
		}
	} else if (x == -1 && (y < m_height && y >= 0 && z < m_depth && z >= 0)) {
		if (m_leftNeighbor) {
			return &(m_leftNeighbor->m_vec[m_width - 1][y][z]);
		} else {
			return nullptr;
		}
	} else if (z == m_depth && (x < m_width && x >= 0 && y < m_height && y >= 0)) {
		if (backNeighbor) {
			return &(backNeighbor->m_vec[x][y][0]);
		} else {
			return nullptr;
		}
	} else if (z == -1 && (x < m_width && x >= 0 && y < m_height && y >= 0)) {
		if (frontNeighbor) {
			return &(frontNeighbor->m_vec[x][y][m_depth - 1]);
		} else {
			return nullptr;
		}
	}

	return nullptr;
}

char Chunk::getCubeId(int x, int y, int z) {
	return m_vec.at(x).at(y).at(z).id;
}

void Chunk::setCube(int x, int y, int z, char id) {
	// We can not change bedrock!
	if (m_vec[x][y][z].id == BED_ROCK)
		return;

	Voxel &voxel = m_vec[x][y][z];
	voxel.id = id;

	// If we removed a cube
	if (id == AIR) {
		updateLightningCubeRemoved(voxel, x, y, z);
	} else { // We added a cube
		updateLightningCubeAdded(x, y, z);
		voxel.lightValue = 0;
	}
	m_isDirty = true;
}

void Chunk::updateLightningCubeRemoved(Voxel& voxel, int x, int y, int z) {

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

void Chunk::updateLightningCubeAdded(int x, int y, int z) {
	updateLightning();
	updateGraphics();
	// TODO Do more accurate check of which neighbors (if any) that needs to be updated
	updateNeighborGraphics();
}

void Chunk::updateGraphics() {
	vector<vector<vector<Voxel>>>*right = nullptr;
	vector<vector<vector<Voxel>>> *left = nullptr;
	vector<vector<vector<Voxel>>> *front = nullptr;
	vector<vector<vector<Voxel>>> *back = nullptr;

	if (m_rightNeighbor.get())
	right = &(m_rightNeighbor->m_vec);

	if (m_leftNeighbor.get())
	left = &(m_leftNeighbor->m_vec);

	if (frontNeighbor.get())
	front = &(frontNeighbor->m_vec);

	if (backNeighbor.get())
	back = &(backNeighbor->m_vec);

	ChunkBatcher::getInstance().removeBatch(m_graphicalChunk);
	m_graphicalChunk.reset(new GraphicalChunk(m_xLocation, 0, m_zLocation, m_vec,
					right, left, back, front));
	ChunkBatcher::getInstance().addBatch(m_graphicalChunk);
}

void Chunk::updateNeighborGraphics() {
	if (m_rightNeighbor) {
		m_rightNeighbor->updateGraphics();

		if (m_rightNeighbor->backNeighbor)
			m_rightNeighbor->backNeighbor->updateGraphics();

		if (m_rightNeighbor->frontNeighbor)
			m_rightNeighbor->frontNeighbor->updateGraphics();

	}

	if (m_leftNeighbor) {
		m_leftNeighbor->updateGraphics();

		if (m_leftNeighbor->backNeighbor)
			m_leftNeighbor->backNeighbor->updateGraphics();

		if (m_leftNeighbor->frontNeighbor)
			m_leftNeighbor->frontNeighbor->updateGraphics();

	}

	if (backNeighbor)
		backNeighbor->updateGraphics();

	if (frontNeighbor)
		frontNeighbor->updateGraphics();
}

void Chunk::setLeftNeighbor(shared_ptr<Chunk> chunk) {
	m_leftNeighbor = chunk;
}

void Chunk::setRightNeighbor(shared_ptr<Chunk> chunk) {
	m_rightNeighbor = chunk;
}

void Chunk::setFrontNeighbor(shared_ptr<Chunk> chunk) {
	frontNeighbor = chunk;
}

void Chunk::setBackNeighbor(shared_ptr<Chunk> chunk) {
	backNeighbor = chunk;
}

void Chunk::removeAllNeighbores() {
	m_leftNeighbor.reset();
	m_rightNeighbor.reset();
	frontNeighbor.reset();
	backNeighbor.reset();
}

void Chunk::doSunLightning(vector<vec3> &lightPropagate) {

	// TODO Not correct k/j problem
	// Sun lightning, only air gets light
	for (int i = 0; i < CHUNK_WIDTH; i++) {
		for (int j = 0; j < CHUNK_DEPTH; j++) {
			bool foundSolid = false;
			for (int k = CHUNK_HEIGHT - 1; k >= 0; k--) {
				if (m_vec[i][k][j].id == AIR) {

					if (foundSolid) {
						m_vec[i][k][j].lightValue = 0;
					} else {
						m_vec[i][k][j].lightValue = m_directSunlight;
						lightPropagate.push_back(vec3(i, k, j));
					}
				} else {
					foundSolid = true;
				}
			}
		}
	}

}

void Chunk::doSunLightning(vector<vec3> &lightPropagate, int x, int y, int z) {
	for (int i = y; y >= 0; i--) {
		if (m_vec[x][i][z].id == AIR) {
			m_vec[x][i][z].lightValue = m_directSunlight;
			lightPropagate.push_back(vec3(x, i, z));
		} else {
			break;
		}
	}
}

void Chunk::updateLightning() {

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

	if (m_rightNeighbor) {
		m_rightNeighbor->doSunLightning(lightPropagateRight);

		if (m_rightNeighbor->backNeighbor)
			m_rightNeighbor->backNeighbor->doSunLightning(
					lightPropagateRightBack);

		if (m_rightNeighbor->frontNeighbor)
			m_rightNeighbor->frontNeighbor->doSunLightning(
					lightPropagateRightFront);

	}

	if (m_leftNeighbor) {
		m_leftNeighbor->doSunLightning(lightPropagateLeft);

		if (m_leftNeighbor->backNeighbor)
			m_leftNeighbor->backNeighbor->doSunLightning(lightPropagateLeftBack);

		if (m_leftNeighbor->frontNeighbor)
			m_leftNeighbor->frontNeighbor->doSunLightning(
					lightPropagateLeftFront);

	}

	if (backNeighbor)
		backNeighbor->doSunLightning(lightPropagateBack);

	if (frontNeighbor)
		frontNeighbor->doSunLightning(lightPropagateFront);

	// Propagate light ##############################################

	for (vec3 vec : lightPropagate)
		propagateLight(vec.x, vec.y, vec.z);

	if (m_rightNeighbor) {
		m_rightNeighbor->collectLightFromRightNeighbor(lightPropagateRight);
		for (vec3 vec : lightPropagateRight)
			m_rightNeighbor->propagateLight(vec.x, vec.y, vec.z);

		if (m_rightNeighbor->backNeighbor.get()) {
			m_rightNeighbor->backNeighbor->collectLightFromRightNeighbor(
					lightPropagateRightBack);
			m_rightNeighbor->backNeighbor->collectLightFromBackNeighbor(
					lightPropagateRightBack);

			for (vec3 vec : lightPropagateRightBack)
				m_rightNeighbor->backNeighbor->propagateLight(vec.x, vec.y,
						vec.z);

		}

		if (m_rightNeighbor->frontNeighbor) {
			m_rightNeighbor->frontNeighbor->collectLightFromRightNeighbor(
					lightPropagateRightFront);
			m_rightNeighbor->frontNeighbor->collectLightFromFrontNeighbor(
					lightPropagateRightFront);

			for (vec3 vec : lightPropagateRightFront)
				m_rightNeighbor->frontNeighbor->propagateLight(vec.x, vec.y,
						vec.z);

		}
	}

	if (m_leftNeighbor) {
		m_leftNeighbor->collectLightFromLeftNeighbor(lightPropagateLeft);
		for (vec3 vec : lightPropagateLeft)
			m_leftNeighbor->propagateLight(vec.x, vec.y, vec.z);

		if (m_leftNeighbor->backNeighbor.get()) {
			m_leftNeighbor->backNeighbor->collectLightFromLeftNeighbor(
					lightPropagateLeftBack);
			m_leftNeighbor->backNeighbor->collectLightFromBackNeighbor(
					lightPropagateLeftBack);

			for (vec3 vec : lightPropagateLeftBack)
				m_leftNeighbor->backNeighbor->propagateLight(vec.x, vec.y, vec.z);

		}

		if (m_leftNeighbor->frontNeighbor) {
			m_leftNeighbor->frontNeighbor->collectLightFromLeftNeighbor(
					lightPropagateLeftFront);
			m_leftNeighbor->frontNeighbor->collectLightFromFrontNeighbor(
					lightPropagateLeftFront);

			for (vec3 vec : lightPropagateLeftFront)
				m_leftNeighbor->frontNeighbor->propagateLight(vec.x, vec.y,
						vec.z);

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

void Chunk::updateLightning2() {
	vector<vec3> lightPropagate;
	doSunLightning(lightPropagate);

	if (m_rightNeighbor.get())
		collectLightFromRightNeighbor(lightPropagate);

	if (m_leftNeighbor.get())
		collectLightFromLeftNeighbor(lightPropagate);

	if (backNeighbor.get())
		collectLightFromBackNeighbor(lightPropagate);

	if (frontNeighbor.get())
		collectLightFromFrontNeighbor(lightPropagate);

	for (glm::vec3 vec : lightPropagate)
		propagateLight(vec.x, vec.y, vec.z);

}

void Chunk::collectLightFromRightNeighbor(vector<vec3> &lightPropagate) {
	if (m_rightNeighbor.get()) {
		for (int j = 0; j < CHUNK_HEIGHT; j++) {
			for (int k = 0; k < CHUNK_DEPTH; k++) {
				char lv = m_rightNeighbor->m_vec[0][j][k].lightValue - 1;

				if (m_rightNeighbor->m_vec[0][j][k].id == AIR
						&& m_vec[15][j][k].id == AIR
						&& lv > m_vec[15][j][k].lightValue) {
					m_vec[15][j][k].lightValue = lv;
					lightPropagate.push_back(vec3(15, j, k));
				}
			}
		}
	}
}

void Chunk::collectLightFromLeftNeighbor(vector<vec3> &lightPropagate) {
	if (m_leftNeighbor.get()) {
		for (int j = 0; j < CHUNK_HEIGHT; j++) {
			for (int k = 0; k < CHUNK_DEPTH; k++) {
				char lv = m_leftNeighbor->m_vec[15][j][k].lightValue - 1;

				if (m_leftNeighbor->m_vec[15][j][k].id == AIR
						&& m_vec[0][j][k].id == AIR
						&& lv > m_vec[0][j][k].lightValue) {
					m_vec[0][j][k].lightValue = lv;
					lightPropagate.push_back(vec3(0, j, k));
				}
			}
		}
	}
}

void Chunk::collectLightFromBackNeighbor(vector<vec3> &lightPropagate) {

	if (backNeighbor.get()) {

		for (int i = 0; i < CHUNK_WIDTH; i++) {
			for (int j = 0; j < CHUNK_HEIGHT; j++) {
				char lv = backNeighbor->m_vec[i][j][0].lightValue - 1;

				if (backNeighbor->m_vec[i][j][0].id == AIR
						&& m_vec[i][j][15].id == AIR
						&& lv > m_vec[i][j][15].lightValue) {

					m_vec[i][j][15].lightValue = lv;
					lightPropagate.push_back(vec3(i, j, 15));
				}
			}
		}
	}
}

void Chunk::collectLightFromFrontNeighbor(vector<vec3> &lightPropagate) {
	if (frontNeighbor.get()) {
		for (int i = 0; i < CHUNK_WIDTH; i++) {
			for (int j = 0; j < CHUNK_HEIGHT; j++) {
				char lv = frontNeighbor->m_vec[i][j][15].lightValue - 1;

				if (frontNeighbor->m_vec[i][j][15].id == AIR
						&& m_vec[i][j][0].id == AIR
						&& lv > m_vec[i][j][0].lightValue) {

					m_vec[i][j][0].lightValue = lv;
					propagateLight(i, j, 0);

				}
			}
		}
	}
}

void Chunk::propagateLight(int x, int y, int z) {

	Voxel &voxel = m_vec[x][y][z];
	int lvInitial = voxel.lightValue - 1;
	vector<vec3> newPropagates;

	// ########################################################################

	// Traverse right
	int lv = lvInitial;
	for (int i = x + 1; lv > 0; i++) {

		if (i < m_width) {
			Voxel &v = m_vec[i][y][z];
			if (v.id == AIR && v.lightValue < lv) {
				v.lightValue = lv;
				newPropagates.push_back(vec3(i, y, z));
				lv--;
			} else {
				break;
			}
		} else {
			if (m_rightNeighbor.get()) {
				if (m_rightNeighbor->m_vec[0][y][z].id == AIR
						&& m_rightNeighbor->m_vec[0][y][z].lightValue < lv) {
					m_rightNeighbor->m_vec[0][y][z].lightValue = lv;
					m_rightNeighbor->propagateLight(0, y, z);
				}
			}
			break;
		}
	}

	// Travers left
	lv = lvInitial;
	for (int i = x - 1; lv > 0; i--) {

		if (i >= 0) {
			Voxel &v = m_vec[i][y][z];
			if (v.id == AIR && v.lightValue < lv) {
				v.lightValue = lv;
				newPropagates.push_back(vec3(i, y, z));
				lv--;
			} else {
				break;
			}
		} else {
			if (m_leftNeighbor.get()) {
				if (m_leftNeighbor->m_vec[m_width - 1][y][z].id == AIR
						&& m_leftNeighbor->m_vec[m_width - 1][y][z].lightValue < lv) {
					m_leftNeighbor->m_vec[m_width - 1][y][z].lightValue = lv;
					m_leftNeighbor->propagateLight(m_width - 1, y, z);
				}
			}
			break;
		}
	}

	// ########################################################################

	// Traverse up
	lv = lvInitial;
	for (int i = y + 1; i < m_height; i++) {
		Voxel &v = m_vec[x][i][z];
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
		Voxel &v = m_vec[x][i][z];
		if (v.id == AIR && v.lightValue < lv) {
			v.lightValue = lv;
			newPropagates.push_back(vec3(x, i, z));
			lv--;
		} else {
			break;
		}
	}

	// ########################################################################

	// Traverse backwards
	lv = lvInitial;
	for (int i = z + 1; lv > 0; i++) {

		if (i < m_depth) {
			Voxel &v = m_vec[x][y][i];
			if (v.id == AIR && v.lightValue < lv) {
				v.lightValue = lv;
				newPropagates.push_back(vec3(x, y, i));
				lv--;
			} else {
				break;
			}
		} else {
			if (backNeighbor.get()) {
				if (backNeighbor->m_vec[x][y][0].id == AIR
						&& backNeighbor->m_vec[x][y][0].lightValue < lv) {
					backNeighbor->m_vec[x][y][0].lightValue = lv;
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

			Voxel &v = m_vec[x][y][i];
			if (v.id == AIR && v.lightValue < lv) {
				v.lightValue = lv;
				newPropagates.push_back(vec3(x, y, i));
				lv--;
			} else {
				break;
			}
		} else {
			if (frontNeighbor.get()) {
				if (frontNeighbor->m_vec[x][y][m_depth - 1].id == AIR
						&& frontNeighbor->m_vec[x][y][m_depth - 1].lightValue
								< lv) {
					frontNeighbor->m_vec[x][y][m_depth - 1].lightValue = lv;
					frontNeighbor->propagateLight(x, y, m_depth - 1);
				}
			}
			break;
		}
	}

	for (vec3 vec : newPropagates)
		propagateLight(vec.x, vec.y, vec.z);

}

void Chunk::dePropagateLight(int x, int y, int z)
{
	Voxel &voxel = m_vec[x][y][z];
	int lvInitial = voxel.lightValue - 1;
	vector<vec3> newPropagates;

}

int Chunk::highestLightValueFromNeighbors(int x, int y, int z) {
	int highestValue = -1;

	Voxel* v = getVoxel2(x + 1, y, z);
	if (v && v->lightValue > highestValue)
		highestValue = v->lightValue;

	v = getVoxel2(x - 1, y, z);
	if (v && v->lightValue > highestValue)
		highestValue = v->lightValue;

	v = getVoxel2(x, y + 1, z);
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

bool Chunk::isInDirectSunlight(int x, int y, int z) {
	for (int i = m_height - 1; i > y; i--)
		if (m_vec[x][i][z].id != AIR)
			return false;

	return true;
}

// ############################################################################

// Helper function
//bool fileExists(string& fileName) {
//    ifstream infile(fileName);
//    return infile.good();
//}

void Chunk::storeChunk(string worldName, int x, int z) {

	if (!m_isDirty)
		return;

	string fileName = config::dataFolder + worldName + "_" + to_string(x) + "_"
			+ to_string(z) + ".chunk";

	ofstream outStream(fileName);
	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_height; y++) {
			for (int z = 0; z < m_depth; z++) {
				outStream << to_string(m_vec[x][y][z].id) << "\n";
			}
		}
	}
	outStream.close();
}

}
