
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
  TerrainGenerator(int width, int height, int depth);

  /**
   * @brief Creates the voxel data.
   *
   * @param options The options controlling how the terrain should be generated.
   * @param x The x coordinate of the chunk.
   * @param y The y coordinate of the chunk.
   * @return A matrix of voxel data.
   */
  VoxelMatrix generateTerrain(chunk::CreationOptions& options, int x, int y);

private:

  inline void fillWithAir(VoxelMatrix &cubes);

  inline void generateFlat(VoxelMatrix &cubes);

  inline void generateNoneFlat(VoxelMatrix &cubes, int x, int y);

  inline void placeTree(VoxelMatrix &cubes, int x, int y, int z);

  unsigned m_counterValue{};

  const int m_width{};
  const int m_height{};
  const int m_depth{};
};

}

#endif /* SRC_MODEL_WORLD_TERRAINGEN_TERRAINGENERATOR_H_ */
