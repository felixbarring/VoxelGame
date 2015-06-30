
#define GLM_FORCE_RADIANS

#include "renderer.h"

#include <GL/glew.h>
#include <glm/gtx/transform.hpp>
#include <vector>

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Renderer::Renderer()
{
	glClearColor(0.2, 0.22, 0.2, 0.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
}

Renderer::~Renderer()
{
}

// ########################################################
// Member Functions########################################
// ########################################################

void Renderer::registerRenderTask(IRenderTask *task)
{
	tasks.push_back(task);
}

void Renderer::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (IRenderTask *task : tasks) {
    	task->getMesh();
    }

}




































/*


#define GLM_FORCE_RADIANS

#include "renderer.h"
#include "texture.h"

#include <GL/glew.h>
#include <glm/gtx/transform.hpp>

#include <iostream>
#include <fstream>
#include <vector>

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Renderer::Renderer() {
	const char *vert =
		"#version 330 core \n"

		"in vec3 positionIn; \n"
		"in vec3 normalIn; \n"
		"in vec2 texCoordIn; \n"

		// Matrices
		"uniform mat4 Projection; \n"
		"uniform mat3 Normal; \n"
		"uniform mat4 ModelView; \n"
		"uniform mat4 ModelViewProjection; \n"

		//
		"uniform vec3 lightDirection; \n"

		"uniform sampler2D dankTexture; \n"

		"out vec3 faceNormal; \n"
		"out vec4 colorOut; \n"

		"void main(){ \n"
		"  faceNormal = normalize( Normal * normalIn); \n"
		//"  colorOut = texture(dankTexture, texCoordIn); \n"
		"  colorOut = texture(dankTexture, vec2(1, 0)); \n"
		//"  colorOut = vec4(texCoordIn.y, texCoordIn.y, texCoordIn.y, 1); \n"
		"  vec4 eyeCoords = ModelView * vec4(positionIn, 1.0); \n"
		//"	vec3 halfV = normalize(lightDirection + eyeCoords)"
		"	gl_Position =  ModelViewProjection * vec4(positionIn,1); \n"
		"} \n";

	const char *frag =
		"#version 330 core \n"

		"in vec3 faceNormal; \n"
		"in vec4 colorOut; \n"

		"uniform vec3 lightDirection; \n"
		"uniform vec3 diffuseLight = vec3(1, 1, 0); \n"
		"uniform vec3 specularLight = vec3(1, 1, 1); \n"

		"uniform vec3 materialDiffuse; \n "
		"uniform vec3 materialSpecular = vec3(1, 1, 1); \n"

		"out vec4 color; \n"

		"vec3 calculateDiffuse() \n "
		"{ \n "
		"  return diffuseLight * materialDiffuse * max(0, dot(faceNormal, normalize(lightDirection))); \n "
		"} \n "

		"void main(){ \n"
		"  vec3 difuse = calculateDiffuse(); \n "
		//"  color = vec4(difuse, 0.1); \n"
		//"  color = vec4(colorOut, 1); \n"
		"  color = colorOut; \n"
		//"  color = vec4(faceNormal, 1);"
		"} \n";

	    program = new ShaderProgram(vert, frag, nullptr);

	    glClearColor(0.2, 0.22, 0.2, 0.0);
	    glEnable(GL_DEPTH_TEST);
	    glDepthFunc(GL_LESS);
	    //glEnable(GL_CULL_FACE);

	    float aspectRatio = 800 / 600;
	    Projection = glm::perspective(80.0f, aspectRatio, 0.1f, 100.0f);

	    std::vector<std::string> list;
	    std::ifstream in_stream;
	    std::string line;
	    in_stream.open("levels/level.lvl");

	    TexturedCube cube{0, 0};
	    cubes.push_back(cube);
	}

Renderer::~Renderer() {
	// TODO Auto-generated destructor stub
}

// ########################################################
// Member Functions########################################
// ########################################################

void Renderer::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program->bind();

    program->setUniform3f("lightDirection", -0.7f, -0.5f, 0.3f);
    program->setUniform3f("materialDiffuse", 0.5, 0.5, 0.5);

    // Camera matrix
    glm::mat4 View = glm::lookAt(
		glm::vec3(0, 3, 7), // Camera location
		glm::vec3(0, 0, 0), // Look at
		glm::vec3(0, 0, 1) // Head is up
	);

    for (TexturedCube cube : cubes) {
    	glm::mat4 ModelView = View * cube.getModelMatrix();
    	program->setUniformMatrix4f("ModelView", ModelView);

    	glm::mat3 Normal = glm::mat3(glm::vec3(ModelView[0]), glm::vec3(ModelView[1]), glm::vec3(ModelView[2]));
    	program->setUniformMatrix3f("Normal", Normal);

    	glm::mat4 ModelViewProjection = Projection * ModelView;
        program->setUniformMatrix4f("ModelViewProjection", ModelViewProjection);
        cube.draw();
    }

    program->unbind();
}



*/


