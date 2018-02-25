#include "voxel.h"

#include <iostream>
#include <string>

#include "../docTestDefine.h"
#include "docTest/doctest.h"

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
  m_lightValues = (value & 0x0F) | (m_lightValues & 0xF0);
}

char
Voxel::getSunLightValue() {
  return m_lightValues & 0x0F;
}

void
Voxel::setOtherLightValue(char value) {
  m_lightValues = ((value << 4) & 0xF0) | (m_lightValues & 0x0F);
}

char
Voxel::getOtherLightValue() {
  return (m_lightValues & 0xF0) >> 4;
}

TEST_CASE("Testing default values") {
  Voxel voxel{};
  CHECK(voxel.getId() == 0);
  CHECK(voxel.getSunLightValue() == 0);
  CHECK(voxel.getOtherLightValue() == 0);
}

TEST_CASE("Testing supplied arguments") {
  {
    Voxel voxel{0, 0, 0};
    CHECK(voxel.getId() == 0);
    CHECK(voxel.getSunLightValue() == 0);
    CHECK(voxel.getOtherLightValue() == 0);
  }
  {
    Voxel voxel{1, 2, 3};
    CHECK(voxel.getId() == 1);
    CHECK(voxel.getSunLightValue() == 2);
    CHECK(voxel.getOtherLightValue() == 3);
  }
}

TEST_CASE("Testing id values") {
  Voxel voxel{0, 0, 0};
  for (int i{0}; i < 256; ++i) {
    voxel.setId(i);
    CHECK(static_cast<unsigned char>(voxel.getId()) == i);
  }
}

TEST_CASE("Testing light values") {
  Voxel voxel{0, 0, 0};

  for (int i{0}; i <= 15; ++i) {
    voxel.setSunLightValue(i);
    CHECK(voxel.getSunLightValue() == i);
  }

  for (int i{0}; i <= 15; ++i) {
    voxel.setOtherLightValue(i);
    CHECK(voxel.getOtherLightValue() == i);
  }
}
