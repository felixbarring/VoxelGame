
#ifndef SRC_MODEL_WORLD_TERRAINGEN_TERRAINGENERATOR_H_
#define SRC_MODEL_WORLD_TERRAINGEN_TERRAINGENERATOR_H_

#include <vector>

#include "../../../util/voxel.h"
#include "../chunk/creationOptions.h"

namespace terrainGen {

/**
 * @brief This class is responsible of creating a matrix of voxel data.
 *
 * The data will be made in a way that it mimics various terrain types that
 * exist in the real world in order to create an interesting world.
 *
 * This class is thread safe, the generateTerrain function can be called
 * concurrently.
 */
class TerrainGenerator {
public:
  using VoxelMatrix = std::vector<std::vector<std::vector<Voxel>>>;
  // TODO Elaborate this documentation.

  /**
   * @brief Create an instance of this class.
   *
   * @param width The width of the matrix generated by generateTerrain.
   * @param height The height of the matrix generated by generateTerrain.
   * @param depth The depth of the matrix generated by generateTerrain.
   */
  TerrainGenerator(int width, int height, int depth, int seed = 1337);

  /**
   * @brief Creates the voxel data.
   *
   * @param options The options controlling how the terrain should be generated.
   * @param x The x coordinate of the chunk.
   * @param z The z coordinate of the chunk.
   * @return A matrix of voxel data.
   */
  VoxelMatrix generateTerrain(chunk::CreationOptions& options, int x, int z);

private:
  inline void defaultFill(VoxelMatrix& cubes);

  inline void generateFlat(VoxelMatrix& cubes);

  inline void generateNoneFlat(VoxelMatrix& cubes, int x, int z);

  inline void generateGrassLand(VoxelMatrix& cubes,
                                double height,
                                int xOff,
                                int zOff,
                                int x,
                                int z);

  inline void generateDessert(VoxelMatrix& cubes,
                              double height,
                              // int xOff,
                              // int zOff,
                              int x,
                              int z);

  inline void placeTree(VoxelMatrix& cubes, int x, int y, int z);

  inline bool hasTreeNeighboor(VoxelMatrix& cubes, int x, int y, int z);

  unsigned m_counterValue{};

  const int m_width{};
  const int m_height{};
  const int m_depth{};

  const int m_seed{};

  const int m_seaLevel{20};
};
}

#endif /* SRC_MODEL_WORLD_TERRAINGEN_TERRAINGENERATOR_H_ */
