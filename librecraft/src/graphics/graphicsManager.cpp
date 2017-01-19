
#include "graphicsManager.h"

#include "../graphics/resources.h"
#include "chunkBatcher.h"
#include "cubeBatcher.h"

using namespace std;

namespace graphics {

GraphicsManager::GraphicsManager()
{
    // TODO Fix skybox...

    texture::TextureCubeMap &texture =
            graphics::Resources::getInstance().getTextureCubeMap(
                    config::cube_map_data::cubeMap1[0],
                    config::cube_map_data::cubeMap1[1],
                    config::cube_map_data::cubeMap1[2],
                    config::cube_map_data::cubeMap1[3],
                    config::cube_map_data::cubeMap1[4],
                    config::cube_map_data::cubeMap1[5],
                    config::cube_map_data::cubeMap1Width,
                    config::cube_map_data::cubeMap1Height);

    m_skyBox = make_unique<CubeMap>(texture, m_playerCamera); // TODO should be some other camera...
}

void GraphicsManager::setSunStrenght(float value)
{
    m_chunkBatcher.setSunStrenght(value);
    m_cubeBatcher.setSunStrenght(value);
}

Camera& GraphicsManager::getPlayerCamer() {
    return m_playerCamera;
}

ChunkBatcher& GraphicsManager::getChunkBatcher() {
    return m_chunkBatcher;
}

CubeBatcher& GraphicsManager::getCubeBatcher() {
    return m_cubeBatcher;
}

SpriteBatcher& GraphicsManager::getSpriteBatcher() {
    return m_spriteBatcher;
}

CubeMap& GraphicsManager::getSkyMap() {
    return *m_skyBox;
}

} /* namespace graphics */
