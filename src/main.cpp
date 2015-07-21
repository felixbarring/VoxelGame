
#define GLM_FORCE_RADIANS

#include "game.h"
#include "demo/textureDemo.h"
#include "demo/cubeDemo.h"
#include "demo/chunkDemo.h"
#include "demo/arrayTextureDemo.h"
#include "demo/cubeMapDemo.h"

#include <string>
#include <exception>
#include <iostream>

int main(int argc, char *argv[])
{
	try {

		std::cout << "argc =" << argc << "\n";

		if (argc == 1) {
			// Run game here
			ChunkDemo chunkDemo;
			chunkDemo.runDemo();
		} else {
			std::string str = argv[1];

			if (str == "textureDemo") {
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
	} catch(std::exception& e){
		std::cout << "An error occured \n" << e.what();
	} catch(...) {
		std::cout << "An error that is not derived from std::exception occured \n";
	}

}





