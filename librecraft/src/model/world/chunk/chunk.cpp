#include "chunk.h"

#include <iostream>
#include <fstream>

#include "../../../graphics/chunkBatcher.h"
#include "../terrainGen/noiseMixer.h"

using namespace std;
using namespace glm;

using namespace graphics;
using namespace config::cube_data;
using namespace config::chunk_data;
using namespace terrainGen;

namespace chunk {

static int counter = 1;
static const int maxCount = LAST_CUBE_USED_FOR_BUILDING_;
static std::mutex s_mutex;

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Chunk::Chunk(string worldName, int x, int z)
    : m_xLocation{x},
      m_zLocation{z},
      m_isDirty{true},
      m_name{createChunkName(worldName)}
{

    for (int i = 0; i < CHUNK_HEIGHT / GRAPHICAL_CHUNK_HEIGHT; ++i) {
        m_graphicalChunksIds.push_back(-1);
        m_dirtyRegions.emplace(i);
    }
}

Chunk::~Chunk() {
    for (auto graphicalChunk : m_graphicalChunksIds)
        ChunkBatcher::getInstance().removeBatch(graphicalChunk);
}

// ########################################################
// Member Functions########################################
// ########################################################

void Chunk::create() {
    ifstream stream;
    stream.open(m_name);
    if (stream.fail())
        generateChunk();
    else
        loadChunk();
}

void Chunk::doSunLightning() {

    // Sun lightning, only air and water gets light
    // Each step in water reduces the light strength
    for (int x = 0; x < CHUNK_WIDTH_AND_DEPTH; ++x) {
        for (int z = 0; z < CHUNK_WIDTH_AND_DEPTH; ++z) {
            bool foundSolid = false;
            int lightValue{m_directSunlight};
            for (int y = CHUNK_HEIGHT - 1; y >= 0; --y) {
                auto &cube = m_cubes[x][y][z];
                if (cube.id == AIR || cube.id == WATER) {
                    if (cube.id == WATER) {
                        --lightValue;
                        if (lightValue < 0)
                            lightValue = 0;
                    }
                    cube.lightValue = lightValue;
                    if (cube.lightValue > 0)
                        m_lightsToPropagate.push_back(vec3(x, y, z));
                } else {
                    cube.lightValue = 0;
                }
            }
        }
    }
    }

void Chunk::collectLightFromAllNeighbors() {
    collectLightFromRightNeighbor();
    collectLightFromLeftNeighbor();
    collectLightFromBackNeighbor();
    collectLightFromFrontNeighbor();
}

void Chunk::collectLightFromRightNeighbor() {
    if (m_rightNeighbor.get()) {
        for (int j = 0; j < CHUNK_HEIGHT; j++) {
            for (int k = 0; k < CHUNK_WIDTH_AND_DEPTH; k++) {
                char lv = m_rightNeighbor->m_cubes[0][j][k].lightValue - 1;

                if (m_rightNeighbor->m_cubes[0][j][k].id == AIR
                        && m_cubes[15][j][k].id == AIR
                        && lv > m_cubes[15][j][k].lightValue) {
                    m_cubes[15][j][k].lightValue = lv;
                    m_lightsToPropagate.push_back(vec3(15, j, k));
                }
            }
        }
    }
}

void Chunk::collectLightFromLeftNeighbor() {
    if (m_leftNeighbor.get()) {
        for (int j = 0; j < CHUNK_HEIGHT; j++) {
            for (int k = 0; k < CHUNK_WIDTH_AND_DEPTH; k++) {
                char lv = m_leftNeighbor->m_cubes[15][j][k].lightValue - 1;

                if (m_leftNeighbor->m_cubes[15][j][k].id == AIR
                        && m_cubes[0][j][k].id == AIR
                        && lv > m_cubes[0][j][k].lightValue) {
                    m_cubes[0][j][k].lightValue = lv;
                    m_lightsToPropagate.push_back(vec3(0, j, k));
                }
            }
        }
    }
}

void Chunk::collectLightFromBackNeighbor() {
    if (m_backNeighbor.get()) {
        for (int i = 0; i < CHUNK_WIDTH_AND_DEPTH; i++) {
            for (int j = 0; j < CHUNK_HEIGHT; j++) {
                char lv = m_backNeighbor->m_cubes[i][j][0].lightValue - 1;

                if (m_backNeighbor->m_cubes[i][j][0].id == AIR
                        && m_cubes[i][j][15].id == AIR
                        && lv > m_cubes[i][j][15].lightValue) {

                    m_cubes[i][j][15].lightValue = lv;
                    m_lightsToPropagate.push_back(vec3(i, j, 15));
                }
            }
        }
    }
}

void Chunk::collectLightFromFrontNeighbor() {
    if (m_frontNeighbor.get()) {
        for (int i = 0; i < CHUNK_WIDTH_AND_DEPTH; i++) {
            for (int j = 0; j < CHUNK_HEIGHT; j++) {
                char lv = m_frontNeighbor->m_cubes[i][j][15].lightValue - 1;

                if (m_frontNeighbor->m_cubes[i][j][15].id == AIR
                        && m_cubes[i][j][0].id == AIR
                        && lv > m_cubes[i][j][0].lightValue) {

                    m_cubes[i][j][0].lightValue = lv;
                    m_lightsToPropagate.push_back(vec3(i, j, 0));
                }
            }
        }
    }
}

void Chunk::propagateLights() {
    for (auto l : m_lightsToPropagate)
        propagateLight(l.x, l.y, l.z);
}

void Chunk::forceUpdateGraphics() {
    for (int i = 0; i < CHUNK_HEIGHT / GRAPHICAL_CHUNK_HEIGHT; ++i)
        m_dirtyRegions.emplace(i);

    updateGraphics();
}

void Chunk::updateGraphics(bool highPriority) {
    vector<vector<vector<Voxel>>> *right = nullptr;
    vector<vector<vector<Voxel>>> *left = nullptr;
    vector<vector<vector<Voxel>>> *front = nullptr;
    vector<vector<vector<Voxel>>> *back = nullptr;

    if (m_rightNeighbor.get())
        right = &(m_rightNeighbor->m_cubes);

    if (m_leftNeighbor.get())
        left = &(m_leftNeighbor->m_cubes);

    if (m_frontNeighbor.get())
        front = &(m_frontNeighbor->m_cubes);

    if (m_backNeighbor.get())
        back = &(m_backNeighbor->m_cubes);

    for (auto i : m_dirtyRegions) {
        auto derp = ChunkBatcher::getInstance().addBatch(
                m_graphicalChunksIds[i],
                m_xLocation, i * GRAPHICAL_CHUNK_HEIGHT, m_zLocation,
                m_cubes, right, left, back, front, highPriority);

        m_graphicalChunksIds[i] = derp;
    }
    m_dirtyRegions.clear();
}

Voxel Chunk::getVoxel(int x, int y, int z) {
    return m_cubes[x][y][z];
}

void Chunk::setCube(int x, int y, int z, char id) {
    // We can not change bedrock!
    if (m_cubes[x][y][z].id == BED_ROCK)
        return;

    Voxel &voxel = m_cubes[x][y][z];
    voxel.id = id;

    // If we removed a cube
    if (id == AIR || id == WATER) {
        updateLightningCubeRemoved(voxel, x, y, z);
    } else { // We added a cube
        updateLightningCubeAdded(x, y, z);
        voxel.lightValue = 0;
    }
    m_isDirty = true;
}

void Chunk::removeAllNeighbors() {
    if (m_leftNeighbor) {
        m_leftNeighbor->m_rightNeighbor.reset();
        m_leftNeighbor.reset();
    }
    if (m_rightNeighbor) {
        m_rightNeighbor->m_leftNeighbor.reset();
        m_rightNeighbor.reset();
    }
    if (m_frontNeighbor) {
        m_frontNeighbor->m_backNeighbor.reset();
        m_frontNeighbor.reset();
    }
    if (m_backNeighbor) {
        m_backNeighbor->m_frontNeighbor.reset();
        m_backNeighbor.reset();
    }
}

void Chunk::storeChunk(string worldName) {
    if (!m_isDirty)
        return;
    string fileName = config::dataFolder + worldName + "_" + to_string(m_xLocation) + "_"
            + to_string(m_zLocation) + ".chunk";

    ofstream outStream(fileName);
    for (int x = 0; x < m_width; x++) {
        for (int y = 0; y < m_height; y++) {
            for (int z = 0; z < m_depth; z++) {
                outStream << to_string(m_cubes[x][y][z].id) << "\n";
            }
        }
    }
    outStream.close();
}

// ############################################################################
// Private Methods ------------------------------------------------------------
// ############################################################################

std::string Chunk::createChunkName(std::string worldName) {
    return config::dataFolder + worldName + "_" +
            std::to_string(m_xLocation) + "_" +
            std::to_string(m_zLocation) + ".chunk";
}

void Chunk::loadChunk() {
    m_isDirty = false;

    vector<string> list;
    string line;
    ifstream inStream;
    inStream.open(m_name);

    // Add all lines to the vector
    while (getline(inStream, line))
        list.push_back(line);

    inStream.close();

    int counter = 0;
    for (int i = 0; i < m_width; ++i) {
        m_cubes.push_back(vector<vector<Voxel>>());

        for (int j = 0; j < m_height; ++j) {
            m_cubes[i].push_back(vector<Voxel>());

            for (int k = 0; k < m_depth; ++k) {
                char voxelId = std::stoi(list[counter]);
                counter++;
                m_cubes[i][j].push_back(Voxel {voxelId, 0});
            }
        }
    }

}

void Chunk::generateChunk() {

    int counterValue;
    {
        lock_guard<mutex> lock(s_mutex);
        ++counter;
        if (counter > maxCount)
            counter = 0;
        counterValue = counter;
    }

    m_isDirty = true;

    for (int x = 0; x < m_width; ++x) {
        m_cubes.push_back(vector<vector<Voxel>>());
        for (int y = 0; y < m_height; ++y) {
            m_cubes[x].push_back(vector<Voxel>());
            for (int z = 0; z < m_depth; ++z) {
                Voxel v;
                v.lightValue = 0;
                v.id = AIR;
                m_cubes[x][y].push_back(v);
            }
        }
    }

    // TODO There should be some kind of seed inorder to generate different worlds.
    // Currently the same world will be generated each time.

    NoiseMixer mixer{};
    mixer.addNoise(100.f, 15.f);
    mixer.addNoise(50, 5);
    mixer.addNoise(15, 3);

    for (int x = 0; x < m_width; ++x) {
        for (int z = 0; z < m_depth; ++z) {


            int noiseValue = mixer.computeNoise(m_xLocation + x, m_zLocation + z);

            for (int y = 0; y < m_height; ++y) {
                Voxel &v = m_cubes[x][y][z];

                if (y == 0) {
                    v.id = BED_ROCK;
                    continue;
                }
                if (y > 5 && y < 25)
                    v.id = config::cube_data::WATER;
                if (y < noiseValue)
                    v.id = counterValue;
            }
        }
    }
}

Voxel* Chunk::getVoxel2(int x, int y, int z) {
    if (x < m_width && x >= 0 && y < m_height && y >= 0 && z < m_depth
            && z >= 0) {

        return &m_cubes[x][y][z];
    } else if (x == m_width
            && (y < m_height && y >= 0 && z < m_depth && z >= 0)) {

        if (m_rightNeighbor)
            return &(m_rightNeighbor->m_cubes[0][y][z]);
        else
            return nullptr;
    } else if (x == -1 && (y < m_height && y >= 0 && z < m_depth && z >= 0)) {

        if (m_leftNeighbor)
            return &(m_leftNeighbor->m_cubes[m_width - 1][y][z]);
        else
            return nullptr;
    } else if (z == m_depth
            && (x < m_width && x >= 0 && y < m_height && y >= 0)) {

        if (m_backNeighbor)
            return &(m_backNeighbor->m_cubes[x][y][0]);
        else
            return nullptr;
    } else if (z == -1 && (x < m_width && x >= 0 && y < m_height && y >= 0)) {

        if (m_frontNeighbor)
            return &(m_frontNeighbor->m_cubes[x][y][m_depth - 1]);
        else
            return nullptr;
    }

    return nullptr;
}

void Chunk::updateLightningCubeRemoved(Voxel& voxel, int x, int y, int z) {
    updateDirtyRegions(y);

    // If the cube is adjacent to a neighbor, the neighbor needs to be update.
    if (x == m_width - 1 && m_rightNeighbor)
        m_rightNeighbor->updateDirtyRegions(y);

    if (x == 0 && m_leftNeighbor)
        m_leftNeighbor->updateDirtyRegions(y);

    if (z == m_depth - 1 && m_backNeighbor)
        m_backNeighbor->updateDirtyRegions(y);

    if (z == 0 && m_frontNeighbor)
        m_frontNeighbor->updateDirtyRegions(y);

    if (isInDirectSunlight(x, y, z)) {
        vector<vec3> lightPropagate;
        doSunLightning(lightPropagate, x, y, z);
        for (auto v : lightPropagate)
            propagateLight(v.x, v.y, v.z);

    } else {
        int highestLightValue = 0;
        highestLightValue = highestLVFromNeighbors(x, y, z) - 1;
        if (highestLightValue < 0)
            highestLightValue = 0;

        voxel.lightValue = highestLightValue;
        propagateLight(x, y, z);
    }

    updateGraphics(true);
    updateNeighborGraphics();
}

void Chunk::updateLightningCubeAdded(int x, int y, int z) {

    if (isInDirectSunlight(x, y, z)) {
        for (int i = y; i >= 0; --i)
            m_cubes[x][i][z].lightValue = 0;

        for (int i = y; i >= 0; --i)
            dePropagateLight(x, i ,z, 15);
    }
    dePropagateLight(x, y ,z);

    updateDirtyRegions(y);
    updateGraphics(true);
    updateNeighborGraphics();
}

void Chunk::updateNeighborGraphics() {
    if (m_rightNeighbor) {
        m_rightNeighbor->updateGraphics();

        if (m_rightNeighbor->m_backNeighbor) {
            m_rightNeighbor->m_backNeighbor->updateGraphics();
        }

        if (m_rightNeighbor->m_frontNeighbor) {
            m_rightNeighbor->m_frontNeighbor->updateGraphics();
        }
    }

    if (m_leftNeighbor) {
        m_leftNeighbor->updateGraphics();

        if (m_leftNeighbor->m_backNeighbor) {
            m_leftNeighbor->m_backNeighbor->updateGraphics();
        }

        if (m_leftNeighbor->m_frontNeighbor) {
            m_leftNeighbor->m_frontNeighbor->updateGraphics();
        }
    }

    if (m_backNeighbor) {
        m_backNeighbor->updateGraphics();
    }

    if (m_frontNeighbor) {
        m_frontNeighbor->updateGraphics();
    }
}

void Chunk::doSunLightning(vector<vec3> &lightPropagate, int x, int y, int z) {
    for (int i = y; y >= 0; i--) {
        auto &cube = m_cubes[x][i][z];
        int lightValue{m_directSunlight};
        if (cube.id == AIR || cube.id == WATER) {
            if (cube.id == WATER && lightValue > 0)
                --lightValue;

            cube.lightValue = lightValue;
            lightPropagate.push_back(vec3(x, i, z));

            m_lightsToPropagate.push_back(vec3(x, y, z));
        } else {
            break;
        }
    }
}

void Chunk::propagateLight(int x, int y, int z) {
    Voxel &voxel = m_cubes[x][y][z];
    int lvInitial = voxel.lightValue - 1;
    vector<vec3> newPropagates;

    // ########################################################################

    // Traverse right
    int lv = lvInitial;
    for (int i = x + 1; lv > 0; i++) {
        if (i < m_width) {
            Voxel &v = m_cubes[i][y][z];
            if (v.id == AIR && v.lightValue < lv) {
                v.lightValue = lv;
                newPropagates.push_back(vec3(i, y, z));
                lv--;
                updateDirtyRegions(y);
            } else {
                break;
            }
        } else {
            if (m_rightNeighbor
                    && m_rightNeighbor->m_cubes[0][y][z].id == AIR
                    && m_rightNeighbor->m_cubes[0][y][z].lightValue < lv) {

                m_rightNeighbor->m_cubes[0][y][z].lightValue = lv;
                m_rightNeighbor->propagateLight(0, y, z);
            }
            break;
        }
    }

    // Travers left
    lv = lvInitial;
    for (int i = x - 1; lv > 0; i--) {
        if (i >= 0) {
            Voxel &v = m_cubes[i][y][z];
            if (v.id == AIR && v.lightValue < lv) {
                v.lightValue = lv;
                newPropagates.push_back(vec3(i, y, z));
                lv--;
                updateDirtyRegions(y);
            } else {
                break;
            }
        } else {
            if (m_leftNeighbor
                    && m_leftNeighbor->m_cubes[m_width - 1][y][z].id == AIR
                    && m_leftNeighbor->m_cubes[m_width - 1][y][z].lightValue
                            < lv) {

                m_leftNeighbor->m_cubes[m_width - 1][y][z].lightValue = lv;
                m_leftNeighbor->propagateLight(m_width - 1, y, z);
            }
            break;
        }
    }

    // ########################################################################

    // Traverse up
    lv = lvInitial;
    for (int i = y + 1; i < m_height; i++) {
        Voxel &v = m_cubes[x][i][z];
        if (v.id == AIR && v.lightValue < lv) {
            v.lightValue = lv;
            newPropagates.push_back(vec3(x, i, z));
            lv--;
            updateDirtyRegions(y);
        } else {
            break;
        }
    }

    // Treaverse down
    lv = lvInitial;
    for (int i = y - 1; i >= 0; i--) {
        Voxel &v = m_cubes[x][i][z];
        if (v.id == AIR && v.lightValue < lv) {
            v.lightValue = lv;
            newPropagates.push_back(vec3(x, i, z));
            lv--;
            updateDirtyRegions(y);
        } else {
            break;
        }
    }

    // ########################################################################

    // Traverse backwards
    lv = lvInitial;
    for (int i = z + 1; lv > 0; i++) {
        if (i < m_depth) {
            Voxel &v = m_cubes[x][y][i];
            if (v.id == AIR && v.lightValue < lv) {
                v.lightValue = lv;
                newPropagates.push_back(vec3(x, y, i));
                lv--;
                updateDirtyRegions(y);
            } else {
                break;
            }
        } else {
            if (m_backNeighbor
                    && m_backNeighbor->m_cubes[x][y][0].id == AIR
                    && m_backNeighbor->m_cubes[x][y][0].lightValue < lv) {

                m_backNeighbor->m_cubes[x][y][0].lightValue = lv;
                m_backNeighbor->propagateLight(x, y, 0);
            }
            break;
        }
    }

    // Traverse forwards
    lv = lvInitial;
    for (int i = z - 1; lv > 0; i--) {
        if (i >= 0) {
            Voxel &v = m_cubes[x][y][i];
            if (v.id == AIR && v.lightValue < lv) {
                v.lightValue = lv;
                newPropagates.push_back(vec3(x, y, i));
                lv--;
                updateDirtyRegions(y);
            } else {
                break;
            }
        } else {
            if (m_frontNeighbor
                    && m_frontNeighbor->m_cubes[x][y][m_depth - 1].id == AIR
                    && m_frontNeighbor->m_cubes[x][y][m_depth - 1].lightValue
                            < lv) {

                m_frontNeighbor->m_cubes[x][y][m_depth - 1].lightValue = lv;
                m_frontNeighbor->propagateLight(x, y, m_depth - 1);
            }
            break;
        }
    }

    for (vec3 vec : newPropagates)
        propagateLight(vec.x, vec.y, vec.z);

}

void Chunk::updateDirtyRegions(int y) {
    int region = y / GRAPHICAL_CHUNK_HEIGHT;
    m_dirtyRegions.emplace(region);
}

void Chunk::dePropagateLight(int x, int y, int z, int _lightValue) {
    Voxel &voxel = m_cubes[x][y][z];

    int lightValue = voxel.lightValue;;
    if (_lightValue != -1)
        lightValue = _lightValue;

    voxel.lightValue = 0;
    updateDirtyRegions(y);

    // ########################################################################

    // Right
    if (x < m_width - 1) {
        if (m_cubes[x + 1][y][z].id == AIR &&
                m_cubes[x + 1][y][z].lightValue < lightValue &&
                highestLVFromNeighbors(x + 1, y, z) <= lightValue) {

            dePropagateLight(x + 1, y, z);
        }
        else
            propagateLight(x + 1, y, z);

    } else if (x == m_width - 1) {

        if (m_rightNeighbor &&
                m_rightNeighbor->m_cubes[0][y][z].id == AIR &&
                m_rightNeighbor->m_cubes[0][y][z].lightValue < lightValue &&
                m_rightNeighbor->highestLVFromNeighbors(0, y, z)
                <= lightValue
                ) {

            m_rightNeighbor->dePropagateLight(0, y, z);
        }
        else
            m_rightNeighbor->propagateLight(0, y, z);
    }

    // Left
    if (x > 0) {
        if (m_cubes[x - 1][y][z].id == AIR &&
                m_cubes[x - 1][y][z].lightValue < lightValue &&
                highestLVFromNeighbors(x - 1, y, z) <= lightValue) {

            dePropagateLight(x - 1, y, z);
        }
        else
            propagateLight(x - 1, y, z);

    } else if (x == 0) {

        if (m_leftNeighbor &&
                m_leftNeighbor->m_cubes[m_width - 1][y][z].id == AIR	&&
                m_leftNeighbor->m_cubes[m_width - 1][y][z].lightValue <
                    lightValue &&
                    m_leftNeighbor->highestLVFromNeighbors(m_width - 1, y, z)
                    <= lightValue
                    ) {

            m_leftNeighbor->dePropagateLight(m_width - 1, y, z);
        }
        else
            m_leftNeighbor->propagateLight(m_width - 1, y, z);
    }

    // ########################################################################

    // Up
    if (y < m_height - 1) {
        if (m_cubes[x][y + 1][z].id == AIR &&
                m_cubes[x][y + 1][z].lightValue < lightValue &&
                highestLVFromNeighbors(x, y + 1, z) <= lightValue) {

            dePropagateLight(x, y + 1, z);
        }
        else {
            propagateLight(x, y + 1, z);
        }
    }

    // Down
    if (_lightValue == -1 && y > 0) {
        if (m_cubes[x][y - 1][z].id == AIR &&
                m_cubes[x][y - 1][z].lightValue < lightValue &&
                highestLVFromNeighbors(x, y - 1, z) <= lightValue) {

            dePropagateLight(x, y - 1, z);
        }
        else {
            propagateLight(x, y - 1, z);
        }
    }

    // ########################################################################

    // Backwards
    if (z < m_depth - 1) {
        if (m_cubes[x][y][z + 1].id == AIR &&
                m_cubes[x][y][z + 1].lightValue < lightValue &&
                highestLVFromNeighbors(x, y, z + 1) <= lightValue) {

            dePropagateLight(x, y, z + 1);
        }
        else
            propagateLight(x, y, z + 1);

    } else if (z == m_depth - 1) {

        if (m_backNeighbor &&
                m_backNeighbor->m_cubes[x][y][0].id == AIR &&
                m_backNeighbor->m_cubes[x][y][0].lightValue < lightValue &&
                m_backNeighbor->highestLVFromNeighbors(x, y, 0)
                    <= lightValue
                    ) {

            m_backNeighbor->dePropagateLight(x, y, 0);
        }
        else
            m_rightNeighbor->propagateLight(x, y, 0);
    }

    // Forwards
    if (z > 0) {
        if (m_cubes[x][y][z - 1].id == AIR &&
                m_cubes[x][y][z - 1].lightValue < lightValue &&
                highestLVFromNeighbors(x, y, z - 1) <= lightValue) {

            dePropagateLight(x, y, z - 1);
        }
        else
            propagateLight(x, y, z - 1);

    } else if (z == 0) {
        if (m_frontNeighbor &&
                m_frontNeighbor->m_cubes[x][y][m_depth - 1].id == AIR &&
                m_frontNeighbor->m_cubes[x][y][m_depth - 1].lightValue
                    < lightValue &&
                m_frontNeighbor->highestLVFromNeighbors(x, y, m_depth - 1)
                    <= lightValue
                    ) {

            m_frontNeighbor->dePropagateLight(x, y, m_depth - 1);
        }
        else
            m_frontNeighbor->propagateLight(x, y, m_depth - 1);
    }

}

int Chunk::highestLVFromNeighbors(int x, int y, int z) {
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
    if (y < m_height)
        return getVoxel(x, y + 1, z).lightValue == m_directSunlight;

    return false;
}

}


// Old function, could be usefull somtime :p
//void Chunk::updateLightning() {
//
//  vector<vec3> lightPropagate;
//
//  vector<vec3> lightPropagateRight;
//  vector<vec3> lightPropagateRightBack;
//  vector<vec3> lightPropagateRightFront;
//
//  vector<vec3> lightPropagateLeft;
//  vector<vec3> lightPropagateLeftBack;
//  vector<vec3> lightPropagateLeftFront;
//
//  vector<vec3> lightPropagateBack;
//  vector<vec3> lightPropagateFront;
//
//  // Sun light ####################################################
//
//  doSunLightning(lightPropagate);
//
//  if (m_rightNeighbor) {
//
//      m_rightNeighbor->doSunLightning(lightPropagateRight);
//
//      if (m_rightNeighbor->m_backNeighbor)
//          m_rightNeighbor->m_backNeighbor->doSunLightning(
//                  lightPropagateRightBack);
//
//      if (m_rightNeighbor->m_frontNeighbor)
//          m_rightNeighbor->m_frontNeighbor->doSunLightning(
//                  lightPropagateRightFront);
//
//  }
//
//  if (m_leftNeighbor) {
//      m_leftNeighbor->doSunLightning(lightPropagateLeft);
//
//      if (m_leftNeighbor->m_backNeighbor)
//          m_leftNeighbor->m_backNeighbor->doSunLightning(
//                  lightPropagateLeftBack);
//
//      if (m_leftNeighbor->m_frontNeighbor)
//          m_leftNeighbor->m_frontNeighbor->doSunLightning(
//                  lightPropagateLeftFront);
//
//  }
//
//  if (m_backNeighbor)
//      m_backNeighbor->doSunLightning(lightPropagateBack);
//
//  if (m_frontNeighbor)
//      m_frontNeighbor->doSunLightning(lightPropagateFront);
//
//  // Propagate light ##############################################
//
//  for (vec3 vec : lightPropagate)
//      propagateLight(vec.x, vec.y, vec.z);
//
//  if (m_rightNeighbor) {
//      m_rightNeighbor->collectLightFromRightNeighbor(lightPropagateRight);
//      for (vec3 vec : lightPropagateRight)
//          m_rightNeighbor->propagateLight(vec.x, vec.y, vec.z);
//
//      if (m_rightNeighbor->m_backNeighbor.get()) {
//          m_rightNeighbor->m_backNeighbor->collectLightFromRightNeighbor(
//                  lightPropagateRightBack);
//          m_rightNeighbor->m_backNeighbor->collectLightFromBackNeighbor(
//                  lightPropagateRightBack);
//
//          for (vec3 vec : lightPropagateRightBack)
//              m_rightNeighbor->m_backNeighbor->propagateLight(vec.x, vec.y,
//                      vec.z);
//
//      }
//
//      if (m_rightNeighbor->m_frontNeighbor) {
//          m_rightNeighbor->m_frontNeighbor->collectLightFromRightNeighbor(
//                  lightPropagateRightFront);
//          m_rightNeighbor->m_frontNeighbor->collectLightFromFrontNeighbor(
//                  lightPropagateRightFront);
//
//          for (vec3 vec : lightPropagateRightFront)
//              m_rightNeighbor->m_frontNeighbor->propagateLight(vec.x, vec.y,
//                      vec.z);
//
//      }
//  }
//
//  if (m_leftNeighbor) {
//      m_leftNeighbor->collectLightFromLeftNeighbor(lightPropagateLeft);
//      for (vec3 vec : lightPropagateLeft)
//          m_leftNeighbor->propagateLight(vec.x, vec.y, vec.z);
//
//      if (m_leftNeighbor->m_backNeighbor.get()) {
//          m_leftNeighbor->m_backNeighbor->collectLightFromLeftNeighbor(
//                  lightPropagateLeftBack);
//          m_leftNeighbor->m_backNeighbor->collectLightFromBackNeighbor(
//                  lightPropagateLeftBack);
//
//          for (vec3 vec : lightPropagateLeftBack)
//              m_leftNeighbor->m_backNeighbor->propagateLight(vec.x, vec.y,
//                      vec.z);
//
//      }
//
//      if (m_leftNeighbor->m_frontNeighbor) {
//          m_leftNeighbor->m_frontNeighbor->collectLightFromLeftNeighbor(
//                  lightPropagateLeftFront);
//          m_leftNeighbor->m_frontNeighbor->collectLightFromFrontNeighbor(
//                  lightPropagateLeftFront);
//
//          for (vec3 vec : lightPropagateLeftFront)
//              m_leftNeighbor->m_frontNeighbor->propagateLight(vec.x, vec.y,
//                      vec.z);
//
//      }
//  }
//
//  if (m_backNeighbor) {
//      m_backNeighbor->collectLightFromBackNeighbor(lightPropagateBack);
//      for (vec3 vec : lightPropagateBack)
//          m_backNeighbor->propagateLight(vec.x, vec.y, vec.z);
//
//  }
//
//  if (m_frontNeighbor) {
//      m_frontNeighbor->collectLightFromFrontNeighbor(lightPropagateFront);
//      for (vec3 vec : lightPropagateFront)
//          m_frontNeighbor->propagateLight(vec.x, vec.y, vec.z);
//
//  }
//
//}


