
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

  defaultFill(cubes);

  if (options.getFlat()) {
    generateFlat(cubes);
  } else {
    generateNoneFlat(cubes, x, y);
  }

  return cubes;
}

void TerrainGenerator::defaultFill(VoxelMatrix &cubes) {
  for (int x{0}; x < m_width; ++x) {
    cubes.push_back(vector<vector<Voxel>>());
    for (int y{0}; y < m_height; ++y) {
      cubes[x].push_back(vector<Voxel>());
      for (int z{0}; z < m_depth; ++z) {
        if (y == 0)
          cubes[x][y].push_back(Voxel{config::cube_data::BED_ROCK, 0});
        else
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

void TerrainGenerator::generateNoneFlat(VoxelMatrix &cubes, int x, int z)
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

  module::Perlin baseSource;
  baseSource.SetOctaveCount(5);
  baseSource.SetFrequency(0.05);
  baseSource.SetPersistence(0.5);

  module::ScaleBias base;
  base.SetSourceModule(0, baseSource);
  base.SetScale(15);
  base.SetBias(25);

  module::Perlin biomeType;
  biomeType.SetFrequency(0.25);

  int xOffsetLocation{x};
  int zOffsetLocation{z};

  for (int x{0}; x < m_width; ++x) {
    for (int z{0}; z < m_depth; ++z) {

      double height = base.GetValue(
        (xOffsetLocation + static_cast<int>(x)) / 10.0,
        (zOffsetLocation + static_cast<int>(z)) / 10.0, 0.5);


      if (height - 2 < m_seaLevel) {
        for (int i{1}; i < m_seaLevel + 2; ++i) {
          auto &v = cubes[x][i][z];

          if (i <= m_seaLevel)
            v.id = config::cube_data::WATER;
          if (i < height)
            v.id = config::cube_data::DIRT;

          if (i >= m_seaLevel && i < height)
            v.id = config::cube_data::SAND;

        }
        continue;
      }

      double biome = biomeType.GetValue(
        (xOffsetLocation + static_cast<int>(x)) / 10.0,
        (zOffsetLocation + static_cast<int>(z)) / 10.0, 0.5);

//      double noiseValue = flat.GetValue(
//        (xOffsetLocation + static_cast<int>(x)) / 10.0,
//        (zOffsetLocation + static_cast<int>(z)) / 10.0, 0.5);

      if (biome > 0.5) {
        generateDessert(cubes, height, xOffsetLocation, zOffsetLocation, x, z);
      } else {
        generateGrassLand(cubes, height, xOffsetLocation, zOffsetLocation, x, z);
      }

    }
  }
}

void TerrainGenerator::generateGrassLand(VoxelMatrix &cubes, double height,
    int xOff, int zOff, int x, int z) {

  module::Perlin tree;
  tree.SetFrequency(2);

  for (int y{1}; y < height; ++y) {
    auto &v = cubes[x][y][z];
    v.id = config::cube_data::DIRT;

    if (y + 1 > height) {
        v.id = config::cube_data::GRASS;

      double value = tree.GetValue(
        (xOff + static_cast<int>(x)) / 10.0,
        (zOff + static_cast<int>(z)) / 10.0, 0.5);

      // TODO Only place a tree if there is no tree in close distance like
      // about 3 cubes.

      if (x > 0 && x + 1 < m_width  && z > 0 && z + 1 < m_depth &&
          value > -0.02 && value < 0.02)
        placeTree(cubes, x, y + 1, z);

      break;
    }
  }
}

void TerrainGenerator::generateDessert(VoxelMatrix &cubes, double height,
    int xOff, int zOff, int x, int z) {
  for (int y{1}; y < height; ++y) {
      auto &v = cubes[x][y][z];

      v.id = config::cube_data::SAND;
  }
}

void TerrainGenerator::placeTree(VoxelMatrix &cubes, int x, int y, int z) {
  for (int i{y}; i < y + 5; ++i)
    cubes[x][i][z].id = config::cube_data::LOG_BIRCH;

  cubes[x][y + 5][z].id = config::cube_data::LEAVES_BIRCH;
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

