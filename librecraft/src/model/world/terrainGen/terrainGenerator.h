
#ifndef SRC_MODEL_WORLD_TERRAINGEN_TERRAINGENERATOR_H_
#define SRC_MODEL_WORLD_TERRAINGEN_TERRAINGENERATOR_H_

#include <vector>

#include "../../../util/voxel.h"
#include "../chunk/creationOptions.h"

using chunk::CreationOptions;

namespace terrainGen {

class TerrainGenerator {
public:

  std::vector<std::vector<std::vector<Voxel>>> generateTerrain(
      CreationOptions& options, int x, int y);

};

}

#endif /* SRC_MODEL_WORLD_TERRAINGEN_TERRAINGENERATOR_H_ */
