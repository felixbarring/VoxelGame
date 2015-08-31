
#ifndef SRC_GRAPHICS_SPRITEBATCHER_H_
#define SRC_GRAPHICS_SPRITEBATCHER_H_

#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "sprite.h"

namespace graphics {

class SpriteBatcher {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	SpriteBatcher();

	virtual ~SpriteBatcher();

	SpriteBatcher(SpriteBatcher const&) = delete;

	void operator=(SpriteBatcher const&) = delete;

public:

	static SpriteBatcher& getInstance()
	{
		static SpriteBatcher INSTANCE;
		return INSTANCE;
	}

// ########################################################
// Member Functions########################################
// ########################################################

	void addBatch(std::shared_ptr<Sprite> batch);

	void draw();

	void setProjection(glm::mat4 projection);

// ########################################################
// Instance Variables #####################################
// ########################################################

private:

	std::vector<std::shared_ptr<Sprite>> batches;
	glm::mat4 projection;

};

} /* namespace graphics */

#endif /* SRC_GRAPHICS_SPRITEBATCHER_H_ */
