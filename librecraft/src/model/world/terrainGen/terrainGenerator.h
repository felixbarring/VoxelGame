
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
 */
class TerrainGenerator {
public:

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
  std::vector<std::vector<std::vector<Voxel>>> generateTerrain(
      chunk::CreationOptions& options, int x, int y);

private:

  void generateFlat(std::vector<std::vector<std::vector<Voxel>>> &cubes);

  void generateNoneFlat(std::vector<std::vector<std::vector<Voxel>>> &cubes,
      int x, int y);

  unsigned m_counterValue{};

  int m_width{};
  int m_height{};
  int m_depth{};

};

}

#endif /* SRC_MODEL_WORLD_TERRAINGEN_TERRAINGENERATOR_H_ */
