#ifndef SRC_UTIL_VOXEL_H_
#define SRC_UTIL_VOXEL_H_

#include <cassert>
#include <iostream>
#include <string>

#include "../config/data.h"

/**
 *
 */
class Voxel {
public:

  Voxel() {
  }

  Voxel(char id, char sun)
    : m_id{ id } {
    setSunLightValue(sun);
  }

  Voxel(char id, char sun, char other)
    : m_id{ id } {
    setSunLightValue(sun);
    setOtherLightValue(other);
  }

  void setId(char value) {
    m_id = value;
  }

  char getId() {
    return m_id;
  }

  void setSunLightValue(char value) {
    assert(value <= config::graphics_data::directSunlightValue);
  //    m_lightValues |= (value & sunBits);
    m_lightValues = value;
  }

  char getSunLightValue() {
    return m_lightValues; // & sunBits;
  }

  void setOtherLightValue(char value) {
    assert(value <= config::graphics_data::directSunlightValue);
  //    m_lightValues |= (value << 4);
    m_otherLight = value;
  }

  char getOtherLightValue() {
    using std::__cxx11::to_string;
//    std::cout << "Other light value = " + to_string(m_lightValues >> 4) + "\n";
//    std::cout << "Other light value anded= " + to_string((m_lightValues >> 4) & sunBits) + "\n";
    return m_otherLight; //(m_lightValues >> 4) & sunBits;
  }

private:
  static const char sunBits = 0x0F;

  char m_id{};
  unsigned char m_lightValues{}; // Both sun and other.
  unsigned char m_otherLight{};
};

#endif /* SRC_UTIL_VOXEL_H_ */
