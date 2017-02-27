
#ifndef SRC_GRAPHICS_GRAPHICSMANAGER_H_
#define SRC_GRAPHICS_GRAPHICSMANAGER_H_

#include "../graphics/cubeMap.h"
#include "../graphics/spriteBatcher.h"
#include "camera.h"
#include "chunkBatcher.h"
#include "cubeBatcher.h"

namespace graphics {

// TODO More detailed description
/**
 * \brief A class that is responsible of managing the graphics rendering.
 *
 */
class GraphicsManager {
private:

    GraphicsManager();

    GraphicsManager(GraphicsManager const&) = delete;

    void operator=(GraphicsManager const&) = delete;

public:

    /**
     * \brief Returns the single instance of this class
     *
     * This class uses the singleton pattern.
     *
     * @return The single instance of this class.
     */
    static GraphicsManager& getInstance() {
        static GraphicsManager INSTANCE;
        return INSTANCE;
    }

    /**
     * \brief Sets the sun strength of the world.
     *
     * This will affect all graphical objects in different batches that are dependent on sunlight.
     *
     * @param value The sun strength, in the range (0, 1) where 0 is no light, and 1 is max light.
     */
    void setSunStrenght(float value);

    /**
     * \brief Returns the camera that is used to render the world from the players perspective.
     *
     * @return The camera uses to render the world from the players perspective.
     */
    Camera& getPlayerCamer();

    /**
     *
     * @return
     */
    ChunkBatcher& getChunkBatcher();

    /**
     *
     * @return
     */
    CubeBatcher& getCubeBatcher();

    /**
     *
     * @return
     */
    SpriteBatcher& getSpriteBatcher();

    /**
     *
     * @return
     */
    CubeMap& getSkyMap();

private:
    Camera m_playerCamera{};
    Camera m_skyMapCamera{};

    ChunkBatcher m_chunkBatcher{m_playerCamera};
    CubeBatcher m_cubeBatcher{m_playerCamera};
    SpriteBatcher m_spriteBatcher;

    std::unique_ptr<graphics::CubeMap> m_skyBox;

};

} /* namespace graphics */

#endif /* SRC_GRAPHICS_GRAPHICSMANAGER_H_ */
