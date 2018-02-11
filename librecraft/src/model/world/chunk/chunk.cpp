#include "chunk.h"

#include <fstream>
#include <iostream>
#include <queue>

#include "../../../graphics/chunkBatcher.h"
#include "../../../graphics/graphicsManager.h"
#include "../terrainGen/terrainGenerator.h"

using namespace std;
using namespace glm;

using namespace graphics;
using namespace config::cube_data;
using namespace config::chunk_data;
using namespace terrainGen;

namespace chunk {

Chunk::Chunk(string worldName,
             int x,
             int z,
             graphics::GraphicsManager& graphicsManager)
  : m_xLocation{x}
  , m_zLocation{z}
  , m_graphicsManager{graphicsManager}
  , m_name{createChunkName(worldName)} {
  for (int i{0}; i < CHUNK_HEIGHT / GRAPHICAL_CHUNK_HEIGHT; ++i) {
    m_graphicalChunksIds.push_back(-1);
    m_dirtyRegions.emplace(i);
  }
}

Chunk::~Chunk() {
  for (int graphicalChunk : m_graphicalChunksIds)
    m_graphicsManager.getChunkBatcher().removeBatch(graphicalChunk);
}

void
Chunk::create(CreationOptions& options) {
  ifstream stream;
  stream.open(m_name);
  if (stream.fail())
    generateChunk(options);
  else
    loadChunk();
}

void
Chunk::doSunLightning() {
  for (int x{0}; x < CHUNK_WIDTH_AND_DEPTH; ++x) {
    for (int z{0}; z < CHUNK_WIDTH_AND_DEPTH; ++z) {
      vector<vec3> dummy;
      doSunLightning(dummy, x, CHUNK_HEIGHT - 1, z, true);
      m_lightsToPropagate.insert(
        m_lightsToPropagate.end(), dummy.begin(), dummy.end());
    }
  }
}

void
Chunk::collectLightFromAllNeighbors() {
  collectLightFromRightNeighbor();
  collectLightFromLeftNeighbor();
  collectLightFromBackNeighbor();
  collectLightFromFrontNeighbor();
}

void
Chunk::collectLightFromRightNeighbor() {
  if (m_rightNeighbor.get()) {
    for (int j{0}; j < CHUNK_HEIGHT; ++j) {
      for (int k{0}; k < CHUNK_WIDTH_AND_DEPTH; ++k) {
        char lv = m_rightNeighbor->m_cubes[0][j][k].getSunLightValue() - 1;
        // TODO collect other light...

        if (m_rightNeighbor->m_cubes[0][j][k].getId() == AIR &&
            m_cubes[15][j][k].getId() == AIR &&
            lv > m_cubes[15][j][k].getSunLightValue()) {
          m_cubes[15][j][k].setSunLightValue(lv);
          m_lightsToPropagate.push_back(vec3(15, j, k));
        }
      }
    }
  }
}

void
Chunk::collectLightFromLeftNeighbor() {
  if (m_leftNeighbor.get()) {
    for (int j{0}; j < CHUNK_HEIGHT; ++j) {
      for (int k{0}; k < CHUNK_WIDTH_AND_DEPTH; ++k) {
        char lv = m_leftNeighbor->m_cubes[15][j][k].getSunLightValue() - 1;
        if (m_leftNeighbor->m_cubes[15][j][k].getId() == AIR &&
            m_cubes[0][j][k].getId() == AIR &&
            lv > m_cubes[0][j][k].getSunLightValue()) {
          m_cubes[0][j][k].setSunLightValue(lv);
          m_lightsToPropagate.push_back(vec3(0, j, k));
        }
      }
    }
  }
}

void
Chunk::collectLightFromBackNeighbor() {
  if (m_backNeighbor.get()) {
    for (int i{0}; i < CHUNK_WIDTH_AND_DEPTH; ++i) {
      for (int j{0}; j < CHUNK_HEIGHT; ++j) {
        char lv = m_backNeighbor->m_cubes[i][j][0].getSunLightValue() - 1;

        if (m_backNeighbor->m_cubes[i][j][0].getId() == AIR &&
            m_cubes[i][j][15].getId() == AIR &&
            lv > m_cubes[i][j][15].getSunLightValue()) {
          m_cubes[i][j][15].setSunLightValue(lv);
          m_lightsToPropagate.push_back(vec3(i, j, 15));
        }
      }
    }
  }
}

void
Chunk::collectLightFromFrontNeighbor() {
  if (m_frontNeighbor.get()) {
    for (int i{0}; i < CHUNK_WIDTH_AND_DEPTH; ++i) {
      for (int j{0}; j < CHUNK_HEIGHT; ++j) {
        char lv = m_frontNeighbor->m_cubes[i][j][15].getSunLightValue() - 1;

        if (m_frontNeighbor->m_cubes[i][j][15].getId() == AIR &&
            m_cubes[i][j][0].getId() == AIR &&
            lv > m_cubes[i][j][0].getSunLightValue()) {
          m_cubes[i][j][0].setSunLightValue(lv);
          m_lightsToPropagate.push_back(vec3(i, j, 0));
        }
      }
    }
  }
}

void
Chunk::propagateLights() {
  for (vec3& l : m_lightsToPropagate)
    propagateSunLight(l.x, l.y, l.z);

  m_lightsToPropagate.clear(); // Is This okay?!?

  for (vec3& l : m_otherLightSources)
    propagateOtherLight(l.x, l.y, l.z);

  m_otherLightSources.clear();
}

void
Chunk::forceUpdateGraphics() {
  for (int i{0}; i < CHUNK_HEIGHT / GRAPHICAL_CHUNK_HEIGHT; ++i)
    m_dirtyRegions.emplace(i);

  updateGraphics();
}

void
Chunk::updateGraphics(bool highPriority) {
  vector<vector<vector<Voxel>>>* right = nullptr;
  vector<vector<vector<Voxel>>>* left = nullptr;
  vector<vector<vector<Voxel>>>* front = nullptr;
  vector<vector<vector<Voxel>>>* back = nullptr;

  if (m_rightNeighbor)
    right = &(m_rightNeighbor->m_cubes);

  if (m_leftNeighbor)
    left = &(m_leftNeighbor->m_cubes);

  if (m_frontNeighbor)
    front = &(m_frontNeighbor->m_cubes);

  if (m_backNeighbor)
    back = &(m_backNeighbor->m_cubes);

  for (int i : m_dirtyRegions) {

    shared_ptr<GraphicalChunk> batch =
      make_shared<GraphicalChunk>(m_xLocation,
                                  i * GRAPHICAL_CHUNK_HEIGHT,
                                  m_zLocation,
                                  m_cubes,
                                  right,
                                  left,
                                  back,
                                  front);

    int derp = m_graphicsManager.getChunkBatcher().addBatch(
      m_graphicalChunksIds[i], batch, highPriority);

    m_graphicalChunksIds[i] = derp;
  }
  m_dirtyRegions.clear();
}

Voxel
Chunk::getVoxel(int x, int y, int z) {
  return m_cubes[x][y][z];
}

void
Chunk::setCube(int x, int y, int z, char id) {
  // We can not change bedrock!
  if (m_cubes[x][y][z].getId() == BED_ROCK)
    return;

  Voxel& voxel = m_cubes[x][y][z];
  int replacedId = voxel.getId();
  voxel.setId(id);

  // If we removed a cube
  if (id == AIR || id == WATER) {
    if (replacedId == LIGHT)
      dePropagateOtherlight(x, y, z);

    updateLightningCubeRemoved(voxel, x, y, z);
  } else { // We added a cube
    updateLightningCubeAdded(x, y, z);
  }
  m_isDirty = true;
}

void
Chunk::removeAllNeighbors() {
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

void
Chunk::storeChunk() {
  if (!m_isDirty)
    return;

  ofstream outStream(m_name);
  for (int x = 0; x < m_width; ++x) {
    for (int y = 0; y < m_height; ++y) {
      for (int z = 0; z < m_depth; ++z) {
        outStream << to_string(m_cubes[x][y][z].getId()) << "\n";
      }
    }
  }
  outStream.close();
}

std::string
Chunk::createChunkName(std::string worldName) {
  return config::dataFolder + worldName + "_" + std::to_string(m_xLocation) +
         "_" + std::to_string(m_zLocation) + ".chunk";
}

void
Chunk::loadChunk() {
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
        ++counter;
        if (voxelId == LIGHT) {
          m_cubes[i][j].push_back(Voxel{voxelId, 0, m_directSunlight});
          m_otherLightSources.push_back(vec3(i, j, k));
        } else {
          m_cubes[i][j].push_back(Voxel{voxelId, 0, 0});
        }
      }
    }
  }
}

void
Chunk::generateChunk(CreationOptions& options) {
  terrainGen::TerrainGenerator generator{
    config::chunk_data::CHUNK_WIDTH_AND_DEPTH,
    config::chunk_data::CHUNK_HEIGHT,
    config::chunk_data::CHUNK_WIDTH_AND_DEPTH,
    options.getSeed()};
  m_cubes = generator.generateTerrain(options, m_xLocation, m_zLocation);
}

Voxel*
Chunk::getVoxel2(int x, int y, int z) {
  if (x < m_width && x >= 0 && y < m_height && y >= 0 && z < m_depth &&
      z >= 0) {
    return &m_cubes[x][y][z];
  } else if (x == m_width &&
             (y < m_height && y >= 0 && z < m_depth && z >= 0)) {
    if (m_rightNeighbor)
      return &(m_rightNeighbor->m_cubes[0][y][z]);
    else
      return nullptr;
  } else if (x == -1 && (y < m_height && y >= 0 && z < m_depth && z >= 0)) {
    if (m_leftNeighbor)
      return &(m_leftNeighbor->m_cubes[m_width - 1][y][z]);
    else
      return nullptr;
  } else if (z == m_depth &&
             (x < m_width && x >= 0 && y < m_height && y >= 0)) {
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

void
Chunk::updateLightningCubeRemoved(Voxel& voxel, int x, int y, int z) {
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
    for (vec3& v : lightPropagate)
      propagateSunLight(v.x, v.y, v.z);

  } else {
    int highestSLV = std::max(highestSunLVFromNeighbors(x, y, z) - 1, 0);
    voxel.setSunLightValue(highestSLV);
    propagateSunLight(x, y, z);
  }

  int highestOLV = std::max(highestOtherLVFromNeighbors(x, y, z) - 1, 0);
  voxel.setOtherLightValue(highestOLV);
  propagateOtherLight(x, y, z);

  updateGraphics(true);
  updateNeighborGraphics();
}

void
Chunk::updateLightningCubeAdded(int x, int y, int z) {
  if (isInDirectSunlight(x, y, z)) {
    for (int i = y; i >= 0; --i)
      m_cubes[x][i][z].setSunLightValue(0);

    for (int i = y; i >= 0; --i)
      dePropagateSunlight(x, i, z, 15);
  }
  dePropagateSunlight(x, y, z);

  Voxel& v{m_cubes[x][y][z]};
  if (v.getId() == LIGHT) {
    v.setOtherLightValue(m_directSunlight);
    propagateOtherLight(x, y, z);
  } else {
    dePropagateOtherlight(x, y, z);
  }

  updateDirtyRegions(y);
  updateGraphics(true);
  updateNeighborGraphics();
}

void
Chunk::updateNeighborGraphics() {
  if (m_rightNeighbor) {
    m_rightNeighbor->updateGraphics();

    if (m_rightNeighbor->m_backNeighbor)
      m_rightNeighbor->m_backNeighbor->updateGraphics();

    if (m_rightNeighbor->m_frontNeighbor)
      m_rightNeighbor->m_frontNeighbor->updateGraphics();
  }

  if (m_leftNeighbor) {
    m_leftNeighbor->updateGraphics();

    if (m_leftNeighbor->m_backNeighbor)
      m_leftNeighbor->m_backNeighbor->updateGraphics();

    if (m_leftNeighbor->m_frontNeighbor)
      m_leftNeighbor->m_frontNeighbor->updateGraphics();
  }

  if (m_backNeighbor)
    m_backNeighbor->updateGraphics();

  if (m_frontNeighbor)
    m_frontNeighbor->updateGraphics();
}

void
Chunk::doSunLightning(vector<vec3>& lightPropagate,
                      int x,
                      int y,
                      int z,
                      bool useVec) {

  static int lightReduction{1};
  // Sun lightning, only air and water gets light.
  // Each step in water reduces the light strength lightReduction.
  int lightValue{m_directSunlight};
  for (int i{y}; i >= 0; --i) {
    Voxel& cube = m_cubes[x][i][z];

    if (cube.getId() == AIR || cube.getId() == WATER) {
      if (cube.getId() == WATER && lightValue > 0)
        lightValue -= lightReduction;

      cube.setSunLightValue(lightValue);
      if (useVec)
        lightPropagate.push_back(vec3(x, i, z));

    } else {
      return;
    }
  }
}

void
Chunk::propagateSunLight(int x, int y, int z) {
  Voxel& voxel = m_cubes[x][y][z];
  int lvInitial = voxel.getSunLightValue() - 1;
  vector<vec3> newPropagates;

  // ########################################################################

  // Traverse right
  int lv = lvInitial;
  for (int i = x + 1; lv > 0; ++i) {
    if (i < m_width) {
      Voxel& v = m_cubes[i][y][z];
      if (v.getId() == AIR && v.getSunLightValue() < lv) {
        v.setSunLightValue(lv);
        newPropagates.push_back(vec3(i, y, z));
        --lv;
        updateDirtyRegions(y);
      } else {
        break;
      }
    } else {
      if (m_rightNeighbor && m_rightNeighbor->m_cubes[0][y][z].getId() == AIR &&
          m_rightNeighbor->m_cubes[0][y][z].getSunLightValue() < lv) {

        m_rightNeighbor->m_cubes[0][y][z].setSunLightValue(lv);
        m_rightNeighbor->propagateSunLight(0, y, z);
      }
      break;
    }
  }

  // Travers left
  lv = lvInitial;
  for (int i = x - 1; lv > 0; --i) {
    if (i >= 0) {
      Voxel& v = m_cubes[i][y][z];
      if (v.getId() == AIR && v.getSunLightValue() < lv) {
        v.setSunLightValue(lv);
        newPropagates.push_back(vec3(i, y, z));
        --lv;
        updateDirtyRegions(y);
      } else {
        break;
      }
    } else {
      if (m_leftNeighbor &&
          m_leftNeighbor->m_cubes[m_width - 1][y][z].getId() == AIR &&
          m_leftNeighbor->m_cubes[m_width - 1][y][z].getSunLightValue() < lv) {

        m_leftNeighbor->m_cubes[m_width - 1][y][z].setSunLightValue(lv);
        m_leftNeighbor->propagateSunLight(m_width - 1, y, z);
      }
      break;
    }
  }

  // ########################################################################

  // Traverse up
  lv = lvInitial;
  for (int i = y + 1; i < m_height; ++i) {
    Voxel& v = m_cubes[x][i][z];
    if (v.getId() == AIR && v.getSunLightValue() < lv) {
      v.setSunLightValue(lv);
      newPropagates.push_back(vec3(x, i, z));
      --lv;
      updateDirtyRegions(y);
    } else {
      break;
    }
  }

  // Treaverse down
  lv = lvInitial;
  for (int i = y - 1; i >= 0; --i) {
    Voxel& v = m_cubes[x][i][z];
    if (v.getId() == AIR && v.getSunLightValue() < lv) {
      v.setSunLightValue(lv);
      newPropagates.push_back(vec3(x, i, z));
      --lv;
      updateDirtyRegions(y);
    } else {
      break;
    }
  }

  // ########################################################################

  // Traverse backwards
  lv = lvInitial;
  for (int i = z + 1; lv > 0; ++i) {
    if (i < m_depth) {
      Voxel& v = m_cubes[x][y][i];
      if (v.getId() == AIR && v.getSunLightValue() < lv) {
        v.setSunLightValue(lv);
        newPropagates.push_back(vec3(x, y, i));
        --lv;
        updateDirtyRegions(y);
      } else {
        break;
      }
    } else {
      if (m_backNeighbor && m_backNeighbor->m_cubes[x][y][0].getId() == AIR &&
          m_backNeighbor->m_cubes[x][y][0].getSunLightValue() < lv) {
        m_backNeighbor->m_cubes[x][y][0].setSunLightValue(lv);
        m_backNeighbor->propagateSunLight(x, y, 0);
      }
      break;
    }
  }

  // Traverse forwards
  lv = lvInitial;
  for (int i = z - 1; lv > 0; --i) {
    if (i >= 0) {
      Voxel& v = m_cubes[x][y][i];
      if (v.getId() == AIR && v.getSunLightValue() < lv) {
        v.setSunLightValue(lv);
        newPropagates.push_back(vec3(x, y, i));
        --lv;
        updateDirtyRegions(y);
      } else {
        break;
      }
    } else {
      if (m_frontNeighbor &&
          m_frontNeighbor->m_cubes[x][y][m_depth - 1].getId() == AIR &&
          m_frontNeighbor->m_cubes[x][y][m_depth - 1].getSunLightValue() < lv) {
        m_frontNeighbor->m_cubes[x][y][m_depth - 1].setSunLightValue(lv);
        m_frontNeighbor->propagateSunLight(x, y, m_depth - 1);
      }
      break;
    }
  }

  for (vec3 vec : newPropagates)
    propagateSunLight(vec.x, vec.y, vec.z);
}

// Yepp this is duplicated code, deal with it.
void
Chunk::propagateOtherLight(int x, int y, int z) {
  Voxel& voxel = m_cubes[x][y][z];
  int lvInitial = voxel.getOtherLightValue() - 1;

  if (lvInitial <= 0)
    return;

  vector<vec3> newPropagates;

  // ########################################################################

  // Traverse right
  int lv = lvInitial;
  for (int i = x + 1; lv > 0; ++i) {
    if (i < m_width) {
      Voxel& v = m_cubes[i][y][z];
      if (v.getId() == AIR && v.getOtherLightValue() < lv) {
        v.setOtherLightValue(lv);
        newPropagates.push_back(vec3(i, y, z));
        --lv;
        updateDirtyRegions(y);
      } else {
        break;
      }
    } else {
      if (m_rightNeighbor && m_rightNeighbor->m_cubes[0][y][z].getId() == AIR &&
          m_rightNeighbor->m_cubes[0][y][z].getOtherLightValue() < lv) {

        m_rightNeighbor->m_cubes[0][y][z].setOtherLightValue(lv);
        m_rightNeighbor->propagateOtherLight(0, y, z);
      }
      break;
    }
  }

  // Traverse left
  lv = lvInitial;
  for (int i = x - 1; lv > 0; --i) {
    if (i >= 0) {
      Voxel& v = m_cubes[i][y][z];
      if (v.getId() == AIR && v.getOtherLightValue() < lv) {
        v.setOtherLightValue(lv);
        newPropagates.push_back(vec3(i, y, z));
        --lv;
        updateDirtyRegions(y);
      } else {
        break;
      }
    } else {
      if (m_leftNeighbor &&
          m_leftNeighbor->m_cubes[m_width - 1][y][z].getId() == AIR &&
          m_leftNeighbor->m_cubes[m_width - 1][y][z].getOtherLightValue() <
            lv) {

        m_leftNeighbor->m_cubes[m_width - 1][y][z].setOtherLightValue(lv);
        m_leftNeighbor->propagateOtherLight(m_width - 1, y, z);
      }
      break;
    }
  }

  // ########################################################################

  // Traverse up
  lv = lvInitial;
  for (int i = y + 1; i < m_height; ++i) {
    Voxel& v = m_cubes[x][i][z];
    if (v.getId() == AIR && v.getOtherLightValue() < lv) {
      v.setOtherLightValue(lv);
      newPropagates.push_back(vec3(x, i, z));
      --lv;
      updateDirtyRegions(y);
    } else {
      break;
    }
  }

  // Treaverse down
  lv = lvInitial;
  for (int i = y - 1; i >= 0; --i) {
    Voxel& v = m_cubes[x][i][z];
    if (v.getId() == AIR && v.getOtherLightValue() < lv) {
      v.setOtherLightValue(lv);
      newPropagates.push_back(vec3(x, i, z));
      --lv;
      updateDirtyRegions(y);
    } else {
      break;
    }
  }

  // ########################################################################

  // Traverse backwards
  lv = lvInitial;
  for (int i = z + 1; lv > 0; ++i) {
    if (i < m_depth) {
      Voxel& v = m_cubes[x][y][i];
      if (v.getId() == AIR && v.getOtherLightValue() < lv) {
        v.setOtherLightValue(lv);
        newPropagates.push_back(vec3(x, y, i));
        --lv;
        updateDirtyRegions(y);
      } else {
        break;
      }
    } else {
      if (m_backNeighbor && m_backNeighbor->m_cubes[x][y][0].getId() == AIR &&
          m_backNeighbor->m_cubes[x][y][0].getOtherLightValue() < lv) {
        m_backNeighbor->m_cubes[x][y][0].setOtherLightValue(lv);
        m_backNeighbor->propagateOtherLight(x, y, 0);
      }
      break;
    }
  }

  // Traverse forwards
  lv = lvInitial;
  for (int i = z - 1; lv > 0; --i) {
    if (i >= 0) {
      Voxel& v = m_cubes[x][y][i];
      if (v.getId() == AIR && v.getOtherLightValue() < lv) {
        v.setOtherLightValue(lv);
        newPropagates.push_back(vec3(x, y, i));
        --lv;
        updateDirtyRegions(y);
      } else {
        break;
      }
    } else {
      if (m_frontNeighbor &&
          m_frontNeighbor->m_cubes[x][y][m_depth - 1].getId() == AIR &&
          m_frontNeighbor->m_cubes[x][y][m_depth - 1].getOtherLightValue() <
            lv) {
        m_frontNeighbor->m_cubes[x][y][m_depth - 1].setOtherLightValue(lv);
        m_frontNeighbor->propagateOtherLight(x, y, m_depth - 1);
      }
      break;
    }
  }

  for (vec3 vec : newPropagates)
    propagateOtherLight(vec.x, vec.y, vec.z);
}

void
Chunk::updateDirtyRegions(int y) {
  int region = y / GRAPHICAL_CHUNK_HEIGHT;
  m_dirtyRegions.emplace(region);
}

void
Chunk::dePropagateSunlight(int x, int y, int z, int _lightValue) {
  Voxel& voxel = m_cubes[x][y][z];

  int lightValue = voxel.getSunLightValue();
  if (_lightValue != -1)
    lightValue = _lightValue;

  voxel.setSunLightValue(0);
  updateDirtyRegions(y);

  // ########################################################################

  // Right
  if (x < m_width - 1) {
    if (m_cubes[x + 1][y][z].getId() == AIR &&
        m_cubes[x + 1][y][z].getSunLightValue() < lightValue &&
        highestSunLVFromNeighbors(x + 1, y, z) <= lightValue) {
      dePropagateSunlight(x + 1, y, z);
    } else
      propagateSunLight(x + 1, y, z);

  } else if (x == m_width - 1) {
    if (m_rightNeighbor && m_rightNeighbor->m_cubes[0][y][z].getId() == AIR &&
        m_rightNeighbor->m_cubes[0][y][z].getSunLightValue() < lightValue &&
        m_rightNeighbor->highestSunLVFromNeighbors(0, y, z) <= lightValue) {
      m_rightNeighbor->dePropagateSunlight(0, y, z);
    } else
      m_rightNeighbor->propagateSunLight(0, y, z);
  }

  // Left
  if (x > 0) {
    if (m_cubes[x - 1][y][z].getId() == AIR &&
        m_cubes[x - 1][y][z].getSunLightValue() < lightValue &&
        highestSunLVFromNeighbors(x - 1, y, z) <= lightValue) {
      dePropagateSunlight(x - 1, y, z);
    } else
      propagateSunLight(x - 1, y, z);

  } else if (x == 0) {
    if (m_leftNeighbor &&
        m_leftNeighbor->m_cubes[m_width - 1][y][z].getId() == AIR &&
        m_leftNeighbor->m_cubes[m_width - 1][y][z].getSunLightValue() <
          lightValue &&
        m_leftNeighbor->highestSunLVFromNeighbors(m_width - 1, y, z) <=
          lightValue) {
      m_leftNeighbor->dePropagateSunlight(m_width - 1, y, z);
    } else
      m_leftNeighbor->propagateSunLight(m_width - 1, y, z);
  }

  // ########################################################################

  // Up
  if (y < m_height - 1) {
    if (m_cubes[x][y + 1][z].getId() == AIR &&
        m_cubes[x][y + 1][z].getSunLightValue() < lightValue &&
        highestSunLVFromNeighbors(x, y + 1, z) <= lightValue) {
      dePropagateSunlight(x, y + 1, z);
    } else
      propagateSunLight(x, y + 1, z);
  }

  // Down
  if (_lightValue == -1 && y > 0) {
    if (m_cubes[x][y - 1][z].getId() == AIR &&
        m_cubes[x][y - 1][z].getSunLightValue() < lightValue &&
        highestSunLVFromNeighbors(x, y - 1, z) <= lightValue) {
      dePropagateSunlight(x, y - 1, z);
    } else
      propagateSunLight(x, y - 1, z);
  }

  // ########################################################################

  // Backwards
  if (z < m_depth - 1) {
    if (m_cubes[x][y][z + 1].getId() == AIR &&
        m_cubes[x][y][z + 1].getSunLightValue() < lightValue &&
        highestSunLVFromNeighbors(x, y, z + 1) <= lightValue) {
      dePropagateSunlight(x, y, z + 1);
    } else
      propagateSunLight(x, y, z + 1);

  } else if (z == m_depth - 1) {

    if (m_backNeighbor && m_backNeighbor->m_cubes[x][y][0].getId() == AIR &&
        m_backNeighbor->m_cubes[x][y][0].getSunLightValue() < lightValue &&
        m_backNeighbor->highestSunLVFromNeighbors(x, y, 0) <= lightValue) {
      m_backNeighbor->dePropagateSunlight(x, y, 0);
    } else
      m_rightNeighbor->propagateSunLight(x, y, 0);
  }

  // Forwards
  if (z > 0) {
    if (m_cubes[x][y][z - 1].getId() == AIR &&
        m_cubes[x][y][z - 1].getSunLightValue() < lightValue &&
        highestSunLVFromNeighbors(x, y, z - 1) <= lightValue) {
      dePropagateSunlight(x, y, z - 1);
    } else
      propagateSunLight(x, y, z - 1);

  } else if (z == 0) {
    if (m_frontNeighbor &&
        m_frontNeighbor->m_cubes[x][y][m_depth - 1].getId() == AIR &&
        m_frontNeighbor->m_cubes[x][y][m_depth - 1].getSunLightValue() <
          lightValue &&
        m_frontNeighbor->highestSunLVFromNeighbors(x, y, m_depth - 1) <=
          lightValue) {
      m_frontNeighbor->dePropagateSunlight(x, y, m_depth - 1);
    } else
      m_frontNeighbor->propagateSunLight(x, y, m_depth - 1);
  }
}

void
Chunk::dePropagateOtherlight(int x, int y, int z /*, int _lightValue*/) {
  updateDirtyRegions(y);

  queue<vec3> depropagates;
  depropagates.emplace(x, y, z);
  vector<vec3> propagates;

  while (!depropagates.empty()) {

    vec3 current = depropagates.front();
    depropagates.pop();
    Voxel& voxel = m_cubes[current.x][current.y][current.z];
    char lightValue{voxel.getOtherLightValue()};
    voxel.setOtherLightValue(0);

    // Right
    if (current.x + 1 < m_width) {
      Voxel& v{m_cubes[current.x + 1][current.y][current.z]};
      if (v.getId() == AIR) {
        if (v.getOtherLightValue() != 0 &&
            v.getOtherLightValue() <= lightValue) {
          depropagates.emplace(current.x + 1, current.y, current.z);
        } else if (v.getOtherLightValue() >= lightValue) {
          propagates.push_back({current.x + 1, current.y, current.z});
        }
      }
    } else {
      if (m_rightNeighbor) {
        Voxel& v{m_rightNeighbor->m_cubes[0][current.y][current.z]};
        if (v.getId() == AIR) {
          if (v.getOtherLightValue() != 0 &&
              v.getOtherLightValue() <= lightValue) {
            m_rightNeighbor->dePropagateOtherlight(0, current.y, current.z);
          }
        }
      }
    }

    // Left
    if (current.x - 1 >= 0) {
      Voxel& v{m_cubes[current.x - 1][current.y][current.z]};
      if (v.getId() == AIR) {
        if (v.getOtherLightValue() != 0 &&
            v.getOtherLightValue() <= lightValue) {
          depropagates.emplace(current.x - 1, current.y, current.z);
        } else if (v.getOtherLightValue() >= lightValue) {
          propagates.push_back({current.x - 1, current.y, current.z});
        }
      }
    } else {
      if (m_leftNeighbor) {
        Voxel& v{m_leftNeighbor->m_cubes[m_width - 1][current.y][current.z]};
        if (v.getId() == AIR) {
          if (v.getOtherLightValue() != 0 &&
              v.getOtherLightValue() <= lightValue) {
            m_leftNeighbor->dePropagateOtherlight(
              m_width - 1, current.y, current.z);
          }
        }
      }
    }

    // Up
    if (current.y + 1 < m_height) {
      Voxel& v{m_cubes[current.x][current.y + 1][current.z]};
      if (v.getId() == AIR) {
        if (v.getOtherLightValue() != 0 &&
            v.getOtherLightValue() <= lightValue) {
          depropagates.emplace(current.x, current.y + 1, current.z);
        } else if (v.getOtherLightValue() >= lightValue) {
          propagates.push_back({current.x, current.y + 1, current.z});
        }
      }
    }

    // Down
    if (current.y - 1 >= 0) {
      Voxel& v{m_cubes[current.x][current.y - 1][current.z]};
      if (v.getId() == AIR) {
        if (v.getOtherLightValue() != 0 &&
            v.getOtherLightValue() <= lightValue) {
          depropagates.emplace(current.x, current.y - 1, current.z);
        } else if (v.getOtherLightValue() >= lightValue) {
          propagates.push_back({current.x, current.y - 1, current.z});
        }
      }
    }

    // Backwards
    if (current.z + 1 > 0) {
      Voxel& v{m_cubes[current.x][current.y][current.z + 1]};
      if (v.getId() == AIR) {
        if (v.getOtherLightValue() != 0 &&
            v.getOtherLightValue() <= lightValue) {
          depropagates.emplace(current.x, current.y, current.z + 1);
        } else if (v.getOtherLightValue() >= lightValue) {
          propagates.push_back({current.x, current.y, current.z + 1});
        }
      }
    } else {
      if (m_backNeighbor) {
        Voxel& v{m_cubes[current.x][current.y][0]};
        if (v.getId() == AIR) {
          if (v.getOtherLightValue() != 0 &&
              v.getOtherLightValue() <= lightValue) {
            m_backNeighbor->dePropagateOtherlight(current.x, current.y, 0);
          }
        }
      }
    }

    // Forward
    if (current.z - 1 >= 0) {
      Voxel& v{m_cubes[current.x][current.y][current.z - 1]};
      if (v.getId() == AIR) {
        if (v.getOtherLightValue() != 0 &&
            v.getOtherLightValue() <= lightValue) {
          depropagates.emplace(current.x, current.y, current.z - 1);
        } else if (v.getOtherLightValue() >= lightValue) {
          propagates.push_back({current.x, current.y, current.z - 1});
        }
      }
    } else {
      if (m_frontNeighbor) {
        Voxel& v{m_cubes[current.x][current.y][m_depth - 1]};
        if (v.getId() == AIR) {
          if (v.getOtherLightValue() != 0 &&
              v.getOtherLightValue() <= lightValue) {
            m_frontNeighbor->dePropagateOtherlight(
              current.x, current.y, m_depth - 1);
          }
        }
      }
    }
  }

  for (vec3& p : propagates)
    propagateOtherLight(p.x, p.y, p.z);
}

int
Chunk::highestSunLVFromNeighbors(int x, int y, int z) {
  int highestValue = -1;

  Voxel* v = getVoxel2(x + 1, y, z);
  if (v && v->getSunLightValue() > highestValue)
    highestValue = v->getSunLightValue();

  v = getVoxel2(x - 1, y, z);
  if (v && v->getSunLightValue() > highestValue)
    highestValue = v->getSunLightValue();

  v = getVoxel2(x, y + 1, z);
  if (v && v->getSunLightValue() > highestValue)
    highestValue = v->getSunLightValue();

  v = getVoxel2(x, y - 1, z);
  if (v && v->getSunLightValue() > highestValue)
    highestValue = v->getSunLightValue();

  v = getVoxel2(x, y, z + 1);
  if (v && v->getSunLightValue() > highestValue)
    highestValue = v->getSunLightValue();

  v = getVoxel2(x, y, z - 1);
  if (v && v->getSunLightValue() > highestValue)
    highestValue = v->getSunLightValue();

  return highestValue;
}

int
Chunk::highestOtherLVFromNeighbors(int x, int y, int z) {
  int highestValue = -1;

  Voxel* v = getVoxel2(x + 1, y, z);
  if (v && v->getOtherLightValue() > highestValue)
    highestValue = v->getOtherLightValue();

  v = getVoxel2(x - 1, y, z);
  if (v && v->getOtherLightValue() > highestValue)
    highestValue = v->getOtherLightValue();

  v = getVoxel2(x, y + 1, z);
  if (v && v->getOtherLightValue() > highestValue)
    highestValue = v->getOtherLightValue();

  v = getVoxel2(x, y - 1, z);
  if (v && v->getOtherLightValue() > highestValue)
    highestValue = v->getOtherLightValue();

  v = getVoxel2(x, y, z + 1);
  if (v && v->getOtherLightValue() > highestValue)
    highestValue = v->getOtherLightValue();

  v = getVoxel2(x, y, z - 1);
  if (v && v->getOtherLightValue() > highestValue)
    highestValue = v->getOtherLightValue();

  return highestValue;
}

bool
Chunk::isInDirectSunlight(int x, int y, int z) {
  if (y < m_height)
    return getVoxel(x, y + 1, z).getSunLightValue() == m_directSunlight;

  return false;
}
}
