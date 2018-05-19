#ifndef SRC_MODEL_WORLD_CHUNK_CHUNKMANAGER_H_
#define SRC_MODEL_WORLD_CHUNK_CHUNKMANAGER_H_

#include <ThreadPool.h>
#include <glm/detail/type_vec.hpp>
#include <memory>
#include <vector>

#include "config/data.h"
#include "util/soundPlayer.h"
#include "util/voxel.h"

#include "model/world/entity/aabb.h"

#include "chunk.h"
#include "creationOptions.h"

namespace chunk {

class ChunkManager {
public:
  ChunkManager(CreationOptions options,
               util::SoundPlayer& soundPlayer,
               graphics::GraphicsManager& graphicsManager);

  void createWorld();

  void saveWorld() const;

  void clearWorld();

  /**
   * Should be called each frame.
   */
  void update();

  Voxel getVoxel(int x, int y, int z) const;

  Voxel getVoxel(glm::vec3 location) const;

  char getCubeId(int x, int y, int z) const;

  char getCubeId(glm::vec3 location) const;

  bool isSolid(int x, int y, int z) const;

  bool isSolid(glm::vec3 location) const;

  bool isAirOrWater(int x, int y, int z) const;

  bool isAirOrWater(glm::vec3 location) const;

  bool removeCube(int x, int y, int z);

  bool removeCube(glm::vec3 location);

  bool setCube(int x, int y, int z, char id);

  bool setCube(glm::vec3, char id);

  /**
   * Sets the center chunk
   * If the new center is different from the previous,
   * chunks that are no longer in the distance bounds from
   * the center chunk will be unloaded and chunks that entered the
   * bounds will be loaded.
   *
   * @param x The x coordinate that will be used to determine the
   *          center chunks x location
   * @param z The z coordinate that will be used to determine the
   *          center chunks z location
   */
  void setCenter(float x, float z);

  bool intersectWithSolidCube(glm::vec3 origin,
                              glm::vec3 direction,
                              glm::vec3& intersected,
                              glm::vec3& previous,
                              float searchLength) const;

  void loadWorldWhenDecentered(bool value = true);

  const entity::AABB& getLimit() const;

private:
  entity::AABB createLimit();

  /*
   * Checks if there is water above or on the sides, not bellow.
   */
  bool hasWaterNeighbour(int x, int y, int z);

  void connectChunks();

  void moveChunksRight();

  void moveChunksLeft();

  void moveChunksUp();

  void moveChunksDown();

  enum class Direction { Right, Left, Up, Down };

  void moveChunks(Direction direction);

  int m_lenghtAcrossMatrix{
    config::chunk_data::NUMBER_OF_CHUNKS_FROM_MIDDLE_TO_BORDER * 2 + 1};

  std::vector<std::vector<std::vector<std::shared_ptr<Chunk>>>> m_chunks;

  int m_xOffset{0};
  int m_zOffset{0};

  std::string m_worldName{};
  CreationOptions m_options;
  util::SoundPlayer& m_soundPlayer;
  graphics::GraphicsManager& m_graphicsManager;

  bool m_loadChunks{true};

  glm::vec3 m_center{4, 0, 4};
  glm::vec3 m_intersectedCube{};
  glm::vec3 m_previousCube{};

  entity::AABB m_limit;

  // Mutex is non movable, hence the unique_ptr
  std::unique_ptr<std::mutex> m_bussyMovingChunksMutex{};
};

} /* namespace chunk */

#endif /* SRC_MODEL_WORLD_CHUNK_CHUNKMANAGER_H_ */
