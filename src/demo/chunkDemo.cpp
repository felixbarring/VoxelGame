
#include "chunkDemo.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../graphics/shaderProgram.h"
#include "../graphics/texture/texture.h"
#include "../graphics/texture/arrayTexture.h"
#include "../graphics/transform.h"
#include "../graphics/graphicalChunk.h"

#include "../graphics/camera.h"

#include "../util/fpsManager.h"

// ########################################################
// Constructor/Destructor #################################
// ########################################################

ChunkDemo::ChunkDemo()
{
}

ChunkDemo::~ChunkDemo()
{
}

// ########################################################
// Member Functions########################################
// ########################################################

void ChunkDemo::runDemo()
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

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Chunk Demo", nullptr, nullptr);
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
	std::map<std::string, int> *map =
			new std::map<std::string, int> {std::pair<std::string, int>("positionIn", 0),
		std::pair<std::string, int>("normalIn", 1), std::pair<std::string, int>("texCoordIn", 2)};

	ShaderProgram program(vertex, fragment, map);

	char data[16][16][16];
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			for (int k = 0; k < 16; k++) {
				data[i][j][k] = 3;
			}
		}
	}

	GraphicalChunk chunk{0, 0, -5.0f, data};

	std::vector<std::string> paths{"../resources/0001_grassSide.png", "../resources/0002_grassTop.png"};
	int textureWidth = 16;
	int textureHeight = 16;
	ArrayTexture arrayTexture{paths, textureWidth, textureHeight};


	float aspectRatio = 800 / 600;
	glm::mat4 Projection = glm::perspective(80.0f, aspectRatio, 0.1f, 100.0f);

	Camera camera{0,0,0};

	float screenCenterX = WIDTH/2;
	float screenCenterY = HEIGHT/2;

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)	{

		fpsManager.frameStart();

		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		program.bind();

		glActiveTexture(GL_TEXTURE0);
		arrayTexture.bind();
		program.setUniformli("texture1", 0);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			camera.moveForward(0.1f);
		}

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		// Reset mouse position for next frame
		glfwSetCursorPos(window, screenCenterX, screenCenterY);

		camera.changeViewDirection(screenCenterX - xpos, screenCenterY - ypos);

		glm::mat4 ModelView = camera.getViewMatrix() * chunk.getTransform().getMatrix();
		glm::mat4 ModelViewProjection = Projection * ModelView;
		program.setUniformMatrix4f("ModelViewProjection", ModelViewProjection);
		chunk.draw();

		fpsManager.sync();
		glfwSwapBuffers(window);
	}
	glfwTerminate();

}

