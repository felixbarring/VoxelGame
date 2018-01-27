
#include "terrainGenerator.h"

#include <iostream>
#include <mutex>
#include <thread>

#include "../../../config/data.h"
#include "../../../dependencies/libnoise/module/billow.h"
#include "../../../dependencies/libnoise/module/perlin.h"
#include "../../../dependencies/libnoise/module/ridgedmulti.h"
#include "../../../dependencies/libnoise/module/scalebias.h"
#include "../../../dependencies/libnoise/module/select.h"

using namespace std;
using namespace noise;

using chunk::CreationOptions;

namespace terrainGen {

int counter{1};
const int maxCount{config::cube_data::LAST_CUBE_USED_FOR_GENERATION};
mutex s_mutex;

// TODO Use the seed for something. :p
TerrainGenerator::TerrainGenerator(int width, int height, int depth, int seed)
  : m_width{width}
  , m_height{height}
  , m_depth{depth}
  , m_seed{seed} {

  int kek = m_seed; // Dummy to prevent unused warnings :-)
  ++kek;
}

TerrainGenerator::VoxelMatrix
TerrainGenerator::generateTerrain(CreationOptions& options, int x, int y) {
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

void
TerrainGenerator::defaultFill(VoxelMatrix& cubes) {
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

void
TerrainGenerator::generateFlat(VoxelMatrix& cubes) {
  for (int x{0}; x < m_width; ++x) {
    for (int z{0}; z < m_depth; ++z) {
      for (int y{0}; y < m_height; ++y) {
        Voxel& v = cubes[x][y][z];
        if (y == 0) {
          v.setId(config::cube_data::BED_ROCK);
          continue;
        }
        if (y < 30)
          v.setId(m_counterValue);
      }
    }
  }
}

void
TerrainGenerator::generateNoneFlat(VoxelMatrix& cubes, int x, int z) {
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
  biomeType.SetFrequency(0.01);

  int xOffsetLocation{x};
  int zOffsetLocation{z};

  for (int x{0}; x < m_width; ++x) {
    for (int z{0}; z < m_depth; ++z) {

      double height =
        base.GetValue((xOffsetLocation + static_cast<int>(x)) / 10.0,
                      (zOffsetLocation + static_cast<int>(z)) / 10.0,
                      0.5);

      if (height - 2 < m_seaLevel) {
        for (int i{1}; i < m_seaLevel + 2; ++i) {
          Voxel& v = cubes[x][i][z];

          if (i <= m_seaLevel)
            v.setId(config::cube_data::WATER);
          if (i < height)
            v.setId(config::cube_data::DIRT);

          if (i >= m_seaLevel && i < height)
            v.setId(config::cube_data::SAND);
        }
        continue;
      }

      double biome =
        biomeType.GetValue((xOffsetLocation + static_cast<int>(x)) / 10.0,
                           (zOffsetLocation + static_cast<int>(z)) / 10.0,
                           0.5);

      // TODO Add this to biomes...
      //      double noiseValue = flat.GetValue(
      //        (xOffsetLocation + static_cast<int>(x)) / 10.0,
      //        (zOffsetLocation + static_cast<int>(z)) / 10.0, 0.5);

      if (biome > 0.8) {
        generateDessert(
          cubes, height /*, xOffsetLocation, zOffsetLocation*/, x, z);
      } else {
        generateGrassLand(
          cubes, height, xOffsetLocation, zOffsetLocation, x, z);
      }
    }
  }
}

void
TerrainGenerator::generateGrassLand(VoxelMatrix& cubes,
                                    double height,
                                    int xOff,
                                    int zOff,
                                    int x,
                                    int z) {

  module::Perlin tree;
  tree.SetFrequency(2);

  module::Perlin treeArea;
  treeArea.SetFrequency(0.25);

  for (int y{1}; y < height; ++y) {
    Voxel& v = cubes[x][y][z];

    static const double stonePercentage{1.0 / 2.0};
    if (y < height * stonePercentage)
      v.setId(config::cube_data::STONE);
    else
      v.setId(config::cube_data::DIRT);

    if (y + 1 > height) {
      v.setId(config::cube_data::GRASS);

      double shouldHaveTrees =
        treeArea.GetValue((xOff + static_cast<int>(x)) / 10.0,
                          (zOff + static_cast<int>(z)) / 10.0,
                          0.5);

      if (shouldHaveTrees < 0.5)
        break;

      double value = tree.GetValue((xOff + static_cast<int>(x)) / 10.0,
                                   (zOff + static_cast<int>(z)) / 10.0,
                                   0.5);

      bool isNotAtBorder{x > 0 && x + 1 < m_width && z > 0 && z + 1 < m_depth};
      bool shouldPlaceTree{value > -0.02 && value < 0.02};
      // Short circuit is important here.
      // Does not seem to work properly :s
      bool hasNeighboor{isNotAtBorder && hasTreeNeighboor(cubes, x, y + 1, z)};

      if (isNotAtBorder && shouldPlaceTree && !hasNeighboor)
        placeTree(cubes, x, y + 1, z);

      break;
    }
  }
}

void
TerrainGenerator::generateDessert(VoxelMatrix& cubes,
                                  double height,
                                  // int xOff,
                                  // int zOff,
                                  int x,
                                  int z) {
  for (int y{1}; y < height; ++y) {
    Voxel& v = cubes[x][y][z];

    v.setId(config::cube_data::SAND);
  }
}

void
TerrainGenerator::placeTree(VoxelMatrix& cubes, int x, int y, int z) {
  for (int i{y}; i < y + 5; ++i)
    cubes[x][i][z].setId(config::cube_data::LOG_BIRCH);

  cubes[x][y + 5][z].setId(config::cube_data::LEAVES_BIRCH);
  cubes[x][y + 5][z + 1].setId(config::cube_data::LEAVES_BIRCH);
  cubes[x][y + 5][z - 1].setId(config::cube_data::LEAVES_BIRCH);

  cubes[x + 1][y + 5][z].setId(config::cube_data::LEAVES_BIRCH);
  cubes[x + 1][y + 5][z + 1].setId(config::cube_data::LEAVES_BIRCH);
  cubes[x + 1][y + 5][z - 1].setId(config::cube_data::LEAVES_BIRCH);

  cubes[x - 1][y + 5][z].setId(config::cube_data::LEAVES_BIRCH);
  cubes[x - 1][y + 5][z + 1].setId(config::cube_data::LEAVES_BIRCH);
  cubes[x - 1][y + 5][z - 1].setId(config::cube_data::LEAVES_BIRCH);
}

bool
TerrainGenerator::hasTreeNeighboor(VoxelMatrix& cubes, int x, int y, int z) {
  return cubes[x][y][z + 1].getId() == config::cube_data::LOG_BIRCH ||
         cubes[x][y][z - 1].getId() == config::cube_data::LOG_BIRCH ||

         cubes[x + 1][y][z + 1].getId() == config::cube_data::LOG_BIRCH ||
         cubes[x + 1][y][z - 1].getId() == config::cube_data::LOG_BIRCH ||

         cubes[x - 1][y][z + 1].getId() == config::cube_data::LOG_BIRCH ||
         cubes[x - 1][y][z - 1].getId() == config::cube_data::LOG_BIRCH;
}
}
