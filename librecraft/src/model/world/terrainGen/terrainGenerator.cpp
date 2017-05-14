
#include "terrainGenerator.h"

#include <thread>
#include <mutex>
#include <iostream>

#include "noiseMixer.h"

#include "../../../config/data.h"
#include "libnoise/module/billow.h"
#include "libnoise/module/perlin.h"
#include "libnoise/module/ridgedmulti.h"
#include "libnoise/module/scalebias.h"
#include "libnoise/module/select.h"

using namespace std;
using namespace noise;

namespace terrainGen{

int counter{1};
const int maxCount{config::cube_data::LAST_CUBE_USED_FOR_BUILDING};
std::mutex s_mutex;

vector<vector<vector<Voxel>>> TerrainGenerator::generateTerrain(
    CreationOptions& options,  int x, int y) {

  {
    lock_guard<mutex> lock(s_mutex);
    if (options.differentCubesForEachChunk())
      ++counter;
    if (counter > maxCount)
      counter = 0;
    counterValue = counter;
  }

  vector<vector<vector<Voxel>>> cubes;

  // Create the voxels with default values
  for (unsigned x{0}; x < m_width; ++x) {
    cubes.push_back(vector<vector<Voxel>>());
    for (unsigned y{0}; y < height; ++y) {
      cubes[x].push_back(vector<Voxel>());
      for (unsigned z{0}; z < depth; ++z) {
        cubes[x][y].push_back(Voxel{config::cube_data::AIR, 0});
      }
    }
  }

  if (options.getFlat()) {
    generateFlat(cubes);
    return cubes;
  } else {

    module::RidgedMulti mountainSource;
    mountainSource.SetFrequency(0.05);
    mountainSource.SetOctaveCount(3);

    module::ScaleBias mountain;
    mountain.SetSourceModule(0, mountainSource);
    mountain.SetBias(40);
    mountain.SetScale(30);


    module::Billow flatSource;
    flatSource.SetFrequency(0.2);

    module::ScaleBias flat;
    flat.SetSourceModule(0, flatSource);
    flat.SetScale(3);
    flat.SetBias(20);

    module::Perlin terrainType;
    terrainType.SetFrequency(0.1);

    module::Select finalTerrain;
    finalTerrain.SetSourceModule(1, flat);
    finalTerrain.SetSourceModule(0, mountain);
    finalTerrain.SetControlModule(terrainType);
    finalTerrain.SetEdgeFalloff(1);

    int xOffsetLocation{x};
    int yOffsetLocation{y};

    for (unsigned x{0}; x < m_width; ++x) {
      for (unsigned z{0}; z < depth; ++z) {

        double noiseValue = finalTerrain.GetValue(
            (xOffsetLocation + static_cast<int>(x)) / 10.0,
            (yOffsetLocation + static_cast<int>(z)) / 10.0, 0.5);

        for (unsigned y{0}; y < height; ++y) {
          auto &v = cubes[x][y][z];
          if (y == 0) {
            v.id = config::cube_data::BED_ROCK;
            continue;
          }
          if (y > 5 && y < 10)
            v.id = config::cube_data::WATER;
          if (y < noiseValue)
            v.id = counterValue;
        }
      }
    }
  }

  return cubes;
}

void TerrainGenerator::generateFlat(std::vector<std::vector<std::vector<Voxel>>> &cubes) {
  for (unsigned x{0}; x < m_width; ++x) {
     for (unsigned z{0}; z < depth; ++z) {
       for (unsigned y{0}; y < height; ++y) {
         auto &v = cubes[x][y][z];
         if (y == 0) {
           v.id = config::cube_data::BED_ROCK;
           continue;
         }
         if (y < 30)
           v.id = counterValue;
         else
           v.id = config::cube_data::AIR; // should already be air?
       }
     }
   }
}

}

