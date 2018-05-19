#ifndef SRC_GRAPHICS_GRAPHICALCHUNK_H_
#define SRC_GRAPHICS_GRAPHICALCHUNK_H_

#include <memory>
#include <vector>

#include "mesh/meshElement.h"
#include "transform.h"

#include "config/data.h"
#include "util/voxel.h"

namespace graphics {

using Matrix = std::vector<std::vector<std::vector<Voxel>>>;

/**
 * @brief Creates a graphical representation of a chunk.
 *
 * This can then be rendered with the two functions drawNoneTransparent and
 * drawTransparent. After calling the constructor, the uploadData functions
 * must be called.
 */
class GraphicalChunk {
public:
  /**
   * @brief Creates an instance of the GraphicalChunk.
   *
   * It will create data that should be uploaded with uploadData. This
   * constructor only reads from the parameters and it is thus safe to run this
   * in parallel as long as the provided arguments are not changed by some other
   * thread. The work done for creating the mesh data can be considered to be
   * big, so its a good idea to run it in parallel.
   *
   * @param x The x location of the chunk.
   * @param y The y location of the chunk.
   * @param z The z location of the chunk.
   * @param data The data for the chunk that this GraphicalChunk represents.
   * @param right Data for the right neighbor, if no neighbor use nullptr.
   * @param left Data for the left neighbor, if no neighbor use nullptr.
   * @param back Data for the back neighbor, if no neighbor use nullptr.
   * @param front Data for the front neighbor, if no neighbor use nullptr.
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
   * @brief Uploads the mesh data to the Graphics Card.
   *
   * Uploads the data for the meshes, this data was created by the constructor.
   * This function must be called from the main thread that has the opengl
   * context. Forgetting to call this function before a draw is an error. Should
   * only be done once.
   */
  void uploadData();

  /**
   * @brief Draws the mesh with none transparent faces.
   */
  void drawNoneTransparent();

  /**
   * @brief Draws the mesh with transparent faces such as water.
   */
  void drawTransparent();

  /**
   * @return If there is any transparent part to render.
   */
  bool hasTransparent();

  /**
   *
   * @return The transform for this GraphicalChunk, should be used to crate
   *         the correct transform matrixes.
   */
  Transform& getTransform();

  /**
   *
   * @return Returns the x location of the GraphicalChunk.
   */
  float getxLocation();

  /**
   *
   * @return Returns the y location of the GraphicalChunk.
   */
  float getyLocation();

  /**
   *
   * @return Returns the z location of the GraphicalChunk.
   */
  float getzLocation();

private:
  class MeshData {
  public:
    std::vector<GLfloat> vertexData{};
    std::vector<GLfloat> lightData{};
    std::vector<GLfloat> normals{};
    std::vector<GLfloat> UV{};
    std::vector<short> elementData{};
  };

  MeshData createMeshData(bool transparent);

  class Lights {
  public:
    Lights(float _bottomLeft,
           float _bottomRight,
           float _topRight,
           float _topLeft)
      : bottomLeft{_bottomLeft}
      , bottomRight{_bottomRight}
      , topRight{_topRight}
      , topLeft{_topLeft} {
    }

    float bottomLeft{};
    float bottomRight{};
    float topRight{};
    float topLeft{};
  };

  void doAORight(int x, int y, int z, Lights& lights);

  void doAOLeft(int x, int y, int z, Lights& lights);

  void doAOFront(int x, int y, int z, Lights& lights);

  void doAOBack(int x, int y, int z, Lights& lights);

  void doAOTop(int x, int y, int z, Lights& lights);

  void doAOBottom(int x, int y, int z, Lights& lights);

  Voxel* getVoxel(int x, int y, int z);

  // clang-format off
  template<bool sunLight>
  void computeAverageRight(
    int lightValue,
    int x,
    int y,
    int z,
    float& bottomLeft,
    float& bottomRight,
    float& topRight,
    float& topLeft) {

    Voxel* cBottomLeft = getVoxel(x, y - 1, z + 1);
    Voxel* cBottomLeft_Right = getVoxel(x + 1, y - 1,z + 1);

    Voxel* cBottomMiddle = getVoxel(x, y - 1, z);
    Voxel* cBottomMiddle_Right = getVoxel(x + 1, y - 1, z);

    Voxel* cBottomRight = getVoxel(x, y - 1, z - 1);
    Voxel* cBottomRight_Right = getVoxel(x + 1, y - 1, z - 1);

    Voxel* cRightRight = getVoxel(x, y, z - 1);
    Voxel* cRightRight_Right = getVoxel(x + 1, y, z - 1);

    Voxel* cTopRight = getVoxel(x, y + 1, z - 1);
    Voxel* cTopRight_Right = getVoxel(x + 1, y + 1, z - 1);

    Voxel* cTopMiddle = getVoxel(x, y + 1, z);
    Voxel* cTopMiddle_Right = getVoxel(x + 1, y + 1, z);

    Voxel* cTopLeft = getVoxel(x, y + 1, z + 1);
    Voxel* cTopLeft_Right = getVoxel(x + 1, y + 1, z + 1);

    Voxel* cLeftLeft = getVoxel(x, y, z + 1);
    Voxel* cLeftLeft_Right = getVoxel(x + 1, y, z + 1);

    computeAverageHelper<sunLight>(lightValue,
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

  template<bool sunLight>
  void computeAverageLeft(
    int lightValue,
    int x,
    int y,
    int z,
    float& bottomLeft,
    float& bottomRight,
    float& topRight,
    float& topLeft) {

    Voxel* cBottomLeft = getVoxel(x, y - 1, z - 1);
    Voxel* cBottomLeft_Left = getVoxel(x - 1, y - 1, z - 1);

    Voxel* cBottomMiddle = getVoxel(x, y - 1, z);
    Voxel* cBottomMiddle_Left = getVoxel(x - 1, y - 1, z);

    Voxel* cBottomRight = getVoxel(x, y - 1, z + 1);
    Voxel* cBottomRight_Left = getVoxel(x - 1, y - 1, z + 1);

    Voxel* cRightRight = getVoxel(x, y, z + 1);
    Voxel* cRightRight_Left = getVoxel(x - 1, y, z + 1);

    Voxel* cTopRight = getVoxel(x, y + 1, z + 1);
    Voxel* cTopRight_Left = getVoxel(x - 1, y + 1, z + 1);

    Voxel* cTopMiddle = getVoxel(x, y + 1, z);
    Voxel* cTopMiddle_Left = getVoxel(x - 1, y + 1, z);

    Voxel* cTopLeft = getVoxel(x, y + 1, z - 1);
    Voxel* cTopLeft_Left = getVoxel(x - 1, y + 1, z - 1);

    Voxel* cLeftLeft = getVoxel(x, y, z - 1);
    Voxel* cLeftLeft_Left = getVoxel(x - 1, y, z - 1);

    computeAverageHelper<sunLight>(lightValue,
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

  template<bool sunLight>
  void computeAverageTop(
    int lightValue,
    int x,
    int y,
    int z,
    float& bottomLeft,
    float& bottomRight,
    float& topRight,
    float& topLeft) {

    Voxel* cBottomLeft = getVoxel(x - 1, y, z + 1);
    Voxel* cBottomLeft_Top = getVoxel(x - 1, y + 1, z + 1);

    Voxel* cBottomMiddle = getVoxel(x, y, z + 1);
    Voxel* cBottomMiddle_Top = getVoxel(x, y + 1, z + 1);

    Voxel* cBottomRight = getVoxel(x + 1, y, z + 1);
    Voxel* cBottomRight_Top = getVoxel(x + 1, y + 1, z + 1);

    Voxel* cRightRight = getVoxel(x + 1, y, z);
    Voxel* cRightRight_Top = getVoxel(x + 1, y + 1, z);

    Voxel* cTopRight = getVoxel(x + 1, y, z - 1);
    Voxel* cTopRight_Top = getVoxel(x + 1, y + 1, z - 1);

    Voxel* cTopMiddle = getVoxel(x, y, z - 1);
    Voxel* cTopMiddle_Top = getVoxel(x, y + 1, z - 1);

    Voxel* cTopLeft = getVoxel(x - 1, y, z - 1);
    Voxel* cTopLeft_Top = getVoxel(x - 1, y + 1, z - 1);

    Voxel* cLeftLeft = getVoxel(x - 1, y, z);
    Voxel* cLeftLeft_Top = getVoxel(x - 1, y + 1, z);

    computeAverageHelper<sunLight>(lightValue,
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

  template<bool sunLight>
  void computeAverageBottom(
    int lightValue,
    int x,
    int y,
    int z,
    float& bottomLeft,
    float& bottomRight,
    float& topRight,
    float& topLeft) {

    Voxel* cBottomLeft = getVoxel(x - 1, y, z - 1);
    Voxel* cBottomLeft_Bottom = getVoxel(x - 1, y - 1, z - 1);

    Voxel* cBottomMiddle = getVoxel(x, y, z - 1);
    Voxel* cBottomMiddle_Bottom = getVoxel(x, y - 1, z - 1);

    Voxel* cBottomRight = getVoxel(x + 1, y, z - 1);
    Voxel* cBottomRight_Bottom = getVoxel(x + 1, y - 1, z - 1);

    Voxel* cRightRight = getVoxel(x + 1, y, z);
    Voxel* cRightRight_Bottom = getVoxel(x + 1, y - 1, z);

    Voxel* cTopRight = getVoxel(x + 1, y, z + 1);
    Voxel* cTopRight_Bottom = getVoxel(x + 1, y - 1, z + 1);

    Voxel* cTopMiddle = getVoxel(x, y, z + 1);
    Voxel* cTopMiddle_Bottom = getVoxel(x, y - 1,z + 1);

    Voxel* cTopLeft = getVoxel(x - 1, y, z + 1);
    Voxel* cTopLeft_Bottom = getVoxel(x - 1, y - 1, z + 1);

    Voxel* cLeftLeft = getVoxel(x - 1, y, z);
    Voxel* cLeftLeft_Bottom = getVoxel(x - 1, y - 1, z);

    computeAverageHelper<sunLight>(lightValue,
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

  template<bool sunLight>
  void computeAverageBack(
    int lightValue,
    int x,
    int y,
    int z,
    float& bottomLeft,
    float& bottomRight,
    float& topRight,
    float& topLeft) {

    Voxel* cBottomLeft = getVoxel(x + 1, y - 1, z);
    Voxel* cBottomLeft_Back = getVoxel(x + 1, y - 1, z - 1);

    Voxel* cBottomMiddle = getVoxel(x, y - 1, z);
    Voxel* cBottomMiddle_Back = getVoxel(x, y - 1, z - 1);

    Voxel* cBottomRight = getVoxel(x - 1, y - 1, z);
    Voxel* cBottomRight_Back = getVoxel(x - 1, y - 1, z - 1);

    Voxel* cRightRight = getVoxel(x - 1, y, z);
    Voxel* cRightRight_Back = getVoxel(x - 1, y, z - 1);

    Voxel* cTopRight = getVoxel(x - 1, y + 1, z);
    Voxel* cTopRight_Back = getVoxel(x - 1, y + 1, z - 1);

    Voxel* cTopMiddle = getVoxel(x, y + 1, z);
    Voxel* cTopMiddle_Back = getVoxel(x, y + 1, z - 1);

    Voxel* cTopLeft = getVoxel(x + 1, y + 1, z);
    Voxel* cTopLeft_Back = getVoxel(x + 1, y + 1, z - 1);

    Voxel* cLeftLeft = getVoxel(x + 1, y, z);
    Voxel* cLeftLeft_Back = getVoxel(x + 1, y, z - 1);

    computeAverageHelper<sunLight>(lightValue,
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

  template<bool sunLight>
  void computeAverageFront(
    int lightValue,
    int x,
    int y,
    int z,
    float& bottomLeft,
    float& bottomRight,
    float& topRight,
    float& topLeft) {

    Voxel* cBottomLeft = getVoxel(x - 1, y - 1, z);
    Voxel* cBottomLeft_Front = getVoxel(x - 1, y - 1, z + 1);

    Voxel* cBottomMiddle = getVoxel(x, y - 1, z);
    Voxel* cBottomMiddle_Front = getVoxel(x, y - 1, z + 1);

    Voxel* cBottomRight = getVoxel(x + 1, y - 1, z);
    Voxel* cBottomRight_Front = getVoxel(x + 1, y - 1, z + 1);

    Voxel* cRightRight = getVoxel(x + 1, y, z);
    Voxel* cRightRight_Front = getVoxel(x + 1, y, z + 1);

    Voxel* cTopRight = getVoxel(x + 1, y + 1, z);
    Voxel* cTopRight_Front = getVoxel(x + 1, y + 1, z + 1);

    Voxel* cTopMiddle = getVoxel(x, y + 1, z);
    Voxel* cTopMiddle_Front = getVoxel(x, y + 1, z + 1);

    Voxel* cTopLeft = getVoxel(x - 1, y + 1, z);
    Voxel* cTopLeft_Front = getVoxel(x - 1, y + 1, z + 1);

    Voxel* cLeftLeft = getVoxel(x - 1, y, z);
    Voxel* cLeftLeft_Front = getVoxel(x - 1, y, z + 1);

    computeAverageHelper<sunLight>(lightValue,
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

  template<bool sunLight>
  void computeAverageHelper(int lightValue,
                            Voxel* cLeftLeft,
                            Voxel* cLeftLeft_Opposite,
                            Voxel* cBottomLeft,
                            Voxel* cBottomLeft_Opposite,
                            Voxel* cBottomMiddle,
                            Voxel* cBottomMiddle_Opposite,
                            Voxel* cBottomRight,
                            Voxel* cBottomRight_Opposite,
                            Voxel* cRightRight,
                            Voxel* cRightRight_Opposite,
                            Voxel* cTopRight,
                            Voxel* cTopRight_Opposite,
                            Voxel* cTopMiddle,
                            Voxel* cTopMiddle_Opposite,
                            Voxel* cTopLeft,
                            Voxel* cTopLeft_Opposite,
                            float& bottomLeft,
                            float& bottomRight,
                            float& topRight,
                            float& topLeft) {

    using namespace config::cube_data;
    // ###########################################################################

    float counter{ 1 };
    float acc = lightValue;
    if (cLeftLeft && cLeftLeft_Opposite && cLeftLeft->getId() != AIR && cLeftLeft_Opposite->getId() == AIR) {
      if
        constexpr(sunLight) {
          acc += cLeftLeft_Opposite->getSunLightValue();
        }
      else {
        acc += cLeftLeft_Opposite->getOtherLightValue();
      }
      ++counter;
    }
    if (cBottomLeft && cBottomLeft_Opposite && cBottomLeft->getId() != AIR && cBottomLeft_Opposite->getId() == AIR &&
        (cLeftLeft_Opposite->getId() == AIR || cBottomMiddle_Opposite->getId() == AIR)) {
      if
        constexpr(sunLight) {
          acc += cBottomLeft_Opposite->getSunLightValue();
        }
      else {
        acc += cBottomLeft_Opposite->getOtherLightValue();
      }
      ++counter;
    }
    if (cBottomMiddle && cBottomMiddle_Opposite && cBottomMiddle->getId() != AIR && cBottomMiddle_Opposite->getId() == AIR) {
      if
        constexpr(sunLight) {
          acc += cBottomMiddle_Opposite->getSunLightValue();
        }
      else {
        acc += cBottomMiddle_Opposite->getOtherLightValue();
      }
      ++counter;
    }
    bottomLeft = acc / counter;

    // ###########################################################################

    counter = 1;
    acc = lightValue;
    if (cBottomMiddle && cBottomMiddle_Opposite && cBottomMiddle->getId() != AIR && cBottomMiddle_Opposite->getId() == AIR) {
      if
        constexpr(sunLight) {
          acc += cBottomMiddle_Opposite->getSunLightValue();
        }
      else {
        acc += cBottomMiddle_Opposite->getOtherLightValue();
      }
      ++counter;
    }
    if (cBottomRight && cBottomRight_Opposite && cBottomRight->getId() != AIR && cBottomRight_Opposite->getId() == AIR &&
        (cBottomMiddle_Opposite->getId() == AIR || cRightRight_Opposite->getId() == AIR)) {
      if
        constexpr(sunLight) {
          acc += cBottomRight_Opposite->getSunLightValue();
        }
      else {
        acc += cBottomRight_Opposite->getOtherLightValue();
      }
      ++counter;
    }
    if (cRightRight && cRightRight_Opposite && cRightRight->getId() != AIR && cRightRight_Opposite->getId() == AIR) {
      if
        constexpr(sunLight) {
          acc += cRightRight_Opposite->getSunLightValue();
        }
      else {
        acc += cRightRight_Opposite->getOtherLightValue();
      }
      ++counter;
    }
    bottomRight = acc / counter;

    // ###########################################################################

    counter = 1;
    acc = lightValue;
    if (cRightRight && cRightRight_Opposite && cRightRight->getId() != AIR && cRightRight_Opposite->getId() == AIR) {
      if
        constexpr(sunLight) {
          acc += cRightRight_Opposite->getSunLightValue();
        }
      else {
        acc += cRightRight_Opposite->getOtherLightValue();
      }
      ++counter;
    }
    if (cTopRight && cTopRight_Opposite && cTopRight->getId() != AIR && cTopRight_Opposite->getId() == AIR &&
        (cRightRight_Opposite->getId() == AIR || cTopMiddle->getId() == AIR)) {
      if
        constexpr(sunLight) {
          acc += cTopRight_Opposite->getSunLightValue();
        }
      else {
        acc += cTopRight_Opposite->getOtherLightValue();
      }
      ++counter;
    }
    if (cTopMiddle && cTopMiddle_Opposite && cTopMiddle->getId() != AIR && cTopMiddle_Opposite->getId() == AIR) {
      if
        constexpr(sunLight) {
          acc += cTopMiddle_Opposite->getSunLightValue();
        }
      else {
        acc += cTopMiddle_Opposite->getOtherLightValue();
      }
      ++counter;
    }
    topRight = acc / counter;

    // ###########################################################################

    counter = 1;
    acc = lightValue;
    if (cTopMiddle && cTopMiddle_Opposite && cTopMiddle->getId() != AIR && cTopMiddle_Opposite->getId() == AIR) {
      if
        constexpr(sunLight) {
          acc += cTopMiddle_Opposite->getSunLightValue();
        }
      else {
        acc += cTopMiddle_Opposite->getOtherLightValue();
      }
      ++counter;
    }
    if (cTopLeft && cTopLeft_Opposite && cTopLeft->getId() != AIR && cTopLeft_Opposite->getId() == AIR &&
        (cTopMiddle_Opposite->getId() == AIR || cLeftLeft_Opposite->getId() == AIR)) {
      if
        constexpr(sunLight) {
          acc += cTopLeft_Opposite->getSunLightValue();
        }
      else {
        acc += cTopLeft_Opposite->getOtherLightValue();
      }
      ++counter;
    }
    if (cLeftLeft && cLeftLeft_Opposite && cLeftLeft->getId() != AIR && cLeftLeft_Opposite->getId() == AIR) {
      if
        constexpr(sunLight) {
          acc += cLeftLeft_Opposite->getSunLightValue();
        }
      else {
        acc += cLeftLeft_Opposite->getOtherLightValue();
      }
      ++counter;
    }
    topLeft = acc / counter;
  }
  // clang-format on

  static const int meshData{0};
  static const int wateterMeshData{1};
  std::vector<MeshData> m_meshData{};

  std::unique_ptr<mesh::MeshElement> m_mesh;
  std::unique_ptr<mesh::MeshElement> m_waterMesh;

  bool m_hasTransparent{true};

  double m_xLocation;
  double m_yLocation;
  double m_zLocation;

  const int m_width{config::chunk_data::GRAPHICAL_CHUNK_WIDTH};
  const int m_height{config::chunk_data::GRAPHICAL_CHUNK_HEIGHT};
  const int m_depth{config::chunk_data::GRAPHICAL_CHUNK_WIDTH};

  Transform transform;

  Matrix& m_data;
  Matrix* m_right;
  Matrix* m_left;
  Matrix* m_back;
  Matrix* m_front;
};

} /* graphics namespace */

#endif /* SRC_GRAPHICS_GRAPHICALCHUNK_H_ */
