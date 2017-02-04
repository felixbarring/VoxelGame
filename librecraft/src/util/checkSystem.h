#ifndef SRC_UTIL_CHECKSYSTEM_H_
#define SRC_UTIL_CHECKSYSTEM_H_

#include <iostream>

#include <GL/glew.h>
#include <thread>

namespace util {

namespace check_system {

static void checkStuff() {

    int maxLayers;
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxLayers);

    std::cout << "Number of Array Texture Layers that are supported " << maxLayers << "\n";

//    const GLubyte *renderer = glGetString(GL_RENDERER);
//    const GLubyte *vendor = glGetString(GL_VENDOR);
//    const GLubyte *version = glGetString(GL_VERSION);
//    const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

//	std::cout << "-------------------------\n";
//	std::cout << "Graphics Card Info \n";
//	std::cout << "Vendor: " << vendor << "\n";
//	std::cout << "Renderer: " << renderer << "\n";
//	std::cout << "Version: " << version << "\n";
//	std::cout << "GLSL Version: " << glslVersion << "\n";
//	std::cout << "-------------------------\n";

	unsigned int hwThreads = std::thread::hardware_concurrency();
	std::cout << "Number of hardware threads = " << hwThreads;

}

}

}

#endif /* SRC_UTIL_CHECKSYSTEM_H_ */
