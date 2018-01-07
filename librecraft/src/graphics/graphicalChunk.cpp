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
bool useAO{ true };
// TODO Make it possible to turn of AO

GraphicalChunk::GraphicalChunk(double _x, double _y, double _z,
                               Matrix& data,
                               Matrix* right,
                               Matrix* left,
                               Matrix* back,
                               Matrix* front)
  : m_xLocation{ _x }
  , m_yLocation{ _y }
  , m_zLocation{ _z }
  , transform{ _x + m_width / 2 + 0.5f,
               _y + m_height / 2 + 0.5f,
               _z + m_depth / 2 + 0.5f }
  , m_data{data}
  , m_right{right}
  , m_left{left}
  , m_back{back}
  , m_front{front}
{

  m_meshData.push_back(createMeshData(false));
  m_meshData.push_back(createMeshData(true));
}

void
GraphicalChunk::uploadData() {
  {
    std::vector<std::pair<std::vector<float>, int>> vobs{
      { m_meshData[meshData].vertexData, 3 }, { m_meshData[meshData].lightData, 2 },
      { m_meshData[meshData].normals, 3 }, { m_meshData[meshData].UV, 3 }
    };
    m_mesh = make_unique<mesh::MeshElement>(move(vobs), m_meshData[meshData].elementData);
  }

  if (m_meshData[wateterMeshData].vertexData.empty())
    m_hasTransparent = false;

  {
    std::vector<std::pair<std::vector<float>, int>> vobs{
      { m_meshData[wateterMeshData].vertexData, 3 }, { m_meshData[wateterMeshData].lightData, 2 },
      { m_meshData[wateterMeshData].normals, 3 }, {m_meshData[wateterMeshData]. UV, 3 }
    };
    m_waterMesh = make_unique<mesh::MeshElement>(move(vobs),
        m_meshData[wateterMeshData].elementData);
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
  bool outsideZRange{z < 0 && z >= m_depth};

  if (outsideYRand || (outsideXRange && outsideZRange))
    return nullptr;

  if (x < m_width && x >= 0 && (z < m_depth && z >= 0)) {
    return &m_data[x][y][z];
  } else if (x == m_width && (z < m_depth && z >= 0)) {

    if (m_right)
      return &((*m_right)[0][y][z]);
    else
      return nullptr;

  } else if (x == -1 && (z < m_depth && z >= 0)) {

    if (m_left )
      return &((*m_left)[m_width - 1][y][z]);
    else
      return nullptr;

  } else if (z == m_depth && (x < m_width && x >= 0)) {

    if (m_back)
      return &((*m_back)[x][y][0]);
    else
      return nullptr;

  } else if (z == -1 && (x < m_width && x >= 0)) {

    if (m_front)
      return &((*m_front)[x][y][m_depth - 1]);
    else
      return nullptr;
  }
  return nullptr;
}

bool
isAirOrWater(int id ) {
  return id == AIR || id == WATER;
}

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
              -0.5f + i + dx, 0.5f + j + dy, 0.5f + k + dz,  //
              0.5f + i + dx,  0.5f + j + dy, 0.5f + k + dz,  //
              0.5f + i + dx,  0.5f + j + dy, -0.5f + k + dz, //
              -0.5f + i + dx, 0.5f + j + dy, -0.5f + k + dz, //
            };

            float sun = top->getSunLightValue();
            Lights lights{sun, sun, sun, sun};
            doAOTop(i, j, k, lights);

            float other = top->getOtherLightValue();
            Lights otherLights{other, other, other, other};
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
          doAORight(i, j, k, lights);

          float other = right->getOtherLightValue();
          Lights otherLights{other, other, other, other};
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
          doAOLeft(i, j, k, lights);

          float other = left->getOtherLightValue();
          Lights otherLights{other, other, other, other};
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
          doAOBack(i, j, k, lights);

          float other = back->getOtherLightValue();
          Lights otherLights{other, other, other, other};
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
          doAOFront(i, j, k, lights);

          float other = front->getOtherLightValue();
          Lights otherLights{other, other, other, other};
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
          doAOTop(i, j, k, lights);

          float other = top->getOtherLightValue();
          Lights otherLights{other, other, other, other};
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
          doAOBottom(i, j, k, lights);

          float other = bottom->getOtherLightValue();
          Lights otherLights{other, other, other, other};
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

int AOFactor{ 4 };

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
  v = getVoxel(x - 1, y - 1,z + 1);
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
  if (v && v->getId()!= AIR)
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

/*
  // Remove faces and compute lightning
  for (int x{ 0 }; x < m_width; ++x) {
    for (int y{ 0 }; y < m_height; ++y) {
      for (int k{ 0 }; k < m_depth; ++k) {

        CubeFaceData& current = m_faceData[x + 1][y + 1][k + 1];
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
  */

