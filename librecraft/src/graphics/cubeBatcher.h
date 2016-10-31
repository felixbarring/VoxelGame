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

    static CubeBatcher& getInstance() {
        static CubeBatcher INSTANCE;
        return INSTANCE;
    }

// ########################################################
// Member Functions########################################
// ########################################################

    void addBatch(char type, Transform &transform, int lightValue = 16);

    void draw();

    /**
     *
     * @param value
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
