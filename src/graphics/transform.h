
#ifndef SRC_GRAPHICS_TRANSFORM_H_
#define SRC_GRAPHICS_TRANSFORM_H_

#include <glm/glm.hpp>

class Transform {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################
	Transform(float x, float y, float z);
	virtual ~Transform();

// ########################################################
// Member Functions########################################
// ########################################################

	void translate(glm::vec3 translation);

	void setRotationX(float value);

	void setRotationY(float value);

	void setRotationZ(float value);

	void rotateX(float value);

	void rotateY(float value);

	void rotateZ(float value);

	glm::mat4 getMatrix();

// ########################################################
// Instance Variables #####################################
// ########################################################
private:
	glm::mat4 translation;
	glm::mat4 xRotation;
	glm::mat4 yRotation;
	glm::mat4 zRotation;
};

#endif /* SRC_GRAPHICS_TRANSFORM_H_ */
