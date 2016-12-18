
#include <string>
#include <iostream>
#include <exception>

#include "arrayTextureDemo.h"
#include "cubeDemo.h"
#include "cubeMapDemo.h"
#include "fontDemo.h"
#include "guiDemo.h"
#include "spriteDemo.h"
#include "textureDemo.h"

int main(int argc, char *argv[]) {
    try {
        std::string str = argv[1];

        if (str == "textureDemo") {
            demo::TextureDemo textureDemo;
            textureDemo.runDemo();
        }
        else if (str == "cubeDemo") {
            demo::CubeDemo cubeDemo;
            cubeDemo.runDemo();
        }
        else if (str == "textureArrayDemo") {
            demo::ArrayTextureDemo arrayTextureDemo;
            arrayTextureDemo.runDemo();
        }
        else if (str == "cubeMapDemo") {
            demo::CubeMapDemo cubeMapDemo;
            cubeMapDemo.runDemo();
        }
        else if (str == "spriteDemo") {
            demo::SpriteDemo spriteDemo;
            spriteDemo.runDemo();
        }
        else if (str == "guiDemo") {
            demo::GuiDemo guiDemo;
            guiDemo.runDemo();
        }
        else if (str == "fontDemo") {
            demo::FontDemo fontDemo;
            fontDemo.runDemo();
        }

    } catch (std::exception &e) {
        std::cout << "An error occured \n" << e.what() << "\n";
    } catch (...) {
        std::cout << "An error that is not derived from std::exception occured \n";
    }

}



