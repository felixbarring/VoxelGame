#include "image.h"

#include "../graphics/resources.h"
#include "../graphics/spriteBatcher.h"
#include "../graphics/graphicsManager.h"

namespace gui {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Image::Image(double x, double y, double width, double height, const std::string image) {
	sprite.reset(new graphics::Sprite{x, y, 0, width, height,
			graphics::Resources::getInstance().getTexture(image)});
}

// ########################################################
// Member Functions########################################
// ########################################################

void Image::draw() {
	graphics::GraphicsManager::getInstance().getSpriteBatcher().addBatch(sprite);
}

} /* namespace gui */
