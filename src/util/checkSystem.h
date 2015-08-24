
#ifndef SRC_UTIL_CHECKSYSTEM_H_
#define SRC_UTIL_CHECKSYSTEM_H_

#include <iostream>

#include <GL/glew.h>

namespace util
{

namespace check_system
{

void checkStuff(){

	int maxLayers;
	glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxLayers);

	// std::cout << "Number of Array Texture Layers that are supported " << maxLayers << "\n";

}

}

}


#endif /* SRC_UTIL_CHECKSYSTEM_H_ */
