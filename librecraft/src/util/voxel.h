#ifndef SRC_UTIL_VOXEL_H_
#define SRC_UTIL_VOXEL_H_

#include <cassert>

#include "../config/data.h"

/**
 * @brief Storage class with all data needed to represent a cube.
 *
 * Class that stores the data values that are needed for a cube. These values
 * are ID of the cube and the light values which are used to lightning
 * computations.
 */
class Voxel {
public:
  /**
   * @brief Create a default voxel. All values will be zero-initialized.
   */
  Voxel();

  /**
   * @brief Creates a voxel with the specified id and sunlight. The value for
   * otherlight will be zero_intialized.
   *
   * @param id The id value
   * @param sun The sunlight value
   */
  Voxel(char id, char sun);

  /**
   * @brief Creates a voxel with the specified id, sunlight, and otherlight.
   *
   * @param id The id value
   * @param sun The sunlight value
   * @param other The otherlight value
   */
  Voxel(char id, char sun, char other);

  /**
   * @brief Set the value of the id
   *
   * @param value The new value for the id
   */
  void setId(char value);

  /**
   *
   * @return The id value
   */
  char getId();

  /**
   * @brief Set the sunlight value
   *
   * @param value The new value for the sunlight, must be in the range of 0 to
   *              15
   */
  void setSunLightValue(char value);

  /**
   *
   * @return The sunlight value
   */
  char getSunLightValue();

  /**
   * @brief Set the other light value
   *
   * @param value The new value for the otherlight, must be in the range of 0
   *              to 15
   */
  void setOtherLightValue(char value);

  /**
   *
   * @return The otherlight value
   */
  char getOtherLightValue();

private:
  char m_id{};
  char m_lightValues{};
};

#endif /* SRC_UTIL_VOXEL_H_ */
