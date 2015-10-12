
#include "transform.h"

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

namespace graphics
{

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Transform::Transform(float x, float y, float z)
{
	translation = glm::translate(glm::mat4(1.f), glm::vec3(x, y, z));

	setRotationX(0);
	setRotationY(0);
	setRotationZ(0);
}

// ########################################################
// Member Functions########################################
// ########################################################

void Transform::translate(glm::vec3 value)
{
	translation = glm::translate(translation, value);
}

void Transform::translate(float x, float y, float z)
{
	translation = glm::translate(translation, glm::vec3(x, y, z));
}

void Transform::setLocation(float x, float y, float z)
{
	translation = glm::translate(glm::mat4(1.f), glm::vec3(x, y, z));;
}

void Transform::setRotationX(float value)
{
	xRotation = glm::rotate(glm::mat4(1.0f), value, glm::vec3(1, 0, 0));
}

void Transform::setRotationY(float value)
{
	yRotation = glm::rotate(glm::mat4(1.0f), value, glm::vec3(0, 1, 0));
}

void Transform::setRotationZ(float value)
{
	zRotation = glm::rotate(glm::mat4(1.0f),value, glm::vec3(0, 0, 1));
}

void Transform::rotateX(float value)
{
	xRotation = glm::rotate(xRotation, value, glm::vec3(1, 0, 0));
}

void Transform::rotateY(float value)
{
	yRotation = glm::rotate(yRotation, value, glm::vec3(0, 1, 0));
}

void Transform::rotateZ(float value)
{
	zRotation = glm::rotate(zRotation, value, glm::vec3(0, 0, 1));
}

glm::mat4 Transform::getMatrix()
{
	return translation * xRotation * yRotation * zRotation;
}

}

