
#include "transform.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace graphics {

Transform::Transform(double x, double y, double z)
{
  translation = glm::translate(glm::mat4(1.f), glm::vec3(x, y, z));

  setRotationX(0);
  setRotationY(0);
  setRotationZ(0);
}

void
Transform::translate(glm::vec3 value)
{
  translation = glm::translate(translation, value);
}

void
Transform::translate(double x, double y, double z)
{
  translation = glm::translate(translation, glm::vec3(x, y, z));
}

void
Transform::setLocation(double x, double y, double z)
{
  translation = glm::translate(glm::mat4(1.f), glm::vec3(x, y, z));
}

void
Transform::setRotationX(double value)
{
  xRotation =
    glm::rotate(glm::mat4(1.0f), static_cast<float>(value), glm::vec3(1, 0, 0));
}

void
Transform::setRotationY(double value)
{
  yRotation =
    glm::rotate(glm::mat4(1.0f), static_cast<float>(value), glm::vec3(0, 1, 0));
}

void
Transform::setRotationZ(double value)
{
  zRotation =
    glm::rotate(glm::mat4(1.0f), static_cast<float>(value), glm::vec3(0, 0, 1));
}

void
Transform::rotateX(double value)
{
  xRotation =
    glm::rotate(xRotation, static_cast<float>(value), glm::vec3(1, 0, 0));
}

void
Transform::rotateY(double value)
{
  yRotation =
    glm::rotate(yRotation, static_cast<float>(value), glm::vec3(0, 1, 0));
}

void
Transform::rotateZ(double value)
{
  zRotation =
    glm::rotate(zRotation, static_cast<float>(value), glm::vec3(0, 0, 1));
}

glm::mat4
Transform::getMatrix()
{
  return translation * xRotation * yRotation * zRotation;
}
}
