
#include <exception>
#include <iostream>
#include <string>

#include "arrayTextureDemo.h"
#include "billboardDemo.h"
#include "cubeDemo.h"
#include "cubeMapDemo.h"
#include "fontDemo.h"
#include "frustumDemo.h"
#include "guiDemo.h"
#include "libNoiseDemo.h"
#include "shadowMapDemo.h"
#include "spriteDemo.h"
#include "textureDemo.h"
#include "xmlDemo.h"

#ifdef DOCTEST_CONFIG_DISABLE

int
main(int, char* argv[]) {

  std::vector<std::string> alternatives{"textureDemo",
                                        "cubeDemo",
                                        "textureArrayDemo",
                                        "cubeMapDemo",
                                        "spriteDemo",
                                        "guiDemo",
                                        "fontDemo",
                                        "shadowMapDemo",
                                        "libNoiseDemo",
                                        "xmlDemo",
                                        "frustumDemo",
                                        "billboardDemo"};

  std::string str{};
  try {
    std::string tmp = argv[1];
    str = tmp;
  } catch (std::exception& e) {
    std::cout
      << "Enter which demo you want to run. These are the alternatives \n";
    for (string& s : alternatives) {
      std::cout << s << "\n";
    }
    return EXIT_SUCCESS;
  }

  try {
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
    } else if (str == "frustumDemo") {
      demo::FrustumDemo frustumDemo;
      frustumDemo.runDemo();
    } else if (str == "billboardDemo") {
      demo::BillboardDemo billboardDemo;
      billboardDemo.runDemo();
    } else {
      std::cout << "Unknown demo: " << str << "\n";
      std::cout
        << "Enter which demo you want to run. These are the alternatives \n";
      for (std::string& s : alternatives) {
        std::cout << s << "\n";
      }
    }

  } catch (std::exception& e) {
    std::cout << "An error occurred \n" << e.what() << "\n";
  } catch (...) {
    std::cout << "An error that is not derived from std::exception occurred \n";
  }
}

#endif
