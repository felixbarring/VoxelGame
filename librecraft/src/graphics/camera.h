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
    void setViewMatrix(glm::mat4 view);

    /**
     *
     * @return
     */
    glm::mat4 getViewMatrix();

    /**
     *
     */
    void setProjectionMatrix(glm::mat4 view);

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

// ########################################################
// Implementation #########################################
// ########################################################

private:

    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up {0.0f, 1.0f, 0.0f};

    // TODO Update this in an intelligent way
    float aspectRatio = config::graphics_data::windowWidth / static_cast<float>(config::graphics_data::windowHeight);

    glm::mat4 projection = glm::perspective(config::graphics_data::fov,	aspectRatio, 0.1f, 1000.0f);

    glm::mat4 view;
};

}

#endif /* SRC_GRAPHICS_CAMERA_H_ */
