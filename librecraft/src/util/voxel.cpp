#include "voxel.h"

#include <iostream>
#include <string>

#include "../docTestDefine.h"
#include "docTest/doctest.h"

Voxel::Voxel() {
}

Voxel::Voxel(unsigned char id, unsigned char sun)
  : m_id{id} {
  setSunLightValue(sun);
}

Voxel::Voxel(unsigned char id, unsigned char sun, unsigned char other)
  : m_id{id} {
  setSunLightValue(sun);
  setOtherLightValue(other);
}

void
Voxel::setId(unsigned char value) {
  m_id = value;
}

unsigned char
Voxel::getId() {
  return m_id;
}

void
Voxel::setSunLightValue(unsigned char value) {
  m_lightValues = value;
}

unsigned char
Voxel::getSunLightValue() {
  return m_lightValues & 0x0F;
}

void
Voxel::setOtherLightValue(unsigned char value) {
  m_otherLight = value;
}

unsigned char
Voxel::getOtherLightValue() {
  return m_otherLight;
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
    CHECK(voxel.getId() == i);
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

