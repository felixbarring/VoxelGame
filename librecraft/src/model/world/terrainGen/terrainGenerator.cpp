
#include "terrainGenerator.h"

#include <thread>
#include <mutex>
#include <iostream>

#include "../../../config/data.h"
#include "../../../dependencies/libnoise/module/billow.h"
#include "../../../dependencies/libnoise/module/perlin.h"
#include "../../../dependencies/libnoise/module/ridgedmulti.h"
#include "../../../dependencies/libnoise/module/scalebias.h"
#include "../../../dependencies/libnoise/module/select.h"

using namespace std;
using namespace noise;

using chunk::CreationOptions;

namespace terrainGen{

int counter{1};
const int maxCount{config::cube_data::LAST_CUBE_USED_FOR_GENERATION};
mutex s_mutex;

TerrainGenerator::TerrainGenerator(int width, int height,
    int depth)
  : m_width{width}
  , m_height{height}
  , m_depth{depth}
{
}

vector<vector<vector<Voxel>>> TerrainGenerator::generateTerrain(
    CreationOptions& options,  int x, int y)
{
  {
    lock_guard<mutex> lock(s_mutex);
    if (options.differentCubesForEachChunk())
      ++counter;
    if (counter > maxCount)
      counter = 0;
    m_counterValue = counter;
  }

  vector<vector<vector<Voxel>>> cubes;

  // Create the voxels with default values
  for (int x{0}; x < m_width; ++x) {
    cubes.push_back(vector<vector<Voxel>>());
    for (int y{0}; y < m_height; ++y) {
      cubes[x].push_back(vector<Voxel>());
      for (int z{0}; z < m_depth; ++z) {
        cubes[x][y].push_back(Voxel{config::cube_data::AIR, 0});
      }
    }
  }

  if (options.getFlat()) {
    generateFlat(cubes);
  } else {
    generateNoneFlat(cubes, x, y);
  }

  return cubes;
}

void TerrainGenerator::generateFlat(vector<vector<vector<Voxel>>> &cubes) {
  for (int x{0}; x < m_width; ++x) {
    for (int z{0}; z < m_depth; ++z) {
       for (int y{0}; y < m_height; ++y) {
         auto &v = cubes[x][y][z];
         if (y == 0) {
           v.id = config::cube_data::BED_ROCK;
           continue;
         }
         if (y < 30)
           v.id = m_counterValue;
         else
           v.id = config::cube_data::AIR; // should already be air?
       }
     }
   }
}

void TerrainGenerator::generateNoneFlat(vector<vector<vector<Voxel>>> &cubes,
    int x, int y)
{
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
  terrainType.SetFrequency(0.05);

  int xOffsetLocation{x};
  int yOffsetLocation{y};

  for (int x{0}; x < m_width; ++x) {
   for (int z{0}; z < m_depth; ++z) {

     double noiseValue{};

//     if (terrainType.GetValue(
//         (xOffsetLocation + static_cast<int>(x)) / 10.0,
//         (yOffsetLocation + static_cast<int>(z)) / 10.0, 0.5)
//         > 0.6)
//     {
       noiseValue = mountain.GetValue(
          (xOffsetLocation + static_cast<int>(x)) / 10.0,
          (yOffsetLocation + static_cast<int>(z)) / 10.0, 0.5);
//     } else {
//       noiseValue = flat.GetValue(
//          (xOffsetLocation + static_cast<int>(x)) / 10.0,
//          (yOffsetLocation + static_cast<int>(z)) / 10.0, 0.5);
//     }

     for (int y{0}; y < m_height; ++y) {
       auto &v = cubes[x][y][z];
       if (y == 0) {
         v.id = config::cube_data::BED_ROCK;
         continue;
       }
       if (y > 5 && y < 10)
         v.id = config::cube_data::WATER;
       if (y < noiseValue)
         v.id = m_counterValue;
     }
   }
  }
}

}

