
#ifndef SRC_MODEL_WORLD_TERRAINGEN_TERRAINGENERATOR_H_
#define SRC_MODEL_WORLD_TERRAINGEN_TERRAINGENERATOR_H_

#include <vector>

#include "../../../util/voxel.h"
#include "../chunk/creationOptions.h"

namespace terrainGen {

class TerrainGenerator {
public:

  std::vector<std::vector<std::vector<Voxel>>> generateTerrain(
      chunk::CreationOptions& options, int x, int y);

private:

  void generateFlat(std::vector<std::vector<std::vector<Voxel>>> &cubes);

  unsigned counterValue{};

  unsigned m_width{16};
  unsigned height{128};
  unsigned depth{16};

};

}

#endif /* SRC_MODEL_WORLD_TERRAINGEN_TERRAINGENERATOR_H_ */
