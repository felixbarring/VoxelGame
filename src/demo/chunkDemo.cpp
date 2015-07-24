
#include "chunkDemo.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../graphics/chunkBatcher.h"
#include "../graphics/shaderProgram.h"
#include "../graphics/graphicalChunk.h"
#include "../graphics/camera.h"
#include "../graphics/texture/textureArray.h"
#include "../util/fpsManager.h"
#include "../config/data.h"
#include "../voxel.h"

#include "../model/world/chunk/chunk.h"

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

	Camera camera{0,0,0};

	float screenCenterX = WIDTH / 2;
	float screenCenterY = HEIGHT / 2;

	Chunk chunkMega{0, 0, 0};
	Chunk chunkMega2{16, 0, 0};
	Chunk chunkMega3{32, 0, 0};
	Chunk chunkMega4{48, 0, 0};

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)	{

		fpsManager.frameStart();

		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			camera.moveForward(0.1f);
		}

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		glfwSetCursorPos(window, screenCenterX, screenCenterY);

		camera.changeViewDirection(screenCenterX - xpos, screenCenterY - ypos);

		ChunkBatcher::getInstance().draw(camera);

		fpsManager.sync();
		glfwSwapBuffers(window);
	}
	glfwTerminate();

}



/*




#include "chunkDemo.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../graphics/shaderProgram.h"
#include "../graphics/graphicalChunk.h"
#include "../graphics/camera.h"
#include "../graphics/texture/textureArray.h"
#include "../util/fpsManager.h"
#include "../config/data.h"
#include "../voxel.h"

#include "../model/world/chunk/chunk.h"

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

	std::map<std::string, int> attributesMap{
		std::pair<std::string, int>("positionIn", 0),
		std::pair<std::string, int>("normalIn", 1),
		std::pair<std::string, int>("texCoordIn", 2)
	};

	ShaderProgram program(vertex, fragment, attributesMap);

	int counter = 1;
	const int maxCount = 4;

	Voxel data[16][16][16];
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			for (int k = 0; k < 16; k++) {
				Voxel v;
				v.id = counter++;
				data[i][j][k] = v;
				if (counter == maxCount) {
					counter = 1;
				}
			}
		}
	}

	GraphicalChunk chunk{0, 0, -5.0f, data};

	int textureWidth = 16;
	int textureHeight = 16;
	TextureArray texture{cube_data::textures, cube_data::TEXTURE_WIDTH, cube_data::TEXTURE_HEIGHT};

	float aspectRatio = WIDTH / HEIGHT;
	glm::mat4 Projection = glm::perspective(80.0f, aspectRatio, 0.1f, 100.0f);

	Camera camera{0,0,0};

	float screenCenterX = WIDTH / 2;
	float screenCenterY = HEIGHT / 2;

	Chunk chunkMega{0, 0, 0};

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)	{

		fpsManager.frameStart();

		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		program.bind();

		glActiveTexture(GL_TEXTURE0);
		texture.bind();
		program.setUniformli("texture1", 0);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			camera.moveForward(0.1f);
		}

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		glfwSetCursorPos(window, screenCenterX, screenCenterY);

		camera.changeViewDirection(screenCenterX - xpos, screenCenterY - ypos);

		glm::mat4 ModelView = camera.getViewMatrix() * chunk.getTransform().getMatrix();
		glm::mat4 ModelViewProjection = Projection * ModelView;
		program.setUniformMatrix4f("ModelViewProjection", ModelViewProjection);
		//chunk.draw();

		chunkMega.render();

		fpsManager.sync();
		glfwSwapBuffers(window);
	}
	glfwTerminate();

}

*/
