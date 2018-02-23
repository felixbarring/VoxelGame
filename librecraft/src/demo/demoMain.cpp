
#include <exception>
#include <iostream>
#include <string>

#include "arrayTextureDemo.h"
#include "cubeDemo.h"
#include "cubeMapDemo.h"
#include "fontDemo.h"
#include "guiDemo.h"
#include "libNoiseDemo.h"
#include "shadowMapDemo.h"
#include "spriteDemo.h"
#include "textureDemo.h"
#include "xmlDemo.h"

#ifdef DOCTEST_CONFIG_DISABLE

int
main(int, char* argv[]) {
  try {
    std::string str = argv[1];

    if (str == "textureDemo") {
      demo::TextureDemo textureDemo;
      textureDemo.runDemo();
    } else if (str == "cubeDemo") {
      demo::CubeDemo cubeDemo;
      cubeDemo.runDemo();
    } else if (str == "textureArrayDemo") {
      demo::ArrayTextureDemo arrayTextureDemo;
      arrayTextureDemo.runDemo();
    } else if (str == "cubeMapDemo") {
      demo::CubeMapDemo cubeMapDemo;
      cubeMapDemo.runDemo();
    } else if (str == "spriteDemo") {
      demo::SpriteDemo spriteDemo;
      spriteDemo.runDemo();
    } else if (str == "guiDemo") {
      demo::GuiDemo guiDemo;
      guiDemo.runDemo();
    } else if (str == "fontDemo") {
      demo::FontDemo fontDemo;
      fontDemo.runDemo();
    } else if (str == "shadowMapDemo") {
      demo::ShadowMapDemo shadowDemo;
      shadowDemo.runDemo();
    } else if (str == "libNoiseDemo") {
      demo::LibNoiseDemo libNoiseDemo;
      libNoiseDemo.runDemo();
    } else if (str == "xmlDemo") {
      demo::XmlDemo xmlDemo;
      xmlDemo.runDemo();
    }

  } catch (std::exception& e) {
    std::cout << "An error occured \n" << e.what() << "\n";
  } catch (...) {
    std::cout << "An error that is not derived from std::exception occured \n";
  }
}

#endif
