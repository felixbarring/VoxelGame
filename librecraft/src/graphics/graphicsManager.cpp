
#include "graphicsManager.h"

#include "../graphics/resources.h"
#include "chunkBatcher.h"
#include "cubeBatcher.h"

using namespace std;

namespace graphics {

GraphicsManager::GraphicsManager()
{
  texture::TextureCubeMap& texture =
    graphics::Resources::getInstance().getTextureCubeMap(
      config::cube_map_data::cubeMap1[0],
      config::cube_map_data::cubeMap1[1],
      config::cube_map_data::cubeMap1[2],
      config::cube_map_data::cubeMap1[3],
      config::cube_map_data::cubeMap1[4],
      config::cube_map_data::cubeMap1[5]);

  m_skyBox = make_unique<CubeMap>(texture, m_playerCamera);
}

void
GraphicsManager::setSunStrength(float value)
{
  m_chunkBatcher.setSunStrenght(value);
  m_cubeBatcher.setSunStrenght(value);
  m_sunStrength = value;
}

Camera&
GraphicsManager::getPlayerCamera()
{
  return m_playerCamera;
}

ChunkBatcher&
GraphicsManager::getChunkBatcher()
{
  return m_chunkBatcher;
}

CubeBatcher&
GraphicsManager::getCubeBatcher()
{
  return m_cubeBatcher;
}

SpriteBatcher&
GraphicsManager::getSpriteBatcher()
{
  return m_spriteBatcher;
}

CubeMap&
GraphicsManager::getSkyMap()
{
  return *m_skyBox;
}

void
GraphicsManager::clearScreen()
{
  glm::vec3 skyColor = config::graphics_data::skyColor;
  glClearColor(skyColor.x, skyColor.y, skyColor.z, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void
GraphicsManager::clearScreenSunDependent()
{
  glm::vec3 skyColor{ config::graphics_data::skyColor };
  glm::vec3 dark{ 0, 0, 0 };
  skyColor = glm::mix(dark, skyColor, m_sunStrength);
  glClearColor(skyColor.x, skyColor.y, skyColor.z, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

} /* namespace graphics */
