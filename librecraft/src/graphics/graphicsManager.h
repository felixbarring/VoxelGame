
#ifndef SRC_GRAPHICS_GRAPHICSMANAGER_H_
#define SRC_GRAPHICS_GRAPHICSMANAGER_H_

namespace graphics {

// TODO More detailed description
/**
 * \brief A class that is responsible of managing the graphics rendering.
 *
 */
class GraphicsManager {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

    GraphicsManager() {};

    GraphicsManager(GraphicsManager const&) = delete;

    void operator=(GraphicsManager const&) = delete;

public:

// ########################################################
// Member Functions########################################
// ########################################################

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

// ########################################################
// Implementation #########################################
// ########################################################

private:

};

} /* namespace graphics */

#endif /* SRC_GRAPHICS_GRAPHICSMANAGER_H_ */
