
#ifndef SRC_GRAPHICS_CUBEBATCHER_H_
#define SRC_GRAPHICS_CUBEBATCHER_H_

#include <vector>
#include <memory>

#include "texturedCube.h"
#include "camera.h"
#include "transform.h"

#include "shaderProgram.h"
#include "texture/textureArray.h"

namespace graphics {

class CubeBatcher {
private:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	CubeBatcher();

	virtual ~CubeBatcher() {};

	CubeBatcher(CubeBatcher const&) = delete;

	void operator=(CubeBatcher const&) = delete;

public:

	static CubeBatcher& getInstance()
	{
		static CubeBatcher INSTANCE;
		return INSTANCE;
	}

// ########################################################
// Member Functions########################################
// ########################################################

	void addBatch(char type, Transform &transform);

	void draw();

// ########################################################
// Instance Variables #####################################
// ########################################################

private:

	class Batch {
	public:

		Batch(TexturedCube &cube, Transform &transform):
			cube(cube),
			transform(transform)
		{
		}

		TexturedCube &cube;
		Transform &transform;
	};

	std::vector<Batch> batches;
	std::vector<TexturedCube> cubes;

	std::shared_ptr<ShaderProgram> program;
	texture::TextureArray &texture;

};

} /* namespace graphics */

#endif /* SRC_GRAPHICS_CUBEBATCHER_H_ */
