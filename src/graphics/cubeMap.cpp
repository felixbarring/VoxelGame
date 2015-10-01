
#include "cubeMap.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"
#include "../graphics/shaderProgram.h"
#include "../graphics/camera.h"
#include "../graphics/texture/textureCubeMap.h"
#include "../graphics/mesh/meshElement.h"

namespace graphics
{

// ########################################################
// Constructor/Destructor #################################
// ########################################################

CubeMap::CubeMap(texture::TextureCubeMap &texture) :
	texture{texture}
{
}

CubeMap::~CubeMap()
{
}


// ########################################################
// Member Functions########################################
// ########################################################

void CubeMap::render()
{

	static std::map<std::string, int> attributesMap{std::pair<std::string, int>("positionIn", 0)};

	// TODO Remove the projcetion * view multiplication from the shader

	static ShaderProgram skyboxShader(
			"#version 330 core \n"

			"in vec3 positionIn; \n"

			"uniform mat4 projection; \n"
			"uniform mat4 view; \n"

			"out vec3 texCoord; \n"

			"void main() \n"
			"{ \n"
			"    vec4 pos = projection * view * vec4(positionIn, 1.0); \n"
			"    gl_Position = pos.xyww; \n"
			"    texCoord = positionIn; \n"
			"} \n",

			"#version 330 core \n"

			"in vec3 texCoord; \n"

			"uniform samplerCube skybox; \n"

			"out vec4 color; \n"

			"void main() \n"
			"{ \n"
			"    color = texture(skybox, texCoord); \n"
			"} \n",
			attributesMap);

	static std::vector<GLfloat> vert{
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	static std::vector<short> element {
		0, 1, 2, 3, 4, 5,
		0+6, 1+6, 2+6, 3+6, 4+6, 5+6,
		0+12, 1+12, 2+12, 3+12, 4+12, 5+12,

		0+18, 1+18, 2+18, 3+18, 4+18, 5+18,
		0+24, 1+24, 2+24, 3+24, 4+24, 5+24,
		0+30, 1+30, 2+30, 3+30, 4+30, 5+30,
	};

	static mesh::MeshElement mesh{vert, 3, element};

    skyboxShader.bind();

    glm::mat4 view = glm::mat4(glm::mat3(graphics::Camera::getInstance().getViewMatrix()));
    skyboxShader.setUniformMatrix4f("view", view);
    skyboxShader.setUniformMatrix4f("projection", graphics::Camera::getInstance().getProjectionMatrix());
    texture.bind();
    mesh.draw();

    skyboxShader.unbind();
}

}
