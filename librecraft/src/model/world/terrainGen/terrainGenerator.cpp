
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

TerrainGenerator::TerrainGenerator(int width, int height, int depth)
  : m_width{width}
  , m_height{height}
  , m_depth{depth}
{
}

TerrainGenerator::VoxelMatrix TerrainGenerator::generateTerrain(
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

  fillWithAir(cubes);

  if (options.getFlat()) {
    generateFlat(cubes);
  } else {
    generateNoneFlat(cubes, x, y);
  }

  return cubes;
}


void TerrainGenerator::fillWithAir(VoxelMatrix &cubes) {
  for (int x{0}; x < m_width; ++x) {
    cubes.push_back(vector<vector<Voxel>>());
    for (int y{0}; y < m_height; ++y) {
      cubes[x].push_back(vector<Voxel>());
      for (int z{0}; z < m_depth; ++z) {
        cubes[x][y].push_back(Voxel{config::cube_data::AIR, 0});
      }
    }
  }
}

void TerrainGenerator::generateFlat(VoxelMatrix &cubes) {
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
       }
     }
   }
}

void TerrainGenerator::generateNoneFlat(VoxelMatrix &cubes, int x, int y)
{
  module::RidgedMulti mountainSource;
  mountainSource.SetFrequency(0.05);
  mountainSource.SetOctaveCount(3);

  module::ScaleBias mountain;
  mountain.SetSourceModule(0, mountainSource);
  mountain.SetBias(40);
  mountain.SetScale(30);


  module::Billow flatSource;
  flatSource.SetFrequency(0.1);

  module::ScaleBias flat;
  flat.SetSourceModule(0, flatSource);
  flat.SetScale(3);
  flat.SetBias(20);

  module::Perlin biomeType;
  biomeType.SetFrequency(0.05);

  module::Perlin tree;
  biomeType.SetFrequency(0.025);

  int xOffsetLocation{x};
  int yOffsetLocation{y};

  for (int x{0}; x < m_width; ++x) {
    for (int z{0}; z < m_depth; ++z) {

    double noiseValue{};

    double value = biomeType.GetValue(
    (xOffsetLocation + static_cast<int>(x)) / 10.0,
    (yOffsetLocation + static_cast<int>(z)) / 10.0, 0.5);

    if (value > 0) {
      m_counterValue = config::cube_data::SAND;
    } else {
      m_counterValue = config::cube_data::DIRT;
    }

    noiseValue = flat.GetValue(
      (xOffsetLocation + static_cast<int>(x)) / 10.0,
      (yOffsetLocation + static_cast<int>(z)) / 10.0, 0.5);

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

//      if (y + 1 > noiseValue)
//        v.id = config::cube_data::GRASS;
      if(y > noiseValue && y < noiseValue + 5 && value < 0) {
        double value = tree.GetValue(
                (xOffsetLocation + static_cast<int>(x)) / 10.0,
                (yOffsetLocation + static_cast<int>(z)) / 10.0, 0.5);

        // TODO Only place a tree if there is no tree in close distance like
        // about 3 cubes.

        if (x > 0 && x + 1 < m_width  && z > 0 && z + 1 < m_depth && value > -0.02 && value < 0.02)
          placeTree(cubes, x, y, z);

        break;
      }
    }
    }
  }
}

void TerrainGenerator::placeTree(VoxelMatrix &cubes, int x, int y, int z) {
  for (int i{y}; i < y + 5; ++i)
    cubes[x][i][z].id = config::cube_data::LOG_BIRCH;

  cubes[x][y + 5][z + 1].id = config::cube_data::LEAVES_BIRCH;
  cubes[x][y + 5][z - 1].id = config::cube_data::LEAVES_BIRCH;

  cubes[x + 1][y + 5][z].id = config::cube_data::LEAVES_BIRCH;
  cubes[x + 1][y + 5][z + 1].id = config::cube_data::LEAVES_BIRCH;
  cubes[x + 1][y + 5][z - 1].id = config::cube_data::LEAVES_BIRCH;

  cubes[x - 1][y + 5][z].id = config::cube_data::LEAVES_BIRCH;
  cubes[x - 1][y + 5][z + 1].id = config::cube_data::LEAVES_BIRCH;
  cubes[x - 1][y + 5][z - 1].id = config::cube_data::LEAVES_BIRCH;
}

}

// Sea..

// Grassland
  // Sparse forest
  // Dense forest
  // Water like ponds, lakes, and rivers

// Dessert

// Frozen


//       noiseValue = mountain.GetValue(
//          (xOffsetLocation + static_cast<int>(x)) / 10.0,
//          (yOffsetLocation + static_cast<int>(z)) / 10.0, 0.5);

//       noiseValue = flat.GetValue(
//          (xOffsetLocation + static_cast<int>(x)) / 10.0,
//          (yOffsetLocation + static_cast<int>(z)) / 10.0, 0.5);


