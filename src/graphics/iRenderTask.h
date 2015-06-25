
#ifndef SRC_GRAPHICS_IRENDERTASK_H_
#define SRC_GRAPHICS_IRENDERTASK_H_

#include "mesh/iMesh.h"
#include <glm/glm.hpp>

class IRenderTask {
public:
	virtual ~IRenderTask() {}

	virtual IMesh * getMesh() = 0;

	virtual ShaderProgram * getProgram() = 0;

	virtual glm::mat4 * getModelMatrix() = 0;

	virtual void setupUniforms() = 0;

};

#endif /* SRC_GRAPHICS_IRENDERTASK_H_ */
