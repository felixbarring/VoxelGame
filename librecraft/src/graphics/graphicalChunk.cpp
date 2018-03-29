#include "graphicalChunk.h"

#include <algorithm>
#include <iostream>
#include <vector>

#include "texturedCube.h"
#include "transform.h"

using namespace std;
using namespace config::cube_data;

namespace graphics {

bool useSmoothShading{false};
bool useAO{true};
// TODO Make it possible to turn of AO

GraphicalChunk::GraphicalChunk(double _x,
                               double _y,
                               double _z,
                               Matrix& data,
                               Matrix* right,
                               Matrix* left,
                               Matrix* back,
                               Matrix* front)
  : m_xLocation{_x}
  , m_yLocation{_y}
  , m_zLocation{_z}
  , transform{_x + m_width / 2 + 0.5f,
              _y + m_height / 2 + 0.5f,
              _z + m_depth / 2 + 0.5f}
  , m_data{data}
  , m_right{right}
  , m_left{left}
  , m_back{back}
  , m_front{front} {
  m_meshData.push_back(createMeshData(false));
  m_meshData.push_back(createMeshData(true));
}

void
GraphicalChunk::uploadData() {
  {
    std::vector<std::pair<std::vector<float>, int>> vobs{
      {m_meshData[meshData].vertexData, 3},
      {m_meshData[meshData].lightData, 2},
      {m_meshData[meshData].normals, 3},
      {m_meshData[meshData].UV, 3}};
    m_mesh = make_unique<mesh::MeshElement>(move(vobs),
                                            m_meshData[meshData].elementData);
  }

  if (m_meshData[wateterMeshData].vertexData.empty())
    m_hasTransparent = false;

  {
    std::vector<std::pair<std::vector<float>, int>> vobs{
      {m_meshData[wateterMeshData].vertexData, 3},
      {m_meshData[wateterMeshData].lightData, 2},
      {m_meshData[wateterMeshData].normals, 3},
      {m_meshData[wateterMeshData].UV, 3}};
    m_waterMesh = make_unique<mesh::MeshElement>(
      move(vobs), m_meshData[wateterMeshData].elementData);
  }
  m_meshData.clear();
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
GraphicalChunk::getVoxel(int x, int y, int z) {
  bool outsideYRand{y >= config::chunk_data::CHUNK_HEIGHT || y < 0};
  bool outsideXRange{x < 0 || x >= m_width};
  bool outsideZRange{z < 0 || z >= m_depth};

  // TODO
  // We already handle this...
  // Maybe remove the complicated code and use the new way :-)
  if (outsideYRand || (outsideXRange && outsideZRange))
    return nullptr;

  if ((x < m_width && x >= 0) && (z < m_depth && z >= 0)) {
    return &m_data[x][y][z];
  } else if (x == m_width) {

    if (m_right)
      return &((*m_right)[0][y][z]);
    else
      return nullptr;

  } else if (x == -1) {

    if (m_left)
      return &((*m_left)[m_width - 1][y][z]);
    else
      return nullptr;

  } else if (z == m_depth) {

    if (m_back)
      return &((*m_back)[x][y][0]);
    else
      return nullptr;

  } else if (z == -1) {

    if (m_front)
      return &((*m_front)[x][y][m_depth - 1]);
    else
      return nullptr;
  }
  return nullptr;
}

bool
isAirOrWater(int id) {
  return id == AIR || id == WATER;
}

// clang-format off

GraphicalChunk::MeshData
GraphicalChunk::createMeshData(bool transparent) {
  MeshData meshData{};
  std::vector<GLfloat>& vertexData = meshData.vertexData;
  std::vector<GLfloat>& lightData = meshData.lightData;
  std::vector<GLfloat>& normals = meshData.normals;
  std::vector<GLfloat>& UV = meshData.UV;
  std::vector<short>& elementData = meshData.elementData;

  short elementOffset{ 0 };
  int totalNumberOfFaces{ 0 };

  float dx = -m_width / 2;
  float dy = -m_height / 2;
  float dz = -m_depth / 2;

  for (int i{ 0 }; i < m_width; ++i) {
    for (int j{ 0 }; j < m_height; ++j) {
      for (int k{ 0 }; k < m_depth; ++k) {
        Voxel* fd = getVoxel(i, j, k);
        if (!fd)
          continue;

        int id{fd->getId()};
        if (id == AIR || (id == WATER && !transparent) ||
            (id != WATER && transparent)) {
          continue;
        }

        int sideTexture = BLOCK_TEXTURES[id][SIDE_TEXTURE];
        int topTexture = BLOCK_TEXTURES[id][TOP_TEXTURE];
        int bottomTexture = BLOCK_TEXTURES[id][BOTTOM_TEXTURE];

        // This is copy pasted, should be done in a better way?
        if (id == WATER) {
          Voxel* top = getVoxel(i, j + 1, k);
          if (top && top->getId() == AIR) {

            vector<GLfloat> vertex{
              -0.5f + i + dx, 0.4f + j + dy, 0.5f + k + dz,  //
              0.5f + i + dx,  0.4f + j + dy, 0.5f + k + dz,  //
              0.5f + i + dx,  0.4f + j + dy, -0.5f + k + dz, //
              -0.5f + i + dx, 0.4f + j + dy, -0.5f + k + dz, //
            };

            float sun = top->getSunLightValue();
            Lights lights{sun, sun, sun, sun};

            if (useAO)
              doAOTop(i, j, k, lights);

            float other = top->getOtherLightValue();
            Lights otherLights{other, other, other, other};

            if (useAO)
              doAOTop(i, j, k, otherLights);

            if (useSmoothShading) {
              computeAverageTop<true>(fd->getSunLightValue(),
                                        i, j, k,
                                        lights.bottomLeft,
                                        lights.bottomRight,
                                        lights.topRight,
                                        lights.topLeft);

              computeAverageTop<false>(fd->getOtherLightValue(),
                                         i, j, k,
                                         otherLights.bottomLeft,
                                         otherLights.bottomRight,
                                         otherLights.topRight,
                                         otherLights.topLeft);
            }

            vector<GLfloat> light{
              lights.bottomLeft, otherLights.bottomLeft,
              lights.bottomRight, otherLights.bottomRight,
              lights.topRight, otherLights.topRight,
              lights.topLeft, otherLights.topLeft,
            };

            vector<GLfloat> nor{
              0.0f, 1.0f, 0.0f, //
              0.0f, 1.0f, 0.0f, //
              0.0f, 1.0f, 0.0f, //
              0.0f, 1.0f, 0.0f  //
            };

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
            ++totalNumberOfFaces;
          }
          continue;
        }

        Voxel* right = getVoxel(i + 1, j, k);
        if (right && isAirOrWater(right->getId())) {
          vector<GLfloat> vertex{
            0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz,  //
            0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz, //
            0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz,  //
            0.5f + i + dx,  0.5f + j + dy, 0.5f + k + dz,  //
          };

          float sun = right->getSunLightValue();
          Lights lights{sun, sun, sun, sun};
          if (useAO)
            doAORight(i, j, k, lights);

          float other = right->getOtherLightValue();
          Lights otherLights{other, other, other, other};
          if (useAO)
            doAORight(i, j, k, otherLights);

          if (useSmoothShading) {
            computeAverageRight<true>(fd->getSunLightValue(),
                                      i, j, k,
                                      lights.bottomLeft,
                                      lights.bottomRight,
                                      lights.topRight,
                                      lights.topLeft);

            computeAverageRight<false>(fd->getOtherLightValue(),
                                       i, j, k,
                                       otherLights.bottomLeft,
                                       otherLights.bottomRight,
                                       otherLights.topRight,
                                       otherLights.topLeft);
          }

          vector<GLfloat> light{
            lights.bottomLeft, otherLights.bottomLeft,
            lights.bottomRight, otherLights.bottomRight,
            lights.topRight, otherLights.topRight,
            lights.topLeft, otherLights.topLeft,
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
          ++totalNumberOfFaces;
        }

        Voxel* left = getVoxel(i - 1, j, k);
        if (left && isAirOrWater(left->getId())) {
          vector<GLfloat> vertex{
            -0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz, //
            -0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz,  //
            -0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz,   //
            -0.5f + i + dx, 0.5f + j + dy,  -0.5f + k + dz, //
          };

          float sun = left->getSunLightValue();
          Lights lights{sun, sun, sun, sun};
          if (useAO)
            doAOLeft(i, j, k, lights);

          float other = left->getOtherLightValue();
          Lights otherLights{other, other, other, other};
          if (useAO)
            doAOLeft(i, j, k, otherLights);

          if (useSmoothShading) {
            computeAverageLeft<true>(fd->getSunLightValue(),
                                      i, j, k,
                                      lights.bottomLeft,
                                      lights.bottomRight,
                                      lights.topRight,
                                      lights.topLeft);

            computeAverageLeft<false>(fd->getOtherLightValue(),
                                       i, j, k,
                                       otherLights.bottomLeft,
                                       otherLights.bottomRight,
                                       otherLights.topRight,
                                       otherLights.topLeft);
          }

          vector<GLfloat> light{
            lights.bottomLeft, otherLights.bottomLeft,
            lights.bottomRight, otherLights.bottomRight,
            lights.topRight, otherLights.topRight,
            lights.topLeft, otherLights.topLeft,
          };

          vector<GLfloat> nor{
            -1.0f, 0.0f, 0.0f, //
            -1.0f, 0.0f, 0.0f, //
            -1.0f, 0.0f, 0.0f, //
            -1.0f, 0.0f, 0.0f, //
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
          ++totalNumberOfFaces;
        }

        Voxel* back = getVoxel(i, j, k - 1);
        if (back && isAirOrWater(back->getId())) {

          vector<GLfloat> vertex{
            0.5f + i + dx,  -0.5f + j + dy, -0.5f + k + dz, //
            -0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz, //
            -0.5f + i + dx, 0.5f + j + dy,  -0.5f + k + dz, //
            0.5f + i + dx,  0.5f + j + dy,  -0.5f + k + dz, //
          };

          float sun = back->getSunLightValue();
          Lights lights{sun, sun, sun, sun};
          if (useAO)
            doAOBack(i, j, k, lights);

          float other = back->getOtherLightValue();
          Lights otherLights{other, other, other, other};
          if (useAO)
            doAOBack(i, j, k, otherLights);

          if (useSmoothShading) {
            computeAverageBack<true>(fd->getSunLightValue(),
                                      i, j, k,
                                      lights.bottomLeft,
                                      lights.bottomRight,
                                      lights.topRight,
                                      lights.topLeft);

            computeAverageBack<false>(fd->getOtherLightValue(),
                                       i, j, k,
                                       otherLights.bottomLeft,
                                       otherLights.bottomRight,
                                       otherLights.topRight,
                                       otherLights.topLeft);
          }

          vector<GLfloat> light{
            lights.bottomLeft, otherLights.bottomLeft,
            lights.bottomRight, otherLights.bottomRight,
            lights.topRight, otherLights.topRight,
            lights.topLeft, otherLights.topLeft,
          };

          vector<GLfloat> nor{
            0.0f, 0.0f, 1.0f, //
            0.0f, 0.0f, 1.0f, //
            0.0f, 0.0f, 1.0f, //
            0.0f, 0.0f, 1.0f, //
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
          ++totalNumberOfFaces;
        }

        Voxel* front = getVoxel(i, j, k + 1);
        if (front && isAirOrWater(front->getId())) {

          vector<GLfloat> vertex{
            -0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz, //
            0.5f + i + dx,  -0.5f + j + dy, 0.5f + k + dz, //
            0.5f + i + dx,  0.5f + j + dy,  0.5f + k + dz, //
            -0.5f + i + dx, 0.5f + j + dy,  0.5f + k + dz, //
          };

          float sun = front->getSunLightValue();
          Lights lights{sun, sun, sun, sun};
          if (useAO)
            doAOFront(i, j, k, lights);

          float other = front->getOtherLightValue();
          Lights otherLights{other, other, other, other};
          if (useAO)
            doAOFront(i, j, k, otherLights);

          if (useSmoothShading) {
            computeAverageFront<true>(fd->getSunLightValue(),
                                      i, j, k,
                                      lights.bottomLeft,
                                      lights.bottomRight,
                                      lights.topRight,
                                      lights.topLeft);

            computeAverageFront<false>(fd->getOtherLightValue(),
                                       i, j, k,
                                       otherLights.bottomLeft,
                                       otherLights.bottomRight,
                                       otherLights.topRight,
                                       otherLights.topLeft);
          }

          vector<GLfloat> light{
            lights.bottomLeft, otherLights.bottomLeft,
            lights.bottomRight, otherLights.bottomRight,
            lights.topRight, otherLights.topRight,
            lights.topLeft, otherLights.topLeft,
          };

          vector<GLfloat> nor{
            0.0f, 0.0f, -1.0f, //
            0.0f, 0.0f, -1.0f, //
            0.0f, 0.0f, -1.0f, //
            0.0f, 0.0f, -1.0f  //
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
          ++totalNumberOfFaces;
        }

        Voxel* top = getVoxel(i, j + 1, k);
        if (top && isAirOrWater(top->getId())) {

          vector<GLfloat> vertex{
            -0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz,  //
            0.5f + i + dx,  0.5f + j + dy, 0.5f + k + dz,  //
            0.5f + i + dx,  0.5f + j + dy, -0.5f + k + dz, //
            -0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz, //
          };

          float sun = top->getSunLightValue();
          Lights lights{sun, sun, sun, sun};
          if (useAO)
            doAOTop(i, j, k, lights);

          float other = top->getOtherLightValue();
          Lights otherLights{other, other, other, other};
          if (useAO)
            doAOTop(i, j, k, otherLights);

          if (useSmoothShading) {
            computeAverageTop<true>(fd->getSunLightValue(),
                                      i, j, k,
                                      lights.bottomLeft,
                                      lights.bottomRight,
                                      lights.topRight,
                                      lights.topLeft);

            computeAverageTop<false>(fd->getOtherLightValue(),
                                       i, j, k,
                                       otherLights.bottomLeft,
                                       otherLights.bottomRight,
                                       otherLights.topRight,
                                       otherLights.topLeft);
          }

          vector<GLfloat> light{
            lights.bottomLeft, otherLights.bottomLeft,
            lights.bottomRight, otherLights.bottomRight,
            lights.topRight, otherLights.topRight,
            lights.topLeft, otherLights.topLeft,
          };

          vector<GLfloat> nor{
            0.0f, 1.0f, 0.0f, //
            0.0f, 1.0f, 0.0f, //
            0.0f, 1.0f, 0.0f, //
            0.0f, 1.0f, 0.0f  //
          };


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
          ++totalNumberOfFaces;
        }

        Voxel* bottom = getVoxel(i, j - 1, k);
        if (bottom && isAirOrWater(bottom->getId())) {

          vector<GLfloat> vertex{
            -0.5f + i + dx, -0.5f + j + dy, -0.5f + k + dz, //
            0.5f + i + dx,  -0.5f + j + dy, -0.5f + k + dz, //
            0.5f + i + dx,  -0.5f + j + dy, 0.5f + k + dz,  //
            -0.5f + i + dx, -0.5f + j + dy, 0.5f + k + dz,  //
          };

          float sun = bottom->getSunLightValue();
          Lights lights{sun, sun, sun, sun};
          if (useAO)
            doAOBottom(i, j, k, lights);

          float other = bottom->getOtherLightValue();
          Lights otherLights{other, other, other, other};
          if (useAO)
            doAOBottom(i, j, k, otherLights);

          if (useSmoothShading) {
            computeAverageBottom<true>(fd->getSunLightValue(),
                                      i, j, k,
                                      lights.bottomLeft,
                                      lights.bottomRight,
                                      lights.topRight,
                                      lights.topLeft);

            computeAverageBottom<false>(fd->getOtherLightValue(),
                                       i, j, k,
                                       otherLights.bottomLeft,
                                       otherLights.bottomRight,
                                       otherLights.topRight,
                                       otherLights.topLeft);
          }

          vector<GLfloat> light{
            lights.bottomLeft, otherLights.bottomLeft,
            lights.bottomRight, otherLights.bottomRight,
            lights.topRight, otherLights.topRight,
            lights.topLeft, otherLights.topLeft,
          };

          vector<GLfloat> nor{
            0.0f, -1.0f, 0.0f, //
            0.0f, -1.0f, 0.0f, //
            0.0f, -1.0f, 0.0f, //
            0.0f, -1.0f, 0.0f  //
          };

          vector<GLfloat> uv{
            0.0f, 0.0f, static_cast<GLfloat>(bottomTexture),
            1.0f, 0.0f, static_cast<GLfloat>(bottomTexture),
            1.0f, 1.0f, static_cast<GLfloat>(bottomTexture),
            0.0f, 1.0f, static_cast<GLfloat>(bottomTexture)
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
          ++totalNumberOfFaces;
        }
      }
    }
  }
  return meshData;
}

// clang-format on

int AOFactor{4};

void
GraphicalChunk::doAORight(int x, int y, int z, Lights& lights) {
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

  lights.bottomLeft -= min<float>(2.0, bottomLeft) * AOFactor;

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

  lights.bottomRight -= min<float>(2.0, bottomRight) * AOFactor;

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

  lights.topRight -= min<float>(2.0, topRight) * AOFactor;

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

  lights.topLeft -= min<float>(2.0, topLeft) * AOFactor;
}

void
GraphicalChunk::doAOLeft(int x, int y, int z, Lights& lights) {
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

  lights.bottomLeft -= min<float>(2.0, bottomLeft) * AOFactor;

  float bottomRight = 0;
  v = getVoxel(x - 1, y - 1, z);
  if (v && v->getId() != AIR)
    ++bottomRight;
  v = getVoxel(x - 1, y - 1, z + 1);
  if (v && v->getId() != AIR)
    ++bottomRight;
  v = getVoxel(x - 1, y, z + 1);
  if (v && v->getId() != AIR)
    ++bottomRight;

  lights.bottomRight -= min<float>(2.0, bottomRight) * AOFactor;

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

  lights.topRight -= min<float>(2.0, topRight) * AOFactor;

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

  lights.topLeft -= topLeft * AOFactor;
}

void
GraphicalChunk::doAOFront(int x, int y, int z, Lights& lights) {
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

  lights.bottomLeft -= min<float>(2.0, bottomLeft) * AOFactor;

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

  lights.bottomRight -= min<float>(2.0, bottomRight) * AOFactor;

  float topRight = 0;
  v = getVoxel(x + 1, y, z + 1);
  if (v && v->getId() != AIR)
    ++topRight;
  v = getVoxel(x + 1, y + 1, z + 1);
  if (v && v->getId() != AIR)
    ++topRight;
  v = getVoxel(x, y + 1, z + 1);
  if (v && v->getId() != AIR)
    ++topRight;

  lights.topRight -= min<float>(2.0, topRight) * AOFactor;

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

  lights.topLeft -= min<float>(2.0, topLeft) * AOFactor;
}

void
GraphicalChunk::doAOBack(int x, int y, int z, Lights& lights) {
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

  lights.bottomLeft -= min<float>(2.0, bottomLeft) * AOFactor;

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

  lights.bottomRight -= min<float>(2.0, bottomRight) * AOFactor;

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

  lights.topRight -= min<float>(2.0, topRight) * AOFactor;

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

  lights.topLeft -= min<float>(2.0, topLeft) * AOFactor;
}

void
GraphicalChunk::doAOTop(int x, int y, int z, GraphicalChunk::Lights& lights) {
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

  lights.bottomLeft -= min<float>(2.0, bottomLeft) * AOFactor;

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

  lights.bottomRight -= min<float>(2.0, bottomRight) * AOFactor;

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

  lights.topRight -= min<float>(2.0, topRight) * AOFactor;

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

  lights.topLeft -= min<float>(2.0, topLeft) * AOFactor;
}

void
GraphicalChunk::doAOBottom(int x, int y, int z, Lights& lights) {
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

  lights.bottomLeft -= min<float>(2.0, bottomLeft) * AOFactor;

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

  lights.bottomRight -= min<float>(2.0, bottomRight) * AOFactor;

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

  lights.topRight -= min<float>(2.0, topRight) * AOFactor;

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

  lights.topLeft -= min<float>(2.0, topLeft) * AOFactor;
}
}
