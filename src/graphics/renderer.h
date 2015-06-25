
#ifndef SRC_GRAPHICS_RENDERER_H_
#define SRC_GRAPHICS_RENDERER_H_

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "texturedCube.h"
#include "shaderProgram.h"
#include "texture.h"

#include "iRenderTask.h"

class Renderer {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	Renderer();
	virtual ~Renderer();

// ########################################################
// Member Functions########################################
// ########################################################

	void registerRenderTask(IRenderTask * task);

	void render();

// ########################################################
// Instance Variables #####################################
// ########################################################
private:
	std::vector<IRenderTask*> tasks;

};

#endif /* SRC_GRAPHICS_RENDERER_H_ */
