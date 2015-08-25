#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <map>
#include <string.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace graphics
{

class ShaderProgram {
public :

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	 ShaderProgram(const char *vertexSource, const char *fragmentSource , const std::map<std::string, int> &mapUniforms);

	~ShaderProgram();

// ########################################################
// Member Functions########################################
// ########################################################

private :

	GLuint createVertexShader(const char *vertexSource);

	GLuint createFragmentShader(const char *fragmentSource);

	GLuint createShader(GLenum shaderType, const char *source);

public :

	// TODO Use GLuint and other correct types

	void setUniformi(std::string uniformName, GLuint value);

	void setUniform1f(std::string uniformName, float value);

	void setUniform3f(std::string uniformName, float x, float y, float z);

	void setUniformMatrix3f(std::string uniformName, glm::mat3&);

	void setUniformMatrix4f(std::string uniformName, glm::mat4&);

	void setUniformli(std::string uniformName, GLuint value);

	void bind();

	void unbind();

// ########################################################
// Instance Variables #####################################
// ########################################################

private :
	GLuint programID;
	std::map<std::string, int> uniforms;

};

}

#endif
