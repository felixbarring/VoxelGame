
#ifndef SRC_GRAPHICS_GRAPHICSMANAGER_H_
#define SRC_GRAPHICS_GRAPHICSMANAGER_H_

#include "graphics/cubeMap.h"
#include "graphics/spriteBatcher.h"
#include "camera.h"
#include "chunkBatcher.h"
#include "cubeBatcher.h"

namespace graphics {

// TODO More detailed description
/**
 * @brief A class that is responsible of managing the graphics rendering.
 */
class GraphicsManager {
public:
  GraphicsManager();

  /**
   * @brief Sets the sun strength of the world.
   *
   * This will affect all graphical objects in different batches that are
   * dependent on sunlight.
   *
   * @param value The sun strength, in the range (0, 1) where 0 is no light,
   *              and 1 is max light.
   */
  void setSunStrength(float value);

  /**
   * @brief Returns the camera that is used to render the world from the
   *        players perspective.
   *
   * @return The camera uses to render the world from the players perspective.
   */
  Camera& getPlayerCamera();

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

  /**
   * @brief Clears the screen.
   *
   * Clears the screen with the default sky color.
   */
  void clearScreen();

  /**
   * @brief Clears the screen.
   *
   * Clears the screen with a sky color that is adjusted to depend on the
   * sun strength set by setSunStrenght. Lower sun strength means darker
   * clear color.
   */
  void clearScreenSunDependent();

private:
  Camera m_playerCamera{};

  ChunkBatcher m_chunkBatcher{m_playerCamera};
  CubeBatcher m_cubeBatcher{m_playerCamera};
  SpriteBatcher m_spriteBatcher;

  double m_sunStrength{};

  std::unique_ptr<graphics::CubeMap> m_skyBox;
};

} /* namespace graphics */

#endif /* SRC_GRAPHICS_GRAPHICSMANAGER_H_ */
