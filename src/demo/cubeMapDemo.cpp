
#include "cubeMapDemo.h"

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../graphics/shaderProgram.h"
#include "../graphics/camera.h"
#include "../graphics/texture/textureCubeMap.h"
#include "../graphics/mesh/meshV.h"
#include "../util/fpsManager.h"


// ########################################################
// Constructor/Destructor #################################
// ########################################################

CubeMapDemo::CubeMapDemo()
{
}


CubeMapDemo::~CubeMapDemo()
{
}

// ########################################################
// Member Functions########################################
// ########################################################

void CubeMapDemo::runDemo()
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

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Cube Map Demo", nullptr, nullptr);
	if (window == nullptr) {
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

    const char* skyBoxVertex =
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
		"} \n";

	const char* skyBoxFrag =
		"#version 330 core \n"

		"in vec3 texCoord; \n"

		"uniform samplerCube skybox; \n"

		"out vec4 color; \n"

		"void main() \n"
		"{ \n"
		"    color = texture(skybox, texCoord); \n"
		"} \n";

	std::map<std::string, int> attributesMap{std::pair<std::string, int>("positionIn", 0)};
	ShaderProgram skyboxShader(skyBoxVertex, skyBoxFrag, attributesMap);

    std::vector<GLfloat> vert{
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

    std::vector<short> element {
    	0, 1, 2, 3, 4, 5,
		0+6, 1+6, 2+6, 3+6, 4+6, 5+6,
		0+12, 1+12, 2+12, 3+12, 4+12, 5+12,

		0+18, 1+18, 2+18, 3+18, 4+18, 5+18,
		0+24, 1+24, 2+24, 3+24, 4+24, 5+24,
		0+30, 1+30, 2+30, 3+30, 4+30, 5+30,
    };

    MeshV mesh{vert, 3, element};

    Camera camera(0.0f, 0.0f, 3.0f);
    TextureCubeMap texture{
    	"../resources/skybox/right.jpg",
		"../resources/skybox/left.jpg",
		"../resources/skybox/top.jpg",
		"../resources/skybox/bottom.jpg",
		"../resources/skybox/back.jpg",
		"../resources/skybox/front.jpg",
		2048, 2048
    };

    float aspectRatio = WIDTH / HEIGHT;
    glm::mat4 projection = glm::perspective(80.0f, aspectRatio, 0.1f, 100.0f);

	float screenCenterX = WIDTH / 2;
	float screenCenterY = HEIGHT / 2;

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0) {

		fpsManager.frameStart();

		glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDepthFunc(GL_LEQUAL);

        double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		glfwSetCursorPos(window, screenCenterX, screenCenterY);

		camera.changeViewDirection(screenCenterX - xpos, screenCenterY - ypos);

        glm::mat4 view = glm::mat4(glm::mat3(camera.getViewMatrix()));

        skyboxShader.bind();

        skyboxShader.setUniformMatrix4f("view", view);
        skyboxShader.setUniformMatrix4f("projection", projection);

        texture.bind();

        mesh.render();

        fpsManager.sync();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
}



/*
#include "cubeMapDemo.h"

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../graphics/shaderProgram.h"
#include "../graphics/camera.h"
#include "../graphics/texture/textureCubeMap.h"
#include "../util/fpsManager.h"


// ########################################################
// Constructor/Destructor #################################
// ########################################################

CubeMapDemo::CubeMapDemo()
{
}


CubeMapDemo::~CubeMapDemo()
{
}

// ########################################################
// Member Functions########################################
// ########################################################

void CubeMapDemo::runDemo()
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

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "CUbe Map Demo", nullptr, nullptr);
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

    const char* skyBoxVertex =
		"#version 330 core \n"
		"layout (location = 0) in vec3 position; \n"
		"out vec3 TexCoords; \n"

		"uniform mat4 projection; \n"
		"uniform mat4 view; \n"

		"void main() \n"
		"{ \n"
		"    vec4 pos = projection * view * vec4(position, 1.0); \n"
		"    gl_Position = pos.xyww; \n"
		"    TexCoords = position; \n"
		"} \n";

	const char* skyBoxFrag =
		"#version 330 core \n"
		"in vec3 TexCoords; \n"
		"out vec4 color; \n"

		"uniform samplerCube skybox; \n"

		"void main() \n"
		"{ \n"
		"    color = texture(skybox, TexCoords); \n"
		"} \n";

	ShaderProgram skyboxShader(skyBoxVertex, skyBoxFrag, nullptr);

    GLfloat skyboxVertices[] = {
        // Positions
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


    // Setup skybox VAO
    GLuint skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);

    std::vector<std::string> paths{
		"../resources/skybox/right.jpg",
		"../resources/skybox/left.jpg",
		"../resources/skybox/top.jpg",
		"../resources/skybox/bottom.jpg",
		"../resources/skybox/back.jpg",
		"../resources/skybox/front.jpg"
    };

    Camera camera(0.0f, 0.0f, 3.0f);
    TextureCubeMap texture{paths, 2048, 2048};

    float aspectRatio = WIDTH / HEIGHT;
    glm::mat4 projection = glm::perspective(80.0f, aspectRatio, 0.1f, 100.0f);

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0) {

		fpsManager.frameStart();

		glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDepthFunc(GL_LEQUAL);

        glm::mat4 view = glm::mat4(glm::mat3(camera.getViewMatrix()));

        skyboxShader.bind();

        skyboxShader.setUniformMatrix4f("view", projection);
        skyboxShader.setUniformMatrix4f("projection", projection);

        texture.bind();

        // skybox cube
        glBindVertexArray(skyboxVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        fpsManager.sync();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
}
*/
