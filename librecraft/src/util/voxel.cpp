#include "voxel.h"

#include <iostream>
#include <string>

using std::__cxx11::to_string;

Voxel::Voxel() {
}

Voxel::Voxel(char id, char sun)
  : m_id{id} {
  setSunLightValue(sun);
}

Voxel::Voxel(char id, char sun, char other)
  : m_id{id} {
  setSunLightValue(sun);
  setOtherLightValue(other);
}

void
Voxel::setId(char value) {
  m_id = value;
}

char
Voxel::getId() {
  return m_id;
}

void
Voxel::setSunLightValue(char value) {
  assert(value <= config::graphics_data::directSunlightValue);
  m_lightValues = value;
}

char
Voxel::getSunLightValue() {
  return m_lightValues & 0x0F;
}

void
Voxel::setOtherLightValue(char value) {
  assert(value <= config::graphics_data::directSunlightValue);
  m_otherLight = value;
}

char
Voxel::getOtherLightValue() {
  return m_otherLight;
}
