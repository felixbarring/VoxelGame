
#include "terrainGenerator.h"

#include <thread>
#include <mutex>

#include "noiseMixer.h"

#include "../../../config/data.h"

using namespace std;

namespace terrainGen{

int counter{1};
const int maxCount{config::cube_data::LAST_CUBE_USED_FOR_BUILDING};
std::mutex s_mutex;

vector<vector<vector<Voxel>>> TerrainGenerator::generateTerrain(
    CreationOptions& options,  int x, int y) {

  unsigned counterValue;
  {
    lock_guard<mutex> lock(s_mutex);
    if (options.differentCubesForEachChunk())
      ++counter;
    if (counter > maxCount)
      counter = 0;
    counterValue = counter;
  }

  vector<vector<vector<Voxel>>> cubes;

  // TODO Get these from input parameters!
  unsigned width{16};
  unsigned height{128};
  unsigned depth{16};

  // Create the voxels with default values
  for (unsigned x{0}; x < width; ++x) {
    cubes.push_back(vector<vector<Voxel>>());
    for (unsigned y{0}; y < height; ++y) {
      cubes[x].push_back(vector<Voxel>());
      for (unsigned z{0}; z < depth; ++z) {
        cubes[x][y].push_back(Voxel{config::cube_data::AIR, 0});
      }
    }
  }

  if (options.getFlat()) {
//    for (unsigned x{0}; x < width; ++x) {
//      for (unsigned z{0}; z < depth; ++z) {
//        for (unsigned y{0}; y < height; ++y) {
//          auto &v = cubes[x][y][z];
//          if (y == 0) {
//            v.id = config::cube_data::BED_ROCK;
//            continue;
//          }
//          if (y < 30)
//            v.id = counterValue;
//          else
//            v.id = config::cube_data::AIR; // should already be air?
//        }
//      }
//    }
  } else {
    // TODO There should be some kind of seed in order to generate different
    // worlds. Currently the same world will be generated each time.

    NoiseMixer mixer{};
    mixer.addNoise(100.f, 15.f);
    mixer.addNoise(50, 5);
    mixer.addNoise(15, 3);

    int xOffsetLocation{x};
    int yOffsetLocation{y};

    for (unsigned x{0}; x < width; ++x) {
      for (unsigned z{0}; z < depth; ++z) {
        unsigned noiseValue{
          mixer.computeNoise(xOffsetLocation + static_cast<int>(x),
              yOffsetLocation + static_cast<int>(z))};
        for (unsigned y{0}; y < height; ++y) {
          auto &v = cubes[x][y][z];
          if (y == 0) {
            v.id = config::cube_data::BED_ROCK;
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

  return cubes;
}

}

