#include "chunkManager.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <limits>
#include <math.h>
#include <thread>

#include "../../../util/globalResources.h"
#include "../../../util/soundPlayer.h"

using util::SoundPlayer;

using namespace std;
using namespace glm;

using namespace config::chunk_data;
using namespace config::cube_data;
using namespace util;
using namespace globalResources;

namespace chunk {

ChunkManager::ChunkManager(CreationOptions options,
                           util::SoundPlayer& soundPlayer,
                           graphics::GraphicsManager& graphicsManager)
  : m_options{move(options)}
  , m_soundPlayer{soundPlayer}
  , m_graphicsManager{graphicsManager} {
  m_worldName = m_options.getName();
  m_bussyMovingChunksMutex = make_unique<mutex>();

  for (int x{0}; x < m_lenghtAcrossMatrix; ++x) {
    m_chunks.push_back(vector<vector<std::shared_ptr<Chunk>>>());
    for (int y{0}; y < config::chunk_data::NUMBER_OF_CHUNKS_Y; ++y) {
      m_chunks[x].push_back(vector<std::shared_ptr<Chunk>>());
      for (int z{0}; z < m_lenghtAcrossMatrix; ++z) {
        m_chunks[x][y].push_back(std::shared_ptr<Chunk>());
      }
    }
  }
}

void
ChunkManager::createWorld() {
  const int lam = m_lenghtAcrossMatrix;
  vector<future<void>> chunkCreationFutures;

  // Create the Chunks
  for (int x{0}; x < lam; ++x) {
    for (int z{0}; z < lam; ++z) {
      shared_ptr<Chunk> chunk = make_shared<Chunk>(m_worldName,
                                                   x * CHUNK_WIDTH_AND_DEPTH,
                                                   z * CHUNK_WIDTH_AND_DEPTH,
                                                   m_graphicsManager);

      chunkCreationFutures.push_back(g_threadPool2.enqueue([chunk, this] {
        chunk->create(m_options);
        chunk->doSunLightning();
      }));
      m_chunks[x][0][z] = chunk;
    }
  }

  connectChunks();

  // Wait for all the chunks to be generated
  for_each(chunkCreationFutures.begin(),
           chunkCreationFutures.end(),
           [](future<void>& f) { f.get(); });

  //    vector<future<void>> updateGrapicsFutures;

  // TODO Should be done in parallel..
  for (int x{0}; x < lam; ++x) {
    for (int z{0}; z < lam; ++z) {
      m_chunks[x][0][z]->propagateLights();
      m_chunks[x][0][z]->updateGraphics();
    }
  }
}

void
ChunkManager::saveWorld() {
  const int lam = m_lenghtAcrossMatrix;
  for (int x = 0; x < lam; ++x) {
    for (int z = 0; z < lam; z++) {
      m_chunks[x][0][z]->storeChunk();
    }
  }
}

void
ChunkManager::clearWorld() {
  const int lam = m_lenghtAcrossMatrix;
  for (int x = 0; x < lam; x++) {
    for (int z = 0; z < lam; z++) {
      m_chunks[x][0][z]->removeAllNeighbors();
      m_chunks[x][0][z].reset();
    }
  }
}

void
ChunkManager::update() {
  // When/if AI is added like npc, this could be a good
  // place to update them
}

Voxel
ChunkManager::getVoxel(int x, int y, int z) {
  // Used to avoid Division every time the function is called.
  static float xD = 1.0 / CHUNK_WIDTH_AND_DEPTH;
  static float yD = 1.0 / CHUNK_HEIGHT;
  static float zD = 1.0 / CHUNK_WIDTH_AND_DEPTH;

  x += m_xOffset;
  z += m_zOffset;

  int chunkX = x * xD;
  int chunkY = y * yD;
  int chunkZ = z * zD;

  int localX = x % CHUNK_WIDTH_AND_DEPTH;
  int localY = y % CHUNK_HEIGHT;
  int localZ = z % CHUNK_WIDTH_AND_DEPTH;

  // This protects from out of bounds access.
  // This obviously comes with a performance penalty.
  // Maybe remove this if there is a way to guarantee no out of range input.
  if (chunkX < 0 || chunkX >= static_cast<int>(m_chunks.size()) || chunkY < 0 ||
      chunkY >= static_cast<int>(m_chunks[0].size()) || chunkZ < 0 ||
      chunkZ >= static_cast<int>(m_chunks[0][0].size()) || localX < 0 ||
      localX >= CHUNK_WIDTH_AND_DEPTH || localY < 0 || localY >= CHUNK_HEIGHT ||
      localZ < 0 || localZ >= CHUNK_WIDTH_AND_DEPTH)
    return {};

  return m_chunks[chunkX][chunkY][chunkZ]->getVoxel(localX, localY, localZ);
}

char
ChunkManager::getCubeId(int x, int y, int z) {
  return getVoxel(x, y, z).getId();
}

bool
ChunkManager::isSolid(int x, int y, int z) {
  // Will not be correct if more cubes that are not solid are added.
  return !isAirOrWater(x, y, z);
}

bool
ChunkManager::isAirOrWater(int x, int y, int z) {
  char cubeId = getCubeId(x, y, z);
  return cubeId == AIR || cubeId == WATER;
}

void
ChunkManager::removeCube(int x, int y, int z) {
  char voxel = getCubeId(x, y, z);
  if (voxel != BED_ROCK && voxel != WATER) {
    if (hasWaterNeighbour(x, y, z))
      setCube(x, y, z, WATER);
    else
      setCube(x, y, z, AIR);

    m_soundPlayer.playSound(config::audio::cubeRemoved);
  }
}

void
ChunkManager::setCube(int x, int y, int z, char id) {
  static float xD = 1.0 / CHUNK_WIDTH_AND_DEPTH;
  static float yD = 1.0 / CHUNK_HEIGHT;
  static float zD = 1.0 / CHUNK_WIDTH_AND_DEPTH;

  x += m_xOffset;
  z += m_zOffset;

  int chunkX = x * xD;
  int chunkY = y * yD;
  int chunkZ = z * zD;

  int localX = x % CHUNK_WIDTH_AND_DEPTH;
  int localY = y % CHUNK_HEIGHT;
  int localZ = z % CHUNK_WIDTH_AND_DEPTH;

  m_chunks[chunkX][chunkY][chunkZ]->setCube(localX, localY, localZ, id);
  m_soundPlayer.playSound(config::audio::cubeAdded);
}

void
ChunkManager::setCenter(float x, float z) {
  if (!m_loadStoreWorldWhenPlyayerIsNotInTheCenterChunk)
    return;

  if (!m_bussyMovingChunksMutex->try_lock())
    return;

  // TODO Remove the hardcoded 16 values!!!

  if (x < NUMBER_OF_CHUNKS_FROM_MIDDLE_TO_BORDER * 16 - m_xOffset)
    moveChunksRight();
  else if (x > NUMBER_OF_CHUNKS_FROM_MIDDLE_TO_BORDER * 16 - m_xOffset + 16)
    moveChunksLeft();
  else if (z < NUMBER_OF_CHUNKS_FROM_MIDDLE_TO_BORDER * 16 - m_zOffset)
    moveChunksUp();
  else if (z > NUMBER_OF_CHUNKS_FROM_MIDDLE_TO_BORDER * 16 - m_zOffset + 16)
    moveChunksDown();
  else
    m_bussyMovingChunksMutex->unlock();
}

bool
ChunkManager::intersectWithSolidCube(vec3 origin,
                                     vec3 direction,
                                     vec3& intersected,
                                     vec3& previous,
                                     float searchLength) {
  static const double smallNumber = 0.0001;

  // When the player is exactly located at an integer position the selection
  // will be wrong due to incorrect rounding of the values. The additions with
  // a small number solves this problem. Not bulletproof but seems to work well
  // in practice.
  origin = origin + glm::vec3{smallNumber, smallNumber, smallNumber};

  // Get the sign of the directions
  int signXDirection = (direction.x > 0) - (direction.x < 0);
  int signYDirection = (direction.y > 0) - (direction.y < 0);
  int signZDirection = (direction.z > 0) - (direction.z < 0);

  // Check if any one is zero
  float xL;
  float yL;
  float zL;

  if (direction.x == 0)
    xL = std::numeric_limits<float>::infinity();
  else
    xL = 1.0 / direction.x;

  if (direction.y == 0)
    yL = std::numeric_limits<float>::infinity();
  else
    yL = 1.0 / direction.y;

  if (direction.z == 0)
    zL = std::numeric_limits<float>::infinity();
  else
    zL = 1.0 / direction.z;

  int currentCubeX = floor(origin.x);
  int currentCubeY = floor(origin.y);
  int currentCubeZ = floor(origin.z);

  // Problem here :s
  // When the player is located exactly at an integer value ie 7.0 we get errors
  int xAxis = signXDirection > 0 ? ceil(origin.x) : floor(origin.x);
  int yAxis = signYDirection > 0 ? ceil(origin.y) : floor(origin.y);
  int zAxis = signZDirection > 0 ? ceil(origin.z) : floor(origin.z);

  float distanceSearched = 0;

  previous = vec3(currentCubeX, currentCubeY, currentCubeZ);

  while (distanceSearched < searchLength) {

    previous = vec3(currentCubeX, currentCubeY, currentCubeZ);

    // Multi means how much along an axis we need to move to pass into the next
    // cube. Step into the next cube by going one step in the direction of the
    // axis with the lowest multi.
    float multiX = (xAxis - origin.x) * xL;
    float multiY = (yAxis - origin.y) * yL;
    float multiZ = (zAxis - origin.z) * zL;

    if (multiX < multiY && multiX < multiZ) {

      currentCubeX += signXDirection;
      xAxis += signXDirection;
      distanceSearched = length(multiX * direction);

    } else if (multiY < multiZ) {

      currentCubeY += signYDirection;
      yAxis += signYDirection;
      distanceSearched = length(multiY * direction);

    } else {

      currentCubeZ += signZDirection;
      zAxis += signZDirection;
      distanceSearched = length(multiZ * direction);
    }

    if (!isAirOrWater(currentCubeX, currentCubeY, currentCubeZ)) {
      intersected = vec3(currentCubeX, currentCubeY, currentCubeZ);
      return true;
    }
  }

  return false;
}

void
ChunkManager::loadWorldWhenDecentered(bool value) {
  m_loadStoreWorldWhenPlyayerIsNotInTheCenterChunk = value;
}

// Private Methods #############################################################

bool
ChunkManager::hasWaterNeighbour(int x, int y, int z) {
  return getCubeId(x + 1, y, z) == WATER || getCubeId(x - 1, y, z) == WATER ||
         getCubeId(x, y, z + 1) == WATER || getCubeId(x, y, z - 1) == WATER ||
         getCubeId(x, y + 1, z) == WATER;
}

void
ChunkManager::connectChunks() {
  const int lam = m_lenghtAcrossMatrix;

  for (int x = 0; x < lam; ++x) {
    for (int z = 0; z < lam; ++z) {
      std::shared_ptr<Chunk> current{m_chunks[x][0][z]};
      if (x != lam - 1) {
        std::shared_ptr<Chunk> right{m_chunks[x + 1][0][z]};
        current->setRightNeighbor(right);
        right->setLeftNeighbor(current);
      }
      if (z != lam - 1) {
        std::shared_ptr<Chunk> back{m_chunks[x][0][z + 1]};
        current->setBackNeighbor(back);
        back->setFrontNeighbor(current);
      }
    }
  }
}

void
ChunkManager::moveChunksRight() {
  m_xOffset += CHUNK_WIDTH_AND_DEPTH;
  moveChunks(Direction::Right);
}

void
ChunkManager::moveChunksLeft() {
  m_xOffset -= CHUNK_WIDTH_AND_DEPTH;
  moveChunks(Direction::Left);
}

void
ChunkManager::moveChunksUp() {
  m_zOffset += CHUNK_WIDTH_AND_DEPTH;
  moveChunks(Direction::Up);
}

void
ChunkManager::moveChunksDown() {
  m_zOffset -= CHUNK_WIDTH_AND_DEPTH;
  moveChunks(Direction::Down);
}

// TODO The thread safety needs to be improved and documented.
void
ChunkManager::moveChunks(Direction direction) {
  vector<shared_ptr<Chunk>> chunksToDelete;

  // Store the chunks that should be removed and moves the old chunks in the
  // matrix.
  if (direction == Direction::Right) {
    for (int i = 0; i < m_lenghtAcrossMatrix; ++i)
      chunksToDelete.push_back(m_chunks[m_lenghtAcrossMatrix - 1][0][i]);

    for (int i = m_lenghtAcrossMatrix - 1; i > 0; --i) {
      for (int j = 0; j < m_lenghtAcrossMatrix; ++j)
        m_chunks[i][0][j] = m_chunks[i - 1][0][j];
    }
  } else if (direction == Direction::Left) {
    for (int i = 0; i < m_lenghtAcrossMatrix; ++i)
      chunksToDelete.push_back(m_chunks[0][0][i]);

    for (int i = 0; i < m_lenghtAcrossMatrix - 1; ++i) {
      for (int j = 0; j < m_lenghtAcrossMatrix; ++j)
        m_chunks[i][0][j] = m_chunks[i + 1][0][j];
    }
  } else if (direction == Direction::Up) {
    for (int i = 0; i < m_lenghtAcrossMatrix; ++i)
      chunksToDelete.push_back(m_chunks[i][0][m_lenghtAcrossMatrix - 1]);

    for (int i = 0; i < m_lenghtAcrossMatrix; ++i) {
      for (int j = m_lenghtAcrossMatrix - 1; j > 0; --j)
        m_chunks[i][0][j] = m_chunks[i][0][j - 1];
    }
  } else if (direction == Direction::Down) {
    for (int i = 0; i < m_lenghtAcrossMatrix; ++i)
      chunksToDelete.push_back(m_chunks[i][0][0]);

    for (int i = 0; i < m_lenghtAcrossMatrix; ++i) {
      for (int j = 0; j < m_lenghtAcrossMatrix - 1; ++j)
        m_chunks[i][0][j] = m_chunks[i][0][j + 1];
    }
  }

  // ##########################################################################
  // This used to be inside the function run by the thread pool!!!
  // ##########################################################################

  // Destroy the chunks that are outside the matrix.
  for (std::shared_ptr<Chunk>& chunk : chunksToDelete) {
    chunk->removeAllNeighbors();
    chunk->storeChunk();
    chunk.reset();
  }

  // Add the chunks, they are not yet fully generated.
  vector<shared_ptr<Chunk>> newChunks{};
  if (direction == Direction::Right) {
    for (int i = 0; i < m_lenghtAcrossMatrix; ++i) {
      std::shared_ptr<Chunk> ch{m_chunks[0 + 1][0][i]};
      std::shared_ptr<Chunk> chunk =
        std::make_shared<Chunk>(m_worldName,
                                ch->getXLocation() - CHUNK_WIDTH_AND_DEPTH,
                                ch->getZLocation(),
                                m_graphicsManager);
      m_chunks[0][0][i] = chunk;
      newChunks.push_back(chunk);
    }
  } else if (direction == Direction::Left) {
    for (int i = 0; i < m_lenghtAcrossMatrix; ++i) {
      std::shared_ptr<Chunk> ch{m_chunks[m_lenghtAcrossMatrix - 2][0][i]};
      std::shared_ptr<Chunk> chunk{
        std::make_shared<Chunk>(m_worldName,
                                ch->getXLocation() + CHUNK_WIDTH_AND_DEPTH,
                                ch->getZLocation(),
                                m_graphicsManager)};
      m_chunks[m_lenghtAcrossMatrix - 1][0][i] = chunk;
      newChunks.push_back(chunk);
    }
  } else if (direction == Direction::Up) {
    for (int i = 0; i < m_lenghtAcrossMatrix; ++i) {
      std::shared_ptr<Chunk> ch{m_chunks[i][0][0 + 1]};
      std::shared_ptr<Chunk> chunk{
        std::make_shared<Chunk>(m_worldName,
                                ch->getXLocation(),
                                ch->getZLocation() - CHUNK_WIDTH_AND_DEPTH,
                                m_graphicsManager)};
      m_chunks[i][0][0] = chunk;
      newChunks.push_back(chunk);
    }
  } else if (direction == Direction::Down) {
    for (int i = 0; i < m_lenghtAcrossMatrix; ++i) {
      std::shared_ptr<Chunk> ch{m_chunks[i][0][m_lenghtAcrossMatrix - 2]};
      std::shared_ptr<Chunk> chunk{
        std::make_shared<Chunk>(m_worldName,
                                ch->getXLocation(),
                                ch->getZLocation() + CHUNK_WIDTH_AND_DEPTH,
                                m_graphicsManager)};
      m_chunks[i][0][m_lenghtAcrossMatrix - 1] = chunk;
      newChunks.push_back(chunk);
    }
  }

  // TODO This could be optimized to only do the work for the new chunks and
  // their neighbors. Currently does the work for chunks that are already
  // connected.
  connectChunks();

  // ##########################################################################
  // ##########################################################################

  // Off load the work on a thread pool so that the game is not blocked during
  // this lengthy task.
  g_threadPool.enqueue([this, direction, newChunks] {

    // Run the creation and sunlightning work in parallel on the thread pool.
    vector<future<void>> chunkCreationFutures{};
    for (std::shared_ptr<Chunk> chunk : newChunks) {
      chunkCreationFutures.push_back(g_threadPool2.enqueue([chunk, this] {
        chunk->create(m_options);
        chunk->doSunLightning();
      }));
    }
    for_each(chunkCreationFutures.begin(),
             chunkCreationFutures.end(),
             [](future<void>& f) { f.get(); });

    vector<future<void>> collectLightFutures{};

    // Collect lights from neighbors
    if (direction == Direction::Right) {
      for (std::shared_ptr<Chunk> chunk : newChunks)
        collectLightFutures.push_back(g_threadPool2.enqueue(
          [chunk] { chunk->collectLightFromRightNeighbor(); }));
    } else if (direction == Direction::Left) {
      for (std::shared_ptr<Chunk> chunk : newChunks)
        collectLightFutures.push_back(g_threadPool2.enqueue(
          [chunk] { chunk->collectLightFromLeftNeighbor(); }));
    } else if (direction == Direction::Up) {
      for (std::shared_ptr<Chunk> chunk : newChunks)
        collectLightFutures.push_back(g_threadPool2.enqueue(
          [chunk] { chunk->collectLightFromBackNeighbor(); }));
    } else if (direction == Direction::Down) {
      for (std::shared_ptr<Chunk> chunk : newChunks)
        collectLightFutures.push_back(g_threadPool2.enqueue(
          [chunk] { chunk->collectLightFromFrontNeighbor(); }));
    }
    for_each(collectLightFutures.begin(),
             collectLightFutures.end(),
             [](future<void>& f) { f.get(); });

    auto f = [&](int start) {
      vector<future<void>> updateGrapicsFutures;
      for (int i{start}; i < static_cast<int>(newChunks.size()); i += 2) {
        std::shared_ptr<Chunk> chunk = newChunks[i];
        updateGrapicsFutures.push_back(
          g_threadPool2.enqueue([chunk, direction] {
            chunk->propagateLights();
            chunk->updateGraphics();
            if (direction == Direction::Right)
              chunk->getRightNeighbor()->forceUpdateGraphics();
            if (direction == Direction::Left)
              chunk->getLeftNeighbor()->forceUpdateGraphics();
            if (direction == Direction::Up)
              chunk->getBackNeighbor()->forceUpdateGraphics();
            if (direction == Direction::Down)
              chunk->getFrontNeighbor()->forceUpdateGraphics();
          }));
      }
      for_each(updateGrapicsFutures.begin(),
               updateGrapicsFutures.end(),
               [](future<void>& f) { f.get(); });
      updateGrapicsFutures.clear();
    };

    f(0);
    f(1);

    m_bussyMovingChunksMutex->unlock();
  });
}
}
