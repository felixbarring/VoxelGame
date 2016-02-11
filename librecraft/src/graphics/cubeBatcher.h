
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

	void addBatch(char type, Transform &transform, int lightValue = 16);

	void draw();

// ########################################################
// Instance Variables #####################################
// ########################################################

private:

	class Batch {
	public:

		Batch(TexturedCube &cube, Transform &transform, int lightValue):
			m_cube(cube),
			m_transform(transform),
			m_lightValue(lightValue)
		{
		}

		TexturedCube &m_cube;
		Transform &m_transform;
		int m_lightValue;
	};

	std::vector<Batch> batches;
	std::vector<TexturedCube> cubes;

	std::shared_ptr<ShaderProgram> program;
	texture::TextureArray &texture;

};

} /* namespace graphics */

#endif /* SRC_GRAPHICS_CUBEBATCHER_H_ */
