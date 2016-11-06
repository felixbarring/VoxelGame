#ifndef SRC_GRAPHICS_CUBEBATCHER_H_
#define SRC_GRAPHICS_CUBEBATCHER_H_

#include <vector>
#include <memory>

#include "texturedCube.h"
#include "camera.h"
#include "transform.h"

#include "shaderProgram.h"
#include "texture/textureArray.h"

namespace graphics {

/**
 * \brief This class is used to draw single cubes efficiently.
 *
 *  A cube can be added to the batcher by calling the addBatch function. The batch will be drawn the next time
 *  the draw function is called. The batch will be removed after the next draw call, hence the absence of a
 *  remove batch function. The sunstrenght can be set to affect how bright color the batches will have.
 *  */
class CubeBatcher {
private:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

    CubeBatcher();

    virtual ~CubeBatcher() { };

    CubeBatcher(CubeBatcher const&) = delete;

    void operator=(CubeBatcher const&) = delete;

public:

    /**
     * \brief Returns the single instance of this class
     *
     * This class uses the singleton pattern.
     *
     * @return The single instance of this class.
     */
    static CubeBatcher& getInstance() {
        static CubeBatcher INSTANCE;
        return INSTANCE;
    }

// ########################################################
// Member Functions########################################
// ########################################################

    /**
     * \brief Adds a cube to be drawn in the next draw call. The batch will only be drawn in the next draw call.
     *
     * @param type The type of cube that should be added to the drawing.
     * @param transform The transform used to transform the cube to the desired position.
     * @param lightValue The light value that will be used to set the color strenght of the cube.
     */
    void addBatch(char type, Transform &transform, int lightValue = 16);

    /**
     * \brief Draws all the batches that has been added.
     *
     * All batches will be cleared and not drawn again after calling this function.
     */
    void draw();

    /**
     * \brief Sets the sun strength that will affect how bright color the batches will have.
     *
     * @param value The value of the sun strength. The value should be in the range (0, 1) where 0 means no sunlight
     *        and 1 means max sunlight.
     */
    void setSunStrenght(float value);

// ########################################################
// Instance Variables #####################################
// ########################################################

private:

    class Batch {
    public:

        Batch(TexturedCube &cube, Transform &transform, int lightValue)
                : m_cube(cube), m_transform(transform),
                  m_lightValue(lightValue) {}

        TexturedCube &m_cube;
        Transform &m_transform;
        int m_lightValue;
    };

    std::vector<Batch> m_batches;
    std::vector<TexturedCube> m_cubes;

    float m_sunStrength{1.0};

    std::shared_ptr<ShaderProgram> m_program;
    texture::TextureArray &m_texture;

};

} /* namespace graphics */

#endif /* SRC_GRAPHICS_CUBEBATCHER_H_ */
