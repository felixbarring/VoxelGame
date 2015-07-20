
#define GLM_FORCE_RADIANS

#include "game.h"
#include "demo/textureDemo.h"
#include "demo/cubeDemo.h"
#include "demo/chunkDemo.h"
#include "demo/arrayTextureDemo.h"
#include "demo/cubeMapDemo.h"

#include <string>

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
	} else if (str == "chunkDemo") {
		ChunkDemo chunkDemo;
		chunkDemo.runDemo();
	} else if (str == "textureArrayDemo") {
		ArrayTextureDemo arrayTextureDemo;
		arrayTextureDemo.runDemo();
	} else if (str == "cubeMapDemo") {
		CubeMapDemo cubeMapDemo;
		cubeMapDemo.runDemo();
	}

}





