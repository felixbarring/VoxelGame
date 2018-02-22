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
  Voxel(unsigned char id, unsigned char sun);

  /**
   * @brief Creates a voxel with the specified id, sunlight, and otherlight.
   *
   * @param id The id value
   * @param sun The sunlight value
   * @param other The otherlight value
   */
  Voxel(unsigned char id, unsigned char sun, unsigned char other);

  /**
   * @brief Set the value of the id
   *
   * @param value The new value for the id
   */
  void setId(unsigned char value);

  /**
   *
   * @return The id value
   */
  unsigned char getId();

  /**
   * @brief Set the sunlight value
   *
   * @param value The new value for the sunlight, must be in the range of 0 to
   *              15
   */
  void setSunLightValue(unsigned char value);

  /**
   *
   * @return The sunlight value
   */
  unsigned char getSunLightValue();

  /**
   * @brief Set the other light value
   *
   * @param value The new value for the otherlight, must be in the range of 0
   *              to 15
   */
  void setOtherLightValue(unsigned char value);

  /**
   *
   * @return The otherlight value
   */
  unsigned char getOtherLightValue();

private:
  unsigned char m_id{};
  unsigned char m_lightValues{};
  // TODO Should only need one light value of 8 bits using bitmasking
  // and shifting.
  unsigned char m_otherLight{};
};

#endif /* SRC_UTIL_VOXEL_H_ */
