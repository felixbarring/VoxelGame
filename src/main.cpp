
#define GLM_FORCE_RADIANS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include "graphics/renderer.h"

#include "game.h"
#include "demo/textureDemo.h"
#include "demo/cubeDemo.h"

#include <string>

using namespace glm;

int main(int argc, char *argv[])
{
	// TODO Handle errors

	std::string str = argv[1];

	if (str == "game") {
		Game game;
		game.run();
	} else if (str == "textureDemo") {
		TextureDemo textureDemo;
		textureDemo.runDemo();
	} else if (str == "cubeDemo") {
		CubeDemo cubeDemo;
		cubeDemo.runDemo();
	}

}





