#include "graphicalChunk.h"

#include <algorithm>
#include <iostream>
#include <vector>

#include "texturedCube.h"
#include "transform.h"

using namespace std;
using namespace config::cube_data;

namespace graphics {

bool useSmoothShading{ true };
// TODO Make it possible to turn of AO

GraphicalChunk::GraphicalChunk(double _x,
                               double _y,
                               double _z,
                               vector<vector<vector<Voxel>>>& data,
                               vector<vector<vector<Voxel>>>* right,
                               vector<vector<vector<Voxel>>>* left,
                               vector<vector<vector<Voxel>>>* back,
                               vector<vector<vector<Voxel>>>* front)
  : m_xLocation{ _x }
  , m_yLocation{ _y }
  , m_zLocation{ _z }
  , transform{ _x + m_width / 2 + 0.5f,
               _y + m_height / 2 + 0.5f,
               _z + m_depth / 2 + 0.5f },
  m_center{data},
  m_right{right},
  m_left{left},
  m_back{back},
  m_front{front},
  {

  // The face data will be one bigger in each direction.
  // This is to make it possible to compare voxels with
  // neighbors in other chunks.
//  for (int x{ 0 }; x < m_width + 2; ++x) {
//    m_faceData.push_back(vector<vector<CubeFaceData>>());
//    for (int y{ 0 }; y < m_height + 2; ++y) {
//      m_faceData[x].push_back(vector<CubeFaceData>());
//      for (int z{ 0 }; z < m_depth + 2; ++z) {
//        CubeFaceData cube;
//
//        Voxel* voxel = getVoxel(
//          x - 1, y - 1 + m_yLocation, z - 1, data, right, left, back, front);
//        if (voxel) {
//          cube.id = voxel->getId();
//          cube.sunLightValue = voxel->getSunLightValue();
//          cube.otherLightValue = voxel->getOtherLightValue();
//        } else {
//          cube.id = AIR;
//          cube.sunLightValue = 0;
//          cube.otherLightValue = 0;
//        }
//
//        cube.front = false;
//        cube.back = false;
//        cube.left = false;
//        cube.right = false;
//        cube.top = false;
//        cube.bottom = false;
//
//        m_faceData[x][y].push_back(cube);
//      }
//    }
//  }

  // Remove faces and compute lightning
  for (int x{ 0 }; x < m_width; ++x) {
    for (int y{ 0 }; y < m_height; ++y) {
      for (int k{ 0 }; k < m_depth; ++k) {

        Voxel* current = getVoxel(data, x + 1, y + 1, k + 1);
        if (current.id == AIR)
          continue;

        if (current.id == WATER) {
          if (m_faceData[x + 1][y + 2][k + 1].id == AIR) {
            current.top = true;
            current.lvTop_BottomLeft = 5;
            current.lvTop_BottomRight = 5;
            current.lvTop_TopRight = 5;
            current.lvTop_TopLeft = 5;
          }
          continue;
        }

        // X ###################################################################
        CubeFaceData cd;
        cd = m_faceData[x + 2][y + 1][k + 1];
        if (cd.id == AIR || cd.id == WATER) {
          current.right = true;

          if (useSmoothShading) {
            computeAverageRight<true>(cd.sunLightValue,
                                      x + 1,
                                      y + 1,
                                      k + 1,
                                      current.lvRight_BottomLeft,
                                      current.lvRight_BottomRight,
                                      current.lvRight_TopRight,
                                      current.lvRight_TopLeft,
                                      m_faceData);

            computeAverageRight<false>(cd.otherLightValue,
                                       x + 1,
                                       y + 1,
                                       k + 1,
                                       current.olvRight_BottomLeft,
                                       current.olvRight_BottomRight,
                                       current.olvRight_TopRight,
                                       current.olvRight_TopLeft,
                                       m_faceData);

          } else {
            current.lvRight_BottomLeft = cd.sunLightValue;
            current.lvRight_BottomRight = cd.sunLightValue;
            current.lvRight_TopRight = cd.sunLightValue;
            current.lvRight_TopLeft = cd.sunLightValue;

            current.olvRight_BottomLeft = cd.otherLightValue;
            current.olvRight_BottomRight = cd.otherLightValue;
            current.olvRight_TopRight = cd.otherLightValue;
            current.olvRight_TopLeft = cd.otherLightValue;
          }

          doAORight(current, x + 1, y + 1, k + 1, m_faceData);
        }

        cd = m_faceData[x][y + 1][k + 1];
        if (cd.id == AIR || cd.id == WATER) {
          current.left = true;

          if (useSmoothShading) {
            computeAverageLeft<true>(cd.sunLightValue,
                                     x + 1,
                                     y + 1,
                                     k + 1,
                                     current.lvLeft_BottomLeft,
                                     current.lvLeft_BottomRight,
                                     current.lvLeft_TopRight,
                                     current.lvLeft_TopLeft,
                                     m_faceData);

            computeAverageLeft<false>(cd.otherLightValue,
                                      x + 1,
                                      y + 1,
                                      k + 1,
                                      current.olvLeft_BottomLeft,
                                      current.olvLeft_BottomRight,
                                      current.olvLeft_TopRight,
                                      current.olvLeft_TopLeft,
                                      m_faceData);
          } else {
            current.lvLeft_BottomLeft = cd.sunLightValue;
            current.lvLeft_BottomRight = cd.sunLightValue;
            current.lvLeft_TopRight = cd.sunLightValue;
            current.lvLeft_TopLeft = cd.sunLightValue;

            current.lvLeft_BottomLeft = cd.otherLightValue;
            current.lvLeft_BottomRight = cd.otherLightValue;
            current.lvLeft_TopRight = cd.otherLightValue;
            current.lvLeft_TopLeft = cd.otherLightValue;
          }

          doAOLeft(current, x + 1, y + 1, k + 1, m_faceData);
        }

        // Z ###################################################################

        cd = m_faceData[x + 1][y + 1][k + 2];
        if (cd.id == AIR || cd.id == WATER) {
          current.front = true;

          if (useSmoothShading) {
            computeAverageFront<true>(cd.sunLightValue,
                                      x + 1,
                                      y + 1,
                                      k + 1,
                                      current.lvFront_BottomLeft,
                                      current.lvFront_BottomRight,
                                      current.lvFront_TopRight,
                                      current.lvFront_TopLeft,
                                      m_faceData);

            computeAverageFront<false>(cd.otherLightValue,
                                       x + 1,
                                       y + 1,
                                       k + 1,
                                       current.olvFront_BottomLeft,
                                       current.olvFront_BottomRight,
                                       current.olvFront_TopRight,
                                       current.olvFront_TopLeft,
                                       m_faceData);
          } else {
            current.lvFront_BottomLeft = cd.sunLightValue;
            current.lvFront_BottomRight = cd.sunLightValue;
            current.lvFront_TopRight = cd.sunLightValue;
            current.lvFront_TopLeft = cd.sunLightValue;

            current.olvFront_BottomLeft = cd.otherLightValue;
            current.olvFront_BottomRight = cd.otherLightValue;
            current.olvFront_TopRight = cd.otherLightValue;
            current.olvFront_TopLeft = cd.otherLightValue;
          }
          doAOFront(current, x + 1, y + 1, k + 1, m_faceData);
        }

        cd = m_faceData[x + 1][y + 1][k];
        if (cd.id == AIR || cd.id == WATER) {
          current.back = true;

          if (useSmoothShading) {
            computeAverageBack<true>(cd.sunLightValue,
                                     x + 1,
                                     y + 1,
                                     k + 1,
                                     current.lvBack_BottomLeft,
                                     current.lvBack_BottomRight,
                                     current.lvBack_TopRight,
                                     current.lvBack_TopLeft,
                                     m_faceData);

            computeAverageBack<false>(cd.otherLightValue,
                                      x + 1,
                                      y + 1,
                                      k + 1,
                                      current.olvBack_BottomLeft,
                                      current.olvBack_BottomRight,
                                      current.olvBack_TopRight,
                                      current.olvBack_TopLeft,
                                      m_faceData);

          } else {
            current.lvBack_BottomLeft = cd.sunLightValue;
            current.lvBack_BottomRight = cd.sunLightValue;
            current.lvBack_TopRight = cd.sunLightValue;
            current.lvBack_TopLeft = cd.sunLightValue;

            current.olvBack_BottomLeft = cd.otherLightValue;
            current.olvBack_BottomRight = cd.otherLightValue;
            current.olvBack_TopRight = cd.otherLightValue;
            current.olvBack_TopLeft = cd.otherLightValue;
          }

          doAOBack(current, x + 1, y + 1, k + 1, m_faceData);
        }
        // Y ###################################################################

        cd = m_faceData[x + 1][y + 2][k + 1];
        if (cd.id == AIR || cd.id == WATER) {
          current.top = true;

          if (useSmoothShading) {
            computeAverageTop<true>(cd.sunLightValue,
                                    x + 1,
                                    y + 1,
                                    k + 1,
                                    current.lvTop_BottomLeft,
                                    current.lvTop_BottomRight,
                                    current.lvTop_TopRight,
                                    current.lvTop_TopLeft,
                                    m_faceData);

            computeAverageTop<false>(cd.otherLightValue,
                                     x + 1,
                                     y + 1,
                                     k + 1,
                                     current.olvTop_BottomLeft,
                                     current.olvTop_BottomRight,
                                     current.olvTop_TopRight,
                                     current.olvTop_TopLeft,
                                     m_faceData);
          } else {
            current.lvTop_BottomLeft = cd.sunLightValue;
            current.lvTop_BottomRight = cd.sunLightValue;
            current.lvTop_TopRight = cd.sunLightValue;
            current.lvTop_TopLeft = cd.sunLightValue;

            current.olvTop_BottomLeft = cd.otherLightValue;
            current.olvTop_BottomRight = cd.otherLightValue;
            current.olvTop_TopRight = cd.otherLightValue;
            current.olvTop_TopLeft = cd.otherLightValue;
          }

          doAOTop(current, x + 1, y + 1, k + 1, m_faceData);
        }

        cd = m_faceData[x + 1][y][k + 1];
        if (cd.id == AIR || cd.id == WATER) {
          current.bottom = true;

          if (useSmoothShading) {
            computeAverageBottom<true>(cd.sunLightValue,
                                       x + 1,
                                       y + 1,
                                       k + 1,
                                       current.lvBottom_BottomLeft,
                                       current.lvBottom_BottomRight,
                                       current.lvBottom_TopRight,
                                       current.lvBottom_TopLeft,
                                       m_faceData);

            computeAverageBottom<false>(cd.otherLightValue,
                                        x + 1,
                                        y + 1,
                                        k + 1,
                                        current.olvBottom_BottomLeft,
                                        current.olvBottom_BottomRight,
                                        current.olvBottom_TopRight,
                                        current.olvBottom_TopLeft,
                                        m_faceData);
          } else {
            current.lvBottom_BottomLeft = cd.sunLightValue;
            current.lvBottom_BottomRight = cd.sunLightValue;
            current.lvBottom_TopRight = cd.sunLightValue;
            current.lvBottom_TopLeft = cd.sunLightValue;

            current.olvBottom_BottomLeft = cd.otherLightValue;
            current.olvBottom_BottomRight = cd.otherLightValue;
            current.olvBottom_TopRight = cd.otherLightValue;
            current.olvBottom_TopLeft = cd.otherLightValue;
          }

          doAOBottom(current, x + 1, y + 1, k + 1, m_faceData);
        }
      }
    }
  }
}

// ########################################################
// Member Functions########################################
// ########################################################

void
GraphicalChunk::uploadData() {
  vector<GLfloat> vertexData;
  vector<GLfloat> lightData;
  vector<GLfloat> normals;
  vector<GLfloat> UV;
  vector<short> elementData;

  createMeshData(
    false, m_faceData, vertexData, lightData, normals, UV, elementData);

  {
    std::vector<std::pair<std::vector<float>, int>> vobs{
      { vertexData, 3 }, { lightData, 2 }, { normals, 3 }, { UV, 3 }
    };
    m_mesh = make_unique<mesh::MeshElement>(move(vobs), elementData);
  }

  vertexData.clear();
  normals.clear();
  UV.clear();
  elementData.clear();

  createMeshData(
    true, m_faceData, vertexData, lightData, normals, UV, elementData);

  if (vertexData.empty())
    m_hasTransparent = false;

  {
    std::vector<std::pair<std::vector<float>, int>> vobs{
      { vertexData, 3 }, { lightData, 2 }, { normals, 3 }, { UV, 3 }
    };
    m_waterMesh = make_unique<mesh::MeshElement>(move(vobs), elementData);
  }

  m_faceData.clear();
}

void
GraphicalChunk::drawNoneTransparent() {
  m_mesh->draw();
}

void
GraphicalChunk::drawTransparent() {
  m_waterMesh->draw();
}

bool
GraphicalChunk::hasTransparent() {
  return m_hasTransparent;
}

Transform&
GraphicalChunk::getTransform() {
  return transform;
}

float
GraphicalChunk::getxLocation() {
  return m_xLocation;
}

float
GraphicalChunk::getyLocation() {
  return m_yLocation;
}

float
GraphicalChunk::getzLocation() {
  return m_zLocation;
}

// Function for getting voxels, can collect from neighbor chunks data
// If there is no neighbor, nullptr will be returned
// Trying to get a voxel that is not adjacent to this chunk is an error
Voxel*
GraphicalChunk::getVoxel(int x,
                         int y,
                         int z,
                         ) {

  if (y >= config::chunk_data::CHUNK_HEIGHT || y < 0)
    return nullptr;

  if (x < m_width && x >= 0 && z < m_depth && z >= 0) {
    return &data[x][y][z];
  } else if (x == m_width && (z < m_depth && z >= 0)) {

    if (right != nullptr)
      return &((*right)[0][y][z]);
    else
      return nullptr;

  } else if (x == -1 && (z < m_depth && z >= 0)) {

    if (left != nullptr)
      return &((*left)[m_width - 1][y][z]);
    else
      return nullptr;

  } else if (z == m_depth && (x < m_width && x >= 0)) {

    if (back != nullptr)
      return &((*back)[x][y][0]);
    else
      return nullptr;

  } else if (z == -1 && (x < m_width && x >= 0)) {

    if (front != nullptr)
      return &((*front)[x][y][m_depth - 1]);
    else
      return nullptr;
  }
  return nullptr;
}

void
GraphicalChunk::createMeshData(
  bool transparent,
  const vector<vector<vector<CubeFaceData>>>& faceData,
  vector<GLfloat>& vertexData,
  vector<GLfloat>& lightData,
  vector<GLfloat>& normals,
  vector<GLfloat>& UV,
  vector<short>& elementData) {

  short elementOffset{ 0 };
  int totalNumberOfFaces{ 0 };

  float dx = -m_width / 2;
  float dy = -m_height / 2;
  float dz = -m_depth / 2;

  for (int i{ 0 }; i < m_width; ++i) {
    for (int j{ 0 }; j < m_height; ++j) {
      for (int k{ 0 }; k < m_depth; ++k) {
        CubeFaceData fd = faceData[i + 1][j + 1][k + 1];

        int id = fd.id;
        if (id == AIR || (id == WATER && !transparent) ||
            (id != WATER && transparent)) {
          continue;
        }

        GLfloat sideTexture = BLOCK_TEXTURES[id][SIDE_TEXTURE];
        GLfloat topTexture = BLOCK_TEXTURES[id][TOP_TEXTURE];
        GLfloat bottomTexture = BLOCK_TEXTURES[id][BOTTOM_TEXTURE];

        if (fd.right) {
          vector<GLfloat> vertex{
            0.5f + i + dx,  -0.5f + j + dy, 0.5f + k + dz, 0.5f + i + dx,
            -0.5f + j + dy, -0.5f + k + dz, 0.5f + i + dx, 0.5f + j + dy,
            -0.5f + k + dz, 0.5f + i + dx,  0.5f + j + dy, 0.5f + k + dz,
          };

          vector<GLfloat> light{
            fd.lvRight_BottomLeft,  fd.olvRight_BottomLeft,
            fd.lvRight_BottomRight, fd.olvRight_BottomRight,
            fd.lvRight_TopRight,    fd.olvRight_TopRight,
            fd.lvRight_TopLeft,     fd.olvRight_TopLeft,
          };

          vector<GLfloat> nor{
            1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
          };

          vector<GLfloat> uv{
            0.0f, 0.0f, static_cast<GLfloat>(sideTexture),
            1.0f, 0.0f, static_cast<GLfloat>(sideTexture),
            1.0f, 1.0f, static_cast<GLfloat>(sideTexture),
            0.0f, 1.0f, static_cast<GLfloat>(sideTexture),
          };

          vector<short> el{
            static_cast<short>(0 + elementOffset),
            static_cast<short>(1 + elementOffset),
            static_cast<short>(2 + elementOffset),
            static_cast<short>(0 + elementOffset),
            static_cast<short>(2 + elementOffset),
            static_cast<short>(3 + elementOffset),
          };

          vertexData.insert(vertexData.end(), vertex.begin(), vertex.end());
          lightData.insert(lightData.end(), light.begin(), light.end());
          normals.insert(normals.end(), nor.begin(), nor.end());
          UV.insert(UV.end(), uv.begin(), uv.end());
          elementData.insert(elementData.end(), el.begin(), el.end());

          elementOffset += 4;
          totalNumberOfFaces++;
        }

        if (fd.left) {
          vector<GLfloat> vertex{
            -0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz, -0.5f + i + dx,
            -0.5f + j + dy, 0.5f + k + dz,  -0.5f + i + dx, 0.5f + j + dy,
            0.5f + k + dz,  -0.5f + i + dx, 0.5f + j + dy,  -0.5f + k + dz,
          };

          vector<GLfloat> light{
            fd.lvLeft_BottomLeft,  fd.olvLeft_BottomLeft,
            fd.lvLeft_BottomRight, fd.olvLeft_BottomRight,
            fd.lvLeft_TopRight,    fd.olvLeft_TopRight,
            fd.lvLeft_TopLeft,     fd.olvLeft_TopLeft,
          };

          vector<GLfloat> nor{
            -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
          };

          vector<GLfloat> uv{
            0.0f, 0.0f, static_cast<GLfloat>(sideTexture),
            1.0f, 0.0f, static_cast<GLfloat>(sideTexture),
            1.0f, 1.0f, static_cast<GLfloat>(sideTexture),
            0.0f, 1.0f, static_cast<GLfloat>(sideTexture),
          };

          vector<short> el{ static_cast<short>(0 + elementOffset),
                            static_cast<short>(1 + elementOffset),
                            static_cast<short>(2 + elementOffset),
                            static_cast<short>(0 + elementOffset),
                            static_cast<short>(2 + elementOffset),
                            static_cast<short>(3 + elementOffset) };

          vertexData.insert(vertexData.end(), vertex.begin(), vertex.end());
          lightData.insert(lightData.end(), light.begin(), light.end());
          normals.insert(normals.end(), nor.begin(), nor.end());
          UV.insert(UV.end(), uv.begin(), uv.end());
          elementData.insert(elementData.end(), el.begin(), el.end());

          elementOffset += 4;
          totalNumberOfFaces++;
        }

        if (fd.back) {

          vector<GLfloat> vertex{
            0.5f + i + dx,  -0.5f + j + dy, -0.5f + k + dz, //
            -0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz, //
            -0.5f + i + dx, 0.5f + j + dy,  -0.5f + k + dz, //
            0.5f + i + dx,  0.5f + j + dy,  -0.5f + k + dz, //
          };

          vector<GLfloat> light{
            fd.lvBack_BottomLeft,  fd.olvBack_BottomLeft,
            fd.lvBack_BottomRight, fd.olvBack_BottomRight,
            fd.lvBack_TopRight,    fd.olvBack_TopRight,
            fd.lvBack_TopLeft,     fd.olvBack_TopLeft,
          };

          vector<GLfloat> nor{
            0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
          };

          vector<GLfloat> uv{
            0.0f, 0.0f, static_cast<GLfloat>(sideTexture),
            1.0f, 0.0f, static_cast<GLfloat>(sideTexture),
            1.0f, 1.0f, static_cast<GLfloat>(sideTexture),
            0.0f, 1.0f, static_cast<GLfloat>(sideTexture),
          };

          vector<short> el{ static_cast<short>(0 + elementOffset),
                            static_cast<short>(1 + elementOffset),
                            static_cast<short>(2 + elementOffset),
                            static_cast<short>(0 + elementOffset),
                            static_cast<short>(2 + elementOffset),
                            static_cast<short>(3 + elementOffset) };

          vertexData.insert(vertexData.end(), vertex.begin(), vertex.end());
          lightData.insert(lightData.end(), light.begin(), light.end());
          normals.insert(normals.end(), nor.begin(), nor.end());
          UV.insert(UV.end(), uv.begin(), uv.end());
          elementData.insert(elementData.end(), el.begin(), el.end());

          elementOffset += 4;
          totalNumberOfFaces++;
        }

        if (fd.front) {

          vector<GLfloat> vertex{
            -0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz, //
            0.5f + i + dx,  -0.5f + j + dy, 0.5f + k + dz, //
            0.5f + i + dx,  0.5f + j + dy,  0.5f + k + dz, //
            -0.5f + i + dx, 0.5f + j + dy,  0.5f + k + dz, //
          };

          vector<GLfloat> light{
            fd.lvFront_BottomLeft,  fd.olvFront_BottomLeft,
            fd.lvFront_BottomRight, fd.olvFront_BottomRight,
            fd.lvFront_TopRight,    fd.olvFront_TopRight,
            fd.lvFront_TopLeft,     fd.olvFront_TopLeft,
          };

          vector<GLfloat> nor{ 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
                               0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f };

          vector<GLfloat> uv{
            0.0f, 0.0f, static_cast<GLfloat>(sideTexture),
            1.0f, 0.0f, static_cast<GLfloat>(sideTexture),
            1.0f, 1.0f, static_cast<GLfloat>(sideTexture),
            0.0f, 1.0f, static_cast<GLfloat>(sideTexture),
          };

          vector<short> el{ static_cast<short>(0 + elementOffset),
                            static_cast<short>(1 + elementOffset),
                            static_cast<short>(2 + elementOffset),
                            static_cast<short>(0 + elementOffset),
                            static_cast<short>(2 + elementOffset),
                            static_cast<short>(3 + elementOffset) };

          vertexData.insert(vertexData.end(), vertex.begin(), vertex.end());
          lightData.insert(lightData.end(), light.begin(), light.end());
          normals.insert(normals.end(), nor.begin(), nor.end());
          UV.insert(UV.end(), uv.begin(), uv.end());
          elementData.insert(elementData.end(), el.begin(), el.end());

          elementOffset += 4;
          totalNumberOfFaces++;
        }

        if (fd.top) {

          vector<GLfloat> vertex{
            -0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz,  //
            0.5f + i + dx,  0.5f + j + dy, 0.5f + k + dz,  //
            0.5f + i + dx,  0.5f + j + dy, -0.5f + k + dz, //
            -0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz, //
          };

          vector<GLfloat> light{
            fd.lvTop_BottomLeft,   fd.olvTop_BottomLeft, fd.lvTop_BottomRight,
            fd.olvTop_BottomRight, fd.lvTop_TopRight,    fd.olvTop_TopRight,
            fd.lvTop_TopLeft,      fd.olvTop_TopLeft,
          };

          vector<GLfloat> nor{ 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                               0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f };

          vector<GLfloat> uv{ 0.0f, 0.0f, static_cast<GLfloat>(topTexture),
                              1.0f, 0.0f, static_cast<GLfloat>(topTexture),
                              1.0f, 1.0f, static_cast<GLfloat>(topTexture),
                              0.0f, 1.0f, static_cast<GLfloat>(topTexture) };

          vector<short> el{ static_cast<short>(0 + elementOffset),
                            static_cast<short>(1 + elementOffset),
                            static_cast<short>(2 + elementOffset),
                            static_cast<short>(0 + elementOffset),
                            static_cast<short>(2 + elementOffset),
                            static_cast<short>(3 + elementOffset) };

          vertexData.insert(vertexData.end(), vertex.begin(), vertex.end());
          lightData.insert(lightData.end(), light.begin(), light.end());
          normals.insert(normals.end(), nor.begin(), nor.end());
          UV.insert(UV.end(), uv.begin(), uv.end());
          elementData.insert(elementData.end(), el.begin(), el.end());

          elementOffset += 4;
          totalNumberOfFaces++;
        }

        if (fd.bottom) {

          vector<GLfloat> vertex{
            -0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz, //
            0.5f + i + dx,  -0.5f + j + dy, -0.5f + k + dz, //
            0.5f + i + dx,  -0.5f + j + dy, 0.5f + k + dz,  //
            -0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz,  //
          };

          vector<GLfloat> light{
            fd.lvBottom_BottomLeft,  fd.olvBottom_BottomLeft,
            fd.lvBottom_BottomRight, fd.olvBottom_BottomRight,
            fd.lvBottom_TopRight,    fd.olvBottom_TopRight,
            fd.lvBottom_TopLeft,     fd.olvBottom_TopLeft,
          };

          vector<GLfloat> nor{ 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
                               0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f };

          vector<GLfloat> uv{ 0.0f, 0.0f, static_cast<GLfloat>(bottomTexture),
                              1.0f, 0.0f, static_cast<GLfloat>(bottomTexture),
                              1.0f, 1.0f, static_cast<GLfloat>(bottomTexture),
                              0.0f, 1.0f, static_cast<GLfloat>(bottomTexture) };

          vector<short> el{ static_cast<short>(0 + elementOffset),
                            static_cast<short>(1 + elementOffset),
                            static_cast<short>(2 + elementOffset),
                            static_cast<short>(0 + elementOffset),
                            static_cast<short>(2 + elementOffset),
                            static_cast<short>(3 + elementOffset) };

          vertexData.insert(vertexData.end(), vertex.begin(), vertex.end());
          lightData.insert(lightData.end(), light.begin(), light.end());
          normals.insert(normals.end(), nor.begin(), nor.end());
          UV.insert(UV.end(), uv.begin(), uv.end());
          elementData.insert(elementData.end(), el.begin(), el.end());

          elementOffset += 4;
          totalNumberOfFaces++;
        }
      }
    }
  }
}

int AOFactor{ 4 };

void
GraphicalChunk::doAORight(int x, int y, int z) {
  float bottomLeft = 0;
  Voxel* v = getVoxel(x + 1, y, z + 1);
  if (v && v->getId() != AIR)
    ++bottomLeft;
  v = getVoxel(x + 1, y - 1, z + 1);
  if (v && v->getId() != AIR)
    ++bottomLeft;
  v = getVoxel(x + 1, y - 1, z);
  if (v && v->getId() != AIR)
    ++bottomLeft;

//  cf.lvRight_BottomLeft -= min<float>(2.0, bottomLeft) * AOFactor;

  float bottomRight = 0;
  v = getVoxel(x + 1, y - 1, z);
  if (v && v->getId() != AIR)
    ++bottomRight;
  v = getVoxel(x + 1, y - 1, z - 1);
  if (v && v->getId() != AIR)
    ++bottomRight;
  v = getVoxel(x + 1, y, z - 1);
  if (v && v->getId() != AIR)
    ++bottomRight;

//  cf.lvRight_BottomRight -= min<float>(2.0, bottomRight) * AOFactor;

  float topRight = 0;
  v = getVoxel(x + 1, y, z - 1);
  if (v && v->getId() != AIR)
    ++topRight;
  v = getVoxel(x + 1, y + 1, z - 1);
  if (v && v->getId() != AIR)
    ++topRight;
  v = getVoxel(x + 1, y + 1, z);
  if (v && v->getId() != AIR)
    ++topRight;

//  cf.lvRight_TopRight -= min<float>(2.0, topRight) * AOFactor;

  float topLeft = 0;
  v = getVoxel(x + 1, y + 1, z);
  if (v && v->getId() != AIR)
    ++topLeft;
  v = getVoxel(x + 1, y + 1, z + 1);
  if (v && v->getId() != AIR)
    ++topLeft;
  v = getVoxel(x + 1, y, z + 1);
  if (v && v->getId() != AIR)
    ++topLeft;

//  cf.lvRight_TopLeft -= min<float>(2.0, topLeft) * AOFactor;
}

void
GraphicalChunk::doAOLeft(int x, int y, int z) {
  float bottomLeft = 0;
  Voxel* v = getVoxel(x - 1, y, z - 1);
  if (v && v->getId() != AIR)
    ++bottomLeft;
  v = getVoxel(x - 1, y - 1, z - 1);
  if (v && v->getId() != AIR)
    ++bottomLeft;
  v = getVoxel(x - 1, y - 1, z);
  if (v && v->getId() != AIR)
    ++bottomLeft;

//  cf.lvLeft_BottomLeft -= min<float>(2.0, bottomLeft) * AOFactor;

  float bottomRight = 0;
  v = getVoxel(x - 1, y - 1, z);
  if (v && v->getId() != AIR)
    ++bottomRight;
  v = getVoxel(x - 1, y - 1,z + 1);
  if (v && v->getId() != AIR)
    ++bottomRight;
  v = getVoxel(x - 1, y, z + 1);
  if (v && v->getId() != AIR)
    ++bottomRight;

//  cf.lvLeft_BottomRight -= min<float>(2.0, bottomRight) * AOFactor;

  float topRight = 0;
  v = getVoxel(x - 1, y, z + 1);
  if (v && v->getId() != AIR)
    ++topRight;
  v = getVoxel(x - 1, y + 1, z + 1);
  if (v && v->getId() != AIR)
    ++topRight;
  v = getVoxel(x - 1, y + 1, z);
  if (v && v->getId() != AIR)
    ++topRight;

//  cf.lvLeft_TopRight -= min<float>(2.0, topRight) * AOFactor;

  char topLeft = 0;
  v = getVoxel(x - 1, y + 1, z);
  if (v && v->getId() != AIR)
    ++topLeft;
  v = getVoxel(x - 1, y + 1, z - 1);
  if (v && v->getId() != AIR)
    ++topLeft;
  v = getVoxel(x - 1, y, z - 1);
  if (v && v->getId() != AIR)
    ++topLeft;

//  cf.lvLeft_TopLeft -= topLeft * AOFactor;
}

void
GraphicalChunk::doAOFront(int x, int y, int z) {
  float bottomLeft = 0;
  Voxel* v = getVoxel(x - 1, y, z + 1);
  if (v && v->getId() != AIR)
    ++bottomLeft;
  v = getVoxel(x - 1, y - 1, z + 1);
  if (v && v->getId() != AIR)
    ++bottomLeft;
  v = getVoxel(x, y - 1, z + 1);
  if (v && v->getId() != AIR)
    ++bottomLeft;

//  cf.lvFront_BottomLeft -= min<float>(2.0, bottomLeft) * AOFactor;

  float bottomRight = 0;
  v = getVoxel(x, y - 1, z + 1);
  if (v && v->getId() != AIR)
    ++bottomRight;
  v = getVoxel(x + 1, y - 1, z + 1);
  if (v && v->getId() != AIR)
    ++bottomRight;
  v = getVoxel(x + 1, y, z + 1);
  if (v && v->getId() != AIR)
    ++bottomRight;

//  cf.lvFront_BottomRight -= min<float>(2.0, bottomRight) * AOFactor;

  float topRight = 0;
  v = getVoxel(x + 1, y, z + 1);
  if (v && v->getId() != AIR)
    ++topRight;
  v = getVoxel(x + 1, y + 1, z + 1);
  if (v && v->getId()!= AIR)
    ++topRight;
  v = getVoxel(x, y + 1, z + 1);
  if (v && v->getId() != AIR)
    ++topRight;

//  cf.lvFront_TopRight -= min<float>(2.0, topRight) * AOFactor;

  float topLeft = 0;
  v = getVoxel(x, y + 1, z + 1);
  if (v && v->getId() != AIR)
    ++topLeft;
  v = getVoxel(x - 1, y + 1, z + 1);
  if (v && v->getId() != AIR)
    ++topLeft;
  v = getVoxel(x - 1, y, z + 1);
  if (v && v->getId() != AIR)
    ++topLeft;

//  cf.lvFront_TopLeft -= min<float>(2.0, topLeft) * AOFactor;
}

void
GraphicalChunk::doAOBack(int x, int y, int z) {
  float bottomLeft = 0;
  Voxel* v = getVoxel(x + 1, y, z - 1);
  if (v && v->getId() != AIR)
    ++bottomLeft;
  v = getVoxel(x + 1, y - 1, z - 1);
  if (v && v->getId() != AIR)
    ++bottomLeft;
  v = getVoxel(x, y - 1, z - 1);
  if (v && v->getId() != AIR)
    ++bottomLeft;

//  cf.lvBack_BottomLeft -= min<float>(2.0, bottomLeft) * AOFactor;

  float bottomRight = 0;
  v = getVoxel(x, y - 1, z - 1);
  if (v && v->getId() != AIR)
    ++bottomRight;
  v = getVoxel(x - 1, y - 1, z - 1);
  if (v && v->getId() != AIR)
    ++bottomRight;
  v = getVoxel(x - 1, y, z - 1);
  if (v && v->getId() != AIR)
    ++bottomRight;

//  cf.lvBack_BottomRight -= min<float>(2.0, bottomRight) * AOFactor;

  float topRight = 0;
  v = getVoxel(x - 1, y, z - 1);
  if (v && v->getId() != AIR)
    ++topRight;
  v = getVoxel(x - 1, y + 1, z - 1);
  if (v && v->getId() != AIR)
    ++topRight;
  v = getVoxel(x, y + 1, z - 1);
  if (v && v->getId() != AIR)
    ++topRight;

//  cf.lvBack_TopRight -= min<float>(2.0, topRight) * AOFactor;

  float topLeft = 0;
  v = getVoxel(x, y + 1, z - 1);
  if (v && v->getId() != AIR)
    ++topLeft;
  v = getVoxel(x + 1, y + 1, z - 1);
  if (v && v->getId() != AIR)
    ++topLeft;
  v = getVoxel(x + 1, y, z - 1);
  if (v && v->getId() != AIR)
    ++topLeft;

//  cf.lvBack_TopLeft -= min<float>(2.0, topLeft) * AOFactor;
}

void
GraphicalChunk::doAOTop(int x, int y, int z) {
  float bottomLeft = 0;
  Voxel* v = getVoxel(x - 1, y + 1, z);
  if (v && v->getId() != AIR)
    ++bottomLeft;
  v = getVoxel(x - 1, y + 1, z + 1);
  if (v && v->getId() != AIR)
    ++bottomLeft;
  v = getVoxel(x, y + 1, z + 1);
  if (v && v->getId() != AIR)
    ++bottomLeft;

//  cf.lvTop_BottomLeft -= min<float>(2.0, bottomLeft) * AOFactor;

  float bottomRight = 0;
  v = getVoxel(x, y + 1, z + 1);
  if (v && v->getId() != AIR)
    ++bottomRight;
  v = getVoxel(x + 1, y + 1, z + 1);
  if (v && v->getId() != AIR)
    ++bottomRight;
  v = getVoxel(x + 1, y + 1, z);
  if (v && v->getId() != AIR)
    ++bottomRight;

//  cf.lvTop_BottomRight -= min<float>(2.0, bottomRight) * AOFactor;

  float topRight = 0;
  v = getVoxel(x + 1, y + 1, z);
  if (v && v->getId() != AIR)
    ++topRight;
  v = getVoxel(x + 1, y + 1, z - 1);
  if (v && v->getId() != AIR)
    ++topRight;
  v = getVoxel(x, y + 1, z - 1);
  if (v && v->getId() != AIR)
    ++topRight;

//  cf.lvTop_TopRight -= min<float>(2.0, topRight) * AOFactor;

  float topLeft = 0;
  v = getVoxel(x, y + 1, z - 1);
  if (v && v->getId() != AIR)
    ++topLeft;
  v = getVoxel(x - 1, y + 1, z - 1);
  if (v && v->getId() != AIR)
    ++topLeft;
  v = getVoxel(x - 1, y + 1, z);
  if (v && v->getId() != AIR)
    ++topLeft;

//  cf.lvTop_TopLeft -= min<float>(2.0, topLeft) * AOFactor;
}

void
GraphicalChunk::doAOBottom(int x, int y, int z) {
  float bottomLeft = 0;
  Voxel* v = getVoxel(x - 1, y - 1, z);
  if (v && v->getId() != AIR)
    ++bottomLeft;
  v = getVoxel(x - 1, y - 1, z - 1);
  if (v && v->getId() != AIR)
    ++bottomLeft;
  v = getVoxel(x, y - 1, z - 1);
  if (v && v->getId() != AIR)
    ++bottomLeft;

  //cf.lvBottom_BottomLeft -= min<float>(2.0, bottomLeft) * AOFactor;

  float bottomRight = 0;
  v = getVoxel(x, y - 1, z - 1);
  if (v && v->getId() != AIR)
    ++bottomRight;
  v = getVoxel(x + 1, y - 1, z - 1);
  if (v && v->getId() != AIR)
    ++bottomRight;
  v = getVoxel(x + 1, y - 1, z);
  if (v && v->getId() != AIR)
    ++bottomRight;

//  cf.lvBottom_BottomRight -= min<float>(2.0, bottomRight) * AOFactor;

  float topRight = 0;
  v = getVoxel(x + 1, y - 1, z);
  if (v && v->getId() != AIR)
    ++topRight;
  v = getVoxel(x + 1, y - 1, z + 1);
  if (v && v->getId() != AIR)
    ++topRight;
  v = getVoxel(x, y - 1, z + 1);
  if (v && v->getId() != AIR)
    ++topRight;

//  cf.lvBottom_TopRight -= min<float>(2.0, topRight) * AOFactor;

  float topLeft = 0;
  v = getVoxel(x, y - 1, z + 1);
  if (v && v->getId() != AIR)
    ++topLeft;
  v = getVoxel(x - 1, y - 1, z + 1);
  if (v && v->getId() != AIR)
    ++topLeft;
  v = getVoxel(x - 1, y - 1, z);
  if (v && v->getId() != AIR)
    ++topLeft;

  //  cf.lvBottom_TopLeft -= min<float>(2.0, topLeft) * AOFactor;
}
}
