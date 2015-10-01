
#define GLM_FORCE_RADIANS

#include "game.h"
#include "demo/textureDemo.h"
#include "demo/cubeDemo.h"
#include "demo/chunkDemo.h"
#include "demo/arrayTextureDemo.h"
#include "demo/cubeMapDemo.h"
#include "demo/playerCollisionDemo.h"
#include "demo/spriteDemo.h"
#include "demo/guiDemo.h"
#include "demo/fontDemo.h"

#include <string>
#include <exception>
#include <iostream>

int main(int argc, char *argv[])
{

	try {

		std::cout << "argc =" << argc << "\n";

		if (argc == 1) {
			// Run game here
			demo::PlayerCollisionDemo playerCollisionDemo;
			playerCollisionDemo.runDemo();
		} else {
			std::string str = argv[1];
			if (str == "game") {
				Game game;
				game.run();
			}
			if (str == "textureDemo") {
				demo::TextureDemo textureDemo;
				textureDemo.runDemo();
			} else if (str == "cubeDemo") {
				demo::CubeDemo cubeDemo;
				cubeDemo.runDemo();
			} else if (str == "chunkDemo") {
				demo::ChunkDemo chunkDemo;
				chunkDemo.runDemo();
			} else if (str == "textureArrayDemo") {
				demo::ArrayTextureDemo arrayTextureDemo;
				arrayTextureDemo.runDemo();
			} else if (str == "cubeMapDemo") {
				demo::CubeMapDemo cubeMapDemo;
				cubeMapDemo.runDemo();
			} else if (str == "playerCollisionDemo") {
				demo::PlayerCollisionDemo playerCollisionDemo;
				playerCollisionDemo.runDemo();
			} else if (str == "spriteDemo") {
				demo::SpriteDemo spriteDemo;
				spriteDemo.runDemo();
			} else if (str == "guiDemo") {
				demo::GuiDemo guiDemo;
				guiDemo.runDemo();
			} else if (str == "fontDemo") {
				demo::FontDemo fontDemo;
				fontDemo.runDemo();
			}
		}

	} catch(std::exception& e){
		std::cout << "An error occured \n" << e.what() << "\n";
	} catch(...) {
		std::cout << "An error that is not derived from std::exception occured \n";
	}

}





