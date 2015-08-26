/*
 * spriteBatcher.h
 *
 *  Created on: Aug 26, 2015
 *      Author: felix
 */

#ifndef SRC_GRAPHICS_SPRITEBATCHER_H_
#define SRC_GRAPHICS_SPRITEBATCHER_H_

#include <vector>
#include <memory>

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

// ########################################################
// Instance Variables #####################################
// ########################################################

	std::vector<std::shared_ptr<Sprite>> batches;

};

} /* namespace graphics */

#endif /* SRC_GRAPHICS_SPRITEBATCHER_H_ */
