#ifndef SRC_GRAPHICS_GRAPHICALCHUNK_H_
#define SRC_GRAPHICS_GRAPHICALCHUNK_H_

#include <memory>
#include <vector>

#include "mesh/meshElement.h"
#include "transform.h"

#include "../config/data.h"
#include "../util/voxel.h"

namespace graphics {

using Matrix = std::vector<std::vector<std::vector<Voxel>>>;

/**
 *
 */
class GraphicalChunk {
public:
  /**
   *
   * @param x
   * @param y
   * @param z
   * @param data
   * @param right
   * @param left
   * @param back
   * @param front
   */
  GraphicalChunk(double x,
                 double y,
                 double z,
                 Matrix& data,
                 Matrix* right,
                 Matrix* left,
                 Matrix* back,
                 Matrix* front);

  /**
   * Creates the meshes that will be used for rendering. This function must be
   * called from the main thread that has the opengl context. Forgetting to
   * call this function before draw is an error. Should only be done once.
   */
  void uploadData();

  /**
   *
   */
  void drawNoneTransparent();

  /**
   *
   */
  void drawTransparent();

  /**
   *
   * @return
   */
  bool hasTransparent();

  /**
   *
   * @return
   */
  Transform& getTransform();

  /**
   *
   * @return
   */
  float getxLocation();

  /**
   *
   * @return
   */
  float getyLocation();

  /**
   *
   * @return
   */
  float getzLocation();

private:

  void createMeshData(bool transparent,
      std::vector<GLfloat>& vertexData,
      std::vector<GLfloat>& lightData,
      std::vector<GLfloat>& normals,
      std::vector<GLfloat>& UV,
      std::vector<short>& elementData);

  void doAORight(int x, int y, int z);

  void doAOLeft(int x, int y, int z);

  void doAOFront(int x, int y, int z);

  void doAOBack(int x, int y, int z);

  void doAOTop(int x, int y, int z);

  void doAOBottom(int x, int y, int z);

  Voxel* getVoxel(int x, int y, int z);

/*
  template<bool sunLight>
  void computeAverageRight(
    int lightValue,
    int x,
    int y,
    int z,
    float& bottomLeft,
    float& bottomRight,
    float& topRight,
    float& topLeft,
    std::vector<std::vector<std::vector<Face>>>& faceData) {

    Face& cBottomLeft = faceData[x][y - 1][z + 1];
    Face& cBottomLeft_Right = faceData[x + 1][y - 1][z + 1];

    Face& cBottomMiddle = faceData[x][y - 1][z];
    Face& cBottomMiddle_Right = faceData[x + 1][y - 1][z];

    Face& cBottomRight = faceData[x][y - 1][z - 1];
    Face& cBottomRight_Right = faceData[x + 1][y - 1][z - 1];

    Face& cRightRight = faceData[x][y][z - 1];
    Face& cRightRight_Right = faceData[x + 1][y][z - 1];

    Face& cTopRight = faceData[x][y + 1][z - 1];
    Face& cTopRight_Right = faceData[x + 1][y + 1][z - 1];

    Face& cTopMiddle = faceData[x][y + 1][z];
    Face& cTopMiddle_Right = faceData[x + 1][y + 1][z];

    Face& cTopLeft = faceData[x][y + 1][z + 1];
    Face& cTopLeft_Right = faceData[x + 1][y + 1][z + 1];

    Face& cLeftLeft = faceData[x][y][z + 1];
    Face& cLeftLeft_Right = faceData[x + 1][y][z + 1];

    if
      constexpr(sunLight) {
        computeAverageHelper<true>(lightValue,
                                   cLeftLeft,
                                   cLeftLeft_Right,
                                   cBottomLeft,
                                   cBottomLeft_Right,
                                   cBottomMiddle,
                                   cBottomMiddle_Right,
                                   cBottomRight,
                                   cBottomRight_Right,
                                   cRightRight,
                                   cRightRight_Right,
                                   cTopRight,
                                   cTopRight_Right,
                                   cTopMiddle,
                                   cTopMiddle_Right,
                                   cTopLeft,
                                   cTopLeft_Right,
                                   bottomLeft,
                                   bottomRight,
                                   topRight,
                                   topLeft);
      }
    else {
      computeAverageHelper<false>(lightValue,
                                  cLeftLeft,
                                  cLeftLeft_Right,
                                  cBottomLeft,
                                  cBottomLeft_Right,
                                  cBottomMiddle,
                                  cBottomMiddle_Right,
                                  cBottomRight,
                                  cBottomRight_Right,
                                  cRightRight,
                                  cRightRight_Right,
                                  cTopRight,
                                  cTopRight_Right,
                                  cTopMiddle,
                                  cTopMiddle_Right,
                                  cTopLeft,
                                  cTopLeft_Right,
                                  bottomLeft,
                                  bottomRight,
                                  topRight,
                                  topLeft);
    }
  }

  template<bool sunLight>
  void computeAverageLeft(
    int lightValue,
    int x,
    int y,
    int z,
    float& bottomLeft,
    float& bottomRight,
    float& topRight,
    float& topLeft,
    std::vector<std::vector<std::vector<CubeFaceData>>>& faceData) {

    CubeFaceData& cBottomLeft = faceData[x][y - 1][z - 1];
    CubeFaceData& cBottomLeft_Left = faceData[x - 1][y - 1][z - 1];

    CubeFaceData& cBottomMiddle = faceData[x][y - 1][z];
    CubeFaceData& cBottomMiddle_Left = faceData[x - 1][y - 1][z];

    CubeFaceData& cBottomRight = faceData[x][y - 1][z + 1];
    CubeFaceData& cBottomRight_Left = faceData[x - 1][y - 1][z + 1];

    CubeFaceData& cRightRight = faceData[x][y][z + 1];
    CubeFaceData& cRightRight_Left = faceData[x - 1][y][z + 1];

    CubeFaceData& cTopRight = faceData[x][y + 1][z + 1];
    CubeFaceData& cTopRight_Left = faceData[x - 1][y + 1][z + 1];

    CubeFaceData& cTopMiddle = faceData[x][y + 1][z];
    CubeFaceData& cTopMiddle_Left = faceData[x - 1][y + 1][z];

    CubeFaceData& cTopLeft = faceData[x][y + 1][z - 1];
    CubeFaceData& cTopLeft_Left = faceData[x - 1][y + 1][z - 1];

    CubeFaceData& cLeftLeft = faceData[x][y][z - 1];
    CubeFaceData& cLeftLeft_Left = faceData[x - 1][y][z - 1];

    if
      constexpr(sunLight) {
        computeAverageHelper<true>(lightValue,
                                   cLeftLeft,
                                   cLeftLeft_Left,
                                   cBottomLeft,
                                   cBottomLeft_Left,
                                   cBottomMiddle,
                                   cBottomMiddle_Left,
                                   cBottomRight,
                                   cBottomRight_Left,
                                   cRightRight,
                                   cRightRight_Left,
                                   cTopRight,
                                   cTopRight_Left,
                                   cTopMiddle,
                                   cTopMiddle_Left,
                                   cTopLeft,
                                   cTopLeft_Left,
                                   bottomLeft,
                                   bottomRight,
                                   topRight,
                                   topLeft);
      }
    else {
      computeAverageHelper<false>(lightValue,
                                  cLeftLeft,
                                  cLeftLeft_Left,
                                  cBottomLeft,
                                  cBottomLeft_Left,
                                  cBottomMiddle,
                                  cBottomMiddle_Left,
                                  cBottomRight,
                                  cBottomRight_Left,
                                  cRightRight,
                                  cRightRight_Left,
                                  cTopRight,
                                  cTopRight_Left,
                                  cTopMiddle,
                                  cTopMiddle_Left,
                                  cTopLeft,
                                  cTopLeft_Left,
                                  bottomLeft,
                                  bottomRight,
                                  topRight,
                                  topLeft);
    }
  }

  template<bool sunLight>
  void computeAverageTop(
    int lightValue,
    int x,
    int y,
    int z,
    float& bottomLeft,
    float& bottomRight,
    float& topRight,
    float& topLeft,
    std::vector<std::vector<std::vector<CubeFaceData>>>& faceData) {

    CubeFaceData& cBottomLeft = faceData[x - 1][y][z + 1];
    CubeFaceData& cBottomLeft_Top = faceData[x - 1][y + 1][z + 1];

    CubeFaceData& cBottomMiddle = faceData[x][y][z + 1];
    CubeFaceData& cBottomMiddle_Top = faceData[x][y + 1][z + 1];

    CubeFaceData& cBottomRight = faceData[x + 1][y][z + 1];
    CubeFaceData& cBottomRight_Top = faceData[x + 1][y + 1][z + 1];

    CubeFaceData& cRightRight = faceData[x + 1][y][z];
    CubeFaceData& cRightRight_Top = faceData[x + 1][y + 1][z];

    CubeFaceData& cTopRight = faceData[x + 1][y][z - 1];
    CubeFaceData& cTopRight_Top = faceData[x + 1][y + 1][z - 1];

    CubeFaceData& cTopMiddle = faceData[x][y][z - 1];
    CubeFaceData& cTopMiddle_Top = faceData[x][y + 1][z - 1];

    CubeFaceData& cTopLeft = faceData[x - 1][y][z - 1];
    CubeFaceData& cTopLeft_Top = faceData[x - 1][y + 1][z - 1];

    CubeFaceData& cLeftLeft = faceData[x - 1][y][z];
    CubeFaceData& cLeftLeft_Top = faceData[x - 1][y + 1][z];

    if
      constexpr(sunLight) {
        computeAverageHelper<true>(lightValue,
                                   cLeftLeft,
                                   cLeftLeft_Top,
                                   cBottomLeft,
                                   cBottomLeft_Top,
                                   cBottomMiddle,
                                   cBottomMiddle_Top,
                                   cBottomRight,
                                   cBottomRight_Top,
                                   cRightRight,
                                   cRightRight_Top,
                                   cTopRight,
                                   cTopRight_Top,
                                   cTopMiddle,
                                   cTopMiddle_Top,
                                   cTopLeft,
                                   cTopLeft_Top,
                                   bottomLeft,
                                   bottomRight,
                                   topRight,
                                   topLeft);
      }
    else {
      computeAverageHelper<false>(lightValue,
                                  cLeftLeft,
                                  cLeftLeft_Top,
                                  cBottomLeft,
                                  cBottomLeft_Top,
                                  cBottomMiddle,
                                  cBottomMiddle_Top,
                                  cBottomRight,
                                  cBottomRight_Top,
                                  cRightRight,
                                  cRightRight_Top,
                                  cTopRight,
                                  cTopRight_Top,
                                  cTopMiddle,
                                  cTopMiddle_Top,
                                  cTopLeft,
                                  cTopLeft_Top,
                                  bottomLeft,
                                  bottomRight,
                                  topRight,
                                  topLeft);
    }
  }

  template<bool sunLight>
  void computeAverageBottom(
    int lightValue,
    int x,
    int y,
    int z,
    float& bottomLeft,
    float& bottomRight,
    float& topRight,
    float& topLeft,
    std::vector<std::vector<std::vector<CubeFaceData>>>& faceData) {

    CubeFaceData& cBottomLeft = faceData[x - 1][y][z - 1];
    CubeFaceData& cBottomLeft_Bottom = faceData[x - 1][y - 1][z - 1];

    CubeFaceData& cBottomMiddle = faceData[x][y][z - 1];
    CubeFaceData& cBottomMiddle_Bottom = faceData[x][y - 1][z - 1];

    CubeFaceData& cBottomRight = faceData[x + 1][y][z - 1];
    CubeFaceData& cBottomRight_Bottom = faceData[x + 1][y - 1][z - 1];

    CubeFaceData& cRightRight = faceData[x + 1][y][z];
    CubeFaceData& cRightRight_Bottom = faceData[x + 1][y - 1][z];

    CubeFaceData& cTopRight = faceData[x + 1][y][z + 1];
    CubeFaceData& cTopRight_Bottom = faceData[x + 1][y - 1][z + 1];

    CubeFaceData& cTopMiddle = faceData[x][y][z + 1];
    CubeFaceData& cTopMiddle_Bottom = faceData[x][y - 1][z + 1];

    CubeFaceData& cTopLeft = faceData[x - 1][y][z + 1];
    CubeFaceData& cTopLeft_Bottom = faceData[x - 1][y - 1][z + 1];

    CubeFaceData& cLeftLeft = faceData[x - 1][y][z];
    CubeFaceData& cLeftLeft_Bottom = faceData[x - 1][y - 1][z];

    if
      constexpr(sunLight) {
        computeAverageHelper<true>(lightValue,
                                   cLeftLeft,
                                   cLeftLeft_Bottom,
                                   cBottomLeft,
                                   cBottomLeft_Bottom,
                                   cBottomMiddle,
                                   cBottomMiddle_Bottom,
                                   cBottomRight,
                                   cBottomRight_Bottom,
                                   cRightRight,
                                   cRightRight_Bottom,
                                   cTopRight,
                                   cTopRight_Bottom,
                                   cTopMiddle,
                                   cTopMiddle_Bottom,
                                   cTopLeft,
                                   cTopLeft_Bottom,
                                   bottomLeft,
                                   bottomRight,
                                   topRight,
                                   topLeft);
      }
    else {
      computeAverageHelper<false>(lightValue,
                                  cLeftLeft,
                                  cLeftLeft_Bottom,
                                  cBottomLeft,
                                  cBottomLeft_Bottom,
                                  cBottomMiddle,
                                  cBottomMiddle_Bottom,
                                  cBottomRight,
                                  cBottomRight_Bottom,
                                  cRightRight,
                                  cRightRight_Bottom,
                                  cTopRight,
                                  cTopRight_Bottom,
                                  cTopMiddle,
                                  cTopMiddle_Bottom,
                                  cTopLeft,
                                  cTopLeft_Bottom,
                                  bottomLeft,
                                  bottomRight,
                                  topRight,
                                  topLeft);
    }
  }

  template<bool sunLight>
  void computeAverageBack(
    int lightValue,
    int x,
    int y,
    int z,
    float& bottomLeft,
    float& bottomRight,
    float& topRight,
    float& topLeft,
    std::vector<std::vector<std::vector<CubeFaceData>>>& faceData) {

    CubeFaceData& cBottomLeft = faceData[x + 1][y - 1][z];
    CubeFaceData& cBottomLeft_Back = faceData[x + 1][y - 1][z - 1];

    CubeFaceData& cBottomMiddle = faceData[x][y - 1][z];
    CubeFaceData& cBottomMiddle_Back = faceData[x][y - 1][z - 1];

    CubeFaceData& cBottomRight = faceData[x - 1][y - 1][z];
    CubeFaceData& cBottomRight_Back = faceData[x - 1][y - 1][z - 1];

    CubeFaceData& cRightRight = faceData[x - 1][y][z];
    CubeFaceData& cRightRight_Back = faceData[x - 1][y][z - 1];

    CubeFaceData& cTopRight = faceData[x - 1][y + 1][z];
    CubeFaceData& cTopRight_Back = faceData[x - 1][y + 1][z - 1];

    CubeFaceData& cTopMiddle = faceData[x][y + 1][z];
    CubeFaceData& cTopMiddle_Back = faceData[x][y + 1][z - 1];

    CubeFaceData& cTopLeft = faceData[x + 1][y + 1][z];
    CubeFaceData& cTopLeft_Back = faceData[x + 1][y + 1][z - 1];

    CubeFaceData& cLeftLeft = faceData[x + 1][y][z];
    CubeFaceData& cLeftLeft_Back = faceData[x + 1][y][z - 1];

    if
      constexpr(sunLight) {
        computeAverageHelper<true>(lightValue,
                                   cLeftLeft,
                                   cLeftLeft_Back,
                                   cBottomLeft,
                                   cBottomLeft_Back,
                                   cBottomMiddle,
                                   cBottomMiddle_Back,
                                   cBottomRight,
                                   cBottomRight_Back,
                                   cRightRight,
                                   cRightRight_Back,
                                   cTopRight,
                                   cTopRight_Back,
                                   cTopMiddle,
                                   cTopMiddle_Back,
                                   cTopLeft,
                                   cTopLeft_Back,
                                   bottomLeft,
                                   bottomRight,
                                   topRight,
                                   topLeft);
      }
    else {
      computeAverageHelper<false>(lightValue,
                                  cLeftLeft,
                                  cLeftLeft_Back,
                                  cBottomLeft,
                                  cBottomLeft_Back,
                                  cBottomMiddle,
                                  cBottomMiddle_Back,
                                  cBottomRight,
                                  cBottomRight_Back,
                                  cRightRight,
                                  cRightRight_Back,
                                  cTopRight,
                                  cTopRight_Back,
                                  cTopMiddle,
                                  cTopMiddle_Back,
                                  cTopLeft,
                                  cTopLeft_Back,
                                  bottomLeft,
                                  bottomRight,
                                  topRight,
                                  topLeft);
    }
  }

  template<bool sunLight>
  void computeAverageFront(
    int lightValue,
    int x,
    int y,
    int z,
    float& bottomLeft,
    float& bottomRight,
    float& topRight,
    float& topLeft,
    std::vector<std::vector<std::vector<CubeFaceData>>>& faceData) {

    CubeFaceData& cBottomLeft = faceData[x - 1][y - 1][z];
    CubeFaceData& cBottomLeft_Front = faceData[x - 1][y - 1][z + 1];

    CubeFaceData& cBottomMiddle = faceData[x][y - 1][z];
    CubeFaceData& cBottomMiddle_Front = faceData[x][y - 1][z + 1];

    CubeFaceData& cBottomRight = faceData[x + 1][y - 1][z];
    CubeFaceData& cBottomRight_Front = faceData[x + 1][y - 1][z + 1];

    CubeFaceData& cRightRight = faceData[x + 1][y][z];
    CubeFaceData& cRightRight_Front = faceData[x + 1][y][z + 1];

    CubeFaceData& cTopRight = faceData[x + 1][y + 1][z];
    CubeFaceData& cTopRight_Front = faceData[x + 1][y + 1][z + 1];

    CubeFaceData& cTopMiddle = faceData[x][y + 1][z];
    CubeFaceData& cTopMiddle_Front = faceData[x][y + 1][z + 1];

    CubeFaceData& cTopLeft = faceData[x - 1][y + 1][z];
    CubeFaceData& cTopLeft_Front = faceData[x - 1][y + 1][z + 1];

    CubeFaceData& cLeftLeft = faceData[x - 1][y][z];
    CubeFaceData& cLeftLeft_Front = faceData[x - 1][y][z + 1];

    if
      constexpr(sunLight) {
        computeAverageHelper<true>(lightValue,
                                   cLeftLeft,
                                   cLeftLeft_Front,
                                   cBottomLeft,
                                   cBottomLeft_Front,
                                   cBottomMiddle,
                                   cBottomMiddle_Front,
                                   cBottomRight,
                                   cBottomRight_Front,
                                   cRightRight,
                                   cRightRight_Front,
                                   cTopRight,
                                   cTopRight_Front,
                                   cTopMiddle,
                                   cTopMiddle_Front,
                                   cTopLeft,
                                   cTopLeft_Front,
                                   bottomLeft,
                                   bottomRight,
                                   topRight,
                                   topLeft);
      }
    else {
      computeAverageHelper<false>(lightValue,
                                  cLeftLeft,
                                  cLeftLeft_Front,
                                  cBottomLeft,
                                  cBottomLeft_Front,
                                  cBottomMiddle,
                                  cBottomMiddle_Front,
                                  cBottomRight,
                                  cBottomRight_Front,
                                  cRightRight,
                                  cRightRight_Front,
                                  cTopRight,
                                  cTopRight_Front,
                                  cTopMiddle,
                                  cTopMiddle_Front,
                                  cTopLeft,
                                  cTopLeft_Front,
                                  bottomLeft,
                                  bottomRight,
                                  topRight,
                                  topLeft);
    }
  }

  template<bool sunLight>
  void computeAverageHelper(int lightValue,
                            const CubeFaceData& cLeftLeft,
                            const CubeFaceData& cLeftLeft_Opposite,
                            const CubeFaceData& cBottomLeft,
                            const CubeFaceData& cBottomLeft_Opposite,
                            const CubeFaceData& cBottomMiddle,
                            const CubeFaceData& cBottomMiddle_Opposite,
                            const CubeFaceData& cBottomRight,
                            const CubeFaceData& cBottomRight_Opposite,
                            const CubeFaceData& cRightRight,
                            const CubeFaceData& cRightRight_Opposite,
                            const CubeFaceData& cTopRight,
                            const CubeFaceData& cTopRight_Opposite,
                            const CubeFaceData& cTopMiddle,
                            const CubeFaceData& cTopMiddle_Opposite,
                            const CubeFaceData& cTopLeft,
                            const CubeFaceData& cTopLeft_Opposite,
                            float& bottomLeft,
                            float& bottomRight,
                            float& topRight,
                            float& topLeft) {

    using namespace config::cube_data;
    // ###########################################################################

    float counter{ 1 };
    float acc = lightValue;
    if (cLeftLeft.id != AIR && cLeftLeft_Opposite.id == AIR) {
      if
        constexpr(sunLight) {
          acc += cLeftLeft_Opposite.sunLightValue;
        }
      else {
        acc += cLeftLeft_Opposite.otherLightValue;
      }
      ++counter;
    }
    if (cBottomLeft.id != AIR && cBottomLeft_Opposite.id == AIR &&
        (cLeftLeft_Opposite.id == AIR || cBottomMiddle_Opposite.id == AIR)) {
      if
        constexpr(sunLight) {
          acc += cBottomLeft_Opposite.sunLightValue;
        }
      else {
        acc += cBottomLeft_Opposite.otherLightValue;
      }
      ++counter;
    }
    if (cBottomMiddle.id != AIR && cBottomMiddle_Opposite.id == AIR) {
      if
        constexpr(sunLight) {
          acc += cBottomMiddle_Opposite.sunLightValue;
        }
      else {
        acc += cBottomMiddle_Opposite.otherLightValue;
      }
      ++counter;
    }
    bottomLeft = acc / counter;

    // ###########################################################################

    counter = 1;
    acc = lightValue;
    if (cBottomMiddle.id != AIR && cBottomMiddle_Opposite.id == AIR) {
      if
        constexpr(sunLight) {
          acc += cBottomMiddle_Opposite.sunLightValue;
        }
      else {
        acc += cBottomMiddle_Opposite.otherLightValue;
      }
      ++counter;
    }
    if (cBottomRight.id != AIR && cBottomRight_Opposite.id == AIR &&
        (cBottomMiddle_Opposite.id == AIR || cRightRight_Opposite.id == AIR)) {
      if
        constexpr(sunLight) {
          acc += cBottomRight_Opposite.sunLightValue;
        }
      else {
        acc += cBottomRight_Opposite.otherLightValue;
      }
      ++counter;
    }
    if (cRightRight.id != AIR && cRightRight_Opposite.id == AIR) {
      if
        constexpr(sunLight) {
          acc += cRightRight_Opposite.sunLightValue;
        }
      else {
        acc += cRightRight_Opposite.otherLightValue;
      }
      ++counter;
    }
    bottomRight = acc / counter;

    // ###########################################################################

    counter = 1;
    acc = lightValue;
    if (cRightRight.id != AIR && cRightRight_Opposite.id == AIR) {
      if
        constexpr(sunLight) {
          acc += cRightRight_Opposite.sunLightValue;
        }
      else {
        acc += cRightRight_Opposite.otherLightValue;
      }
      ++counter;
    }
    if (cTopRight.id != AIR && cTopRight_Opposite.id == AIR &&
        (cRightRight_Opposite.id == AIR || cTopMiddle.id == AIR)) {
      if
        constexpr(sunLight) {
          acc += cTopRight_Opposite.sunLightValue;
        }
      else {
        acc += cTopRight_Opposite.otherLightValue;
      }
      ++counter;
    }
    if (cTopMiddle.id != AIR && cTopMiddle_Opposite.id == AIR) {
      if
        constexpr(sunLight) {
          acc += cTopMiddle_Opposite.sunLightValue;
        }
      else {
        acc += cTopMiddle_Opposite.otherLightValue;
      }
      ++counter;
    }
    topRight = acc / counter;

    // ###########################################################################

    counter = 1;
    acc = lightValue;
    if (cTopMiddle.id != AIR && cTopMiddle_Opposite.id == AIR) {
      if
        constexpr(sunLight) {
          acc += cTopMiddle_Opposite.sunLightValue;
        }
      else {
        acc += cTopMiddle_Opposite.otherLightValue;
      }
      ++counter;
    }
    if (cTopLeft.id != AIR && cTopLeft_Opposite.id == AIR &&
        (cTopMiddle_Opposite.id == AIR || cLeftLeft_Opposite.id == AIR)) {
      if
        constexpr(sunLight) {
          acc += cTopLeft_Opposite.sunLightValue;
        }
      else {
        acc += cTopLeft_Opposite.otherLightValue;
      }
      ++counter;
    }
    if (cLeftLeft.id != AIR && cLeftLeft_Opposite.id == AIR) {
      if
        constexpr(sunLight) {
          acc += cLeftLeft_Opposite.sunLightValue;
        }
      else {
        acc += cLeftLeft_Opposite.otherLightValue;
      }
      ++counter;
    }
    topLeft = acc / counter;
  }
  */

  std::unique_ptr<mesh::MeshElement> m_mesh;
  std::unique_ptr<mesh::MeshElement> m_waterMesh;

  bool m_hasTransparent{ true };

  double m_xLocation;
  double m_yLocation;
  double m_zLocation;

  int m_width = config::chunk_data::GRAPHICAL_CHUNK_WIDTH;
  int m_height = config::chunk_data::GRAPHICAL_CHUNK_HEIGHT;
  int m_depth = config::chunk_data::GRAPHICAL_CHUNK_WIDTH;

  Transform transform;

  Matrix& m_data;
  Matrix* m_right;
  Matrix* m_left;
  Matrix* m_back;
  Matrix* m_front;
};

} /* graphics namespace */

#endif /* SRC_GRAPHICS_GRAPHICALCHUNK_H_ */
