#include <vector>
#include <iostream>
#include <stdlib.h>
#include <map>
#include <string.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "shaderProgram.h"

using namespace std;

namespace graphics {

// TODO Remove printf

ShaderProgram::ShaderProgram(
    const string &vertexSource,
    const string &fragmentSource,
    const map<string, int> &attributes)
{
  // Create the shaders
  GLuint vertexID = createVertexShader(vertexSource);
  GLuint fragmentID = createFragmentShader(fragmentSource);

  programID = glCreateProgram();

  if (programID == 0)
    cout << "Error, could not create a program";

  glAttachShader(programID, vertexID);
  glAttachShader(programID, fragmentID);

  // Create locations for all the attributes
  for (auto attribute : attributes) {

  //		cout << "Attributes: " << attributes.size() << "\n";
  //		cout << "Attribute = " << attribute.second << " " << attribute.first.c_str() << " \n";

    glBindAttribLocation(programID, attribute.second, attribute.first.c_str());
    int errorCheck = glGetError();
    switch (errorCheck) {
    case GL_NO_ERROR:
      break; // All is well
    case GL_INVALID_VALUE:
      cout << " ################################################\n\n";
      cout << "Error, glBindAttribLocation gave a GL_INVALID_VALUE error \n\n";
      cout << "  Faulty attribute is = " << attribute.first.c_str() << "\n\n";
      cout << vertexSource;
      cout << fragmentSource;
      cout << " \n################################################\n";
      break;
    case GL_INVALID_OPERATION:
      cout << " ################################################\n\n";
      cout << "Error, glBindAttribLocation gave a GL_INVALID_OPERATION error. \n";
      cout << "  Faulty attribute is = " << attribute.first.c_str() << "\n\n";
      cout << vertexSource;
      cout << fragmentSource;
      cout << " \n################################################\n";
      break;
    }
  }

  glLinkProgram(programID);
  glUseProgram(programID);

  // For all uniforms, get a location and add the name and the location to the map.
  GLint numberOfUniforms;
  glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &numberOfUniforms);
  GLenum type;
  vector<GLchar> nameData(256);
  for (int i = 0; i < numberOfUniforms; ++i) {
    GLint arraySize = 0;
    GLsizei actualLength = 0;
    glGetActiveUniform(programID, i, nameData.size(), &actualLength,
            &arraySize, &type, &nameData[0]);
    string name((char*) &nameData[0], actualLength);

    int uniformLocation = glGetUniformLocation(programID, name.c_str());
    uniforms.insert(make_pair(name, uniformLocation));
    //cout << "Inserted new Uniform with name :" << name << ", and location: " << uniformLocation << "\n ";

    GLenum errorCheck = glGetError();
    switch (errorCheck) {
    case GL_NO_ERROR:
      break; // All is well
    case GL_INVALID_VALUE:
      cout << "Error, glGetUniformLocation gave a GL_INVALID_VALUE error. \n";
      return;
    case GL_INVALID_OPERATION:
      cout << "Error, glGetUniformLocation gave a GL_INVALID_OPERATION error. \n";
      return;
    }
  }

  // Program error Check
  GLint result = GL_FALSE;
  int logLength;
  glGetProgramiv(programID, GL_LINK_STATUS, &result);
  glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
  if (logLength > 0) {
    vector<char> errorMessage(logLength + 1);
    glGetProgramInfoLog(programID, logLength, NULL, &errorMessage[0]);
    cout << &errorMessage[0] << "\n";
  }

  glDetachShader(programID, vertexID);
  glDetachShader(programID, fragmentID);

  glDeleteShader(vertexID);
  glDeleteShader(fragmentID);

}

GLuint ShaderProgram::createVertexShader(const string &source) {
  return createShader(GL_VERTEX_SHADER, source);
}

GLuint ShaderProgram::createFragmentShader(const string &source) {
  return createShader(GL_FRAGMENT_SHADER, source);
}

GLuint ShaderProgram::createShader(GLenum shaderType, const string &source) {
  GLuint shaderID = glCreateShader(shaderType);

  char const *shaderSourcePointer = source.c_str();
  glShaderSource(shaderID, 1, &shaderSourcePointer, nullptr);
  glCompileShader(shaderID);

  // Error Check
  GLint result = GL_FALSE;
  int logLength;
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
  glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
  if (result == GL_FALSE) {
    vector<char> errorMessage(logLength + 1);
    glGetShaderInfoLog(shaderID, logLength, NULL, &errorMessage[0]);
    cout << &errorMessage[0] << "\n";
    cout << source;
  }
  return shaderID;
}

void ShaderProgram::setUniformi(string uniformName, GLuint value) {
  glUniform1ui(uniforms.find(uniformName)->second, value);
}

void ShaderProgram::setUniform1f(string uniformName, float value) {
  glUniform1f(uniforms.find(uniformName)->second, value);
}

void ShaderProgram::setUniform3f(string uniformName, float x, float y,
    float z)
{
  glUniform3f(uniforms.find(uniformName)->second, x, y, z);
}

void ShaderProgram::setUniformMatrix3f(string uniformName, glm::mat3& matrix) {
  glUniformMatrix3fv(uniforms.find(uniformName)->second, 1, GL_FALSE,
      &matrix[0][0]);
}

void ShaderProgram::setUniformMatrix4f(string uniformName, glm::mat4& matrix) {
  glUniformMatrix4fv(uniforms.find(uniformName)->second, 1, GL_FALSE,
      &matrix[0][0]);
}

void ShaderProgram::setUniformli(string uniformName, GLuint value) {
  glUniform1i(uniforms.find(uniformName)->second, value);
}

void ShaderProgram::bind() {
  glUseProgram(programID);
}

void ShaderProgram::unbind() {
  glUseProgram(0);
}

}

