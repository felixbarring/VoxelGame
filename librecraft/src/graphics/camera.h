#ifndef SRC_GRAPHICS_CAMERA_H_
#define SRC_GRAPHICS_CAMERA_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../config/data.h"

namespace graphics {

/**
 * \brief This class is used to produce the view matrix used for rendering.
 *
 */
class Camera {
public:
// ########################################################
// Constructor/Destructor #################################
// ########################################################

    /**
     * \brief Creates a camera with position 0 for x, y, and z.
     */
    Camera();

    /**
     * \brief Creates a camera at the specified position.
     *
     * @param xPosition The x position of the camera.
     * @param yPosition The y position of the camera.
     * @param zPosition The z position of the camera.
     */
    Camera(float xPosition, float yPosition, float zPosition);

// ########################################################
// Member Functions########################################
// ########################################################


    // TODO Document this...

    /**
     *
     * @param view
     */
    void setViewMatrix(glm::mat4 value);

    /**
     *
     * @return
     */
    glm::mat4 getViewMatrix();

    /**
     *
     */
    void setProjectionMatrix(glm::mat4 value);

    /**
     *
     * @return
     */
    glm::mat4& getProjectionMatrix();

    /**
     *
     * @return
     */
    glm::vec3& getPosition();

    /**
     *
     * @param position
     * @param direction
     * @param up
     */
    void updateView(glm::vec3 position, glm::vec3 direction, glm::vec3 up);


    /**
     * \brief Sets the projection matrix to use the fov specified
     *
     * @param value The fov that should be used. The value shall be in degrees.
     */
    void setFov(float value);

// ########################################################
// Implementation #########################################
// ########################################################

private:

    glm::vec3 m_position;
    glm::vec3 m_direction;
    glm::vec3 m_up{0.0f, 1.0f, 0.0f};

    // TODO Update this in an intelligent way
    float m_aspectRatio = config::graphics_data::windowWidth / static_cast<float>(config::graphics_data::windowHeight);

    const float m_close{0.1f};
    const float m_far{1000.0f};

    glm::mat4 m_projection;
    glm::mat4 m_view;
};

}

#endif /* SRC_GRAPHICS_CAMERA_H_ */
