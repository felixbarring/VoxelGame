
#include "cubeDemo.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../graphics/shaderProgram.h"
#include "../graphics/texture/textureArray.h"
#include "../graphics/transform.h"
#include "../graphics/texturedCube.h"
#include "../util/fpsManager.h"
#include "../config/cubeData.h"

// ########################################################
// Constructor/Destructor #################################
// ########################################################

CubeDemo::CubeDemo()
{
}

CubeDemo::~CubeDemo()
{
}

// ########################################################
// Member Functions########################################
// ########################################################

void CubeDemo::runDemo()
{

	FPSManager fpsManager(100);
	const GLuint WIDTH = 800, HEIGHT = 600;

	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
	}
	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Textured Cubes Demo", nullptr, nullptr);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(-1);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
	}

	glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(0.2f, 0.22f, 0.2f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	const char *vertex =
		"#version 330 core \n"

		"in vec3 positionIn; \n"
		"in vec3 normalIn; \n"
		"in vec3 texCoordIn; \n"

		"uniform mat4 ModelViewProjection; \n"

		"out vec3 faceNormal; \n"
		"out vec3 texCoord; \n"

		"void main(){ \n"
		"  texCoord = vec3(texCoordIn.x, 1 - texCoordIn.y, texCoordIn.z); \n"
		"  gl_Position =  ModelViewProjection * vec4(positionIn, 1); \n"
		"} \n";

	const char *fragment =
		"#version 330 core \n"

		"in vec3 texCoord; \n"

		"uniform sampler2DArray texture1; \n"

		"out vec4 color; \n"

		"void main(){ \n"
		"  color = texture(texture1, texCoord); \n"
		"} \n";


	// Use Smart Pointer
	std::map<std::string, int> *map = new std::map<std::string, int> {
		std::pair<std::string, int>("positionIn", 0),
		std::pair<std::string, int>("normalIn", 1),
		std::pair<std::string, int>("texCoordIn", 2)
	};

	ShaderProgram program(vertex, fragment, map);

	TexturedCube texturedCube{2, 0, -1.0f, 0};
	TexturedCube texturedCube2{0, 0, -1.0f, 1};
	TexturedCube texturedCube3{-2, 0, -1.0f, 2};

	TextureArray texture(cube_data::textures, cube_data::TEXTURE_WIDTH, cube_data::TEXTURE_HEIGHT);

	float aspectRatio = 800 / 600;
	glm::mat4 Projection = glm::perspective(80.0f, aspectRatio, 0.1f, 100.0f);

	glm::mat4 camera = glm::lookAt(
		glm::vec3(0, 0.1, 2), // Camera location
		glm::vec3(0, 0, 0),   // Look at
		glm::vec3(0, 0, 1)    // Head is up
	);

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)	{

		fpsManager.frameStart();

		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		program.bind();

		glActiveTexture(GL_TEXTURE0);
		texture.bind();
		program.setUniformli("texture1", 0);

		texturedCube.getTransform().rotateX(0.05);
		texturedCube2.getTransform().rotateY(0.05);
		texturedCube3.getTransform().rotateZ(0.05);

		glm::mat4 ModelView = camera * texturedCube.getTransform().getMatrix();
		glm::mat4 ModelViewProjection = Projection * ModelView;
		program.setUniformMatrix4f("ModelViewProjection", ModelViewProjection);
		texturedCube.draw();

		glm::mat4 ModelView2 = camera * texturedCube2.getTransform().getMatrix();
		glm::mat4 ModelViewProjection2 = Projection * ModelView2;
		program.setUniformMatrix4f("ModelViewProjection", ModelViewProjection2);
		texturedCube2.draw();

		glm::mat4 ModelView3 = camera * texturedCube3.getTransform().getMatrix();
		glm::mat4 ModelViewProjection3 = Projection * ModelView3;
		program.setUniformMatrix4f("ModelViewProjection", ModelViewProjection3);
		texturedCube3.draw();

		fpsManager.sync();
		glfwSwapBuffers(window);
	}
	glfwTerminate();

}

