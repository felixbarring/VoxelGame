#include "image.h"

#include "../graphics/graphicsManager.h"
#include "../graphics/resources.h"
#include "../graphics/spriteBatcher.h"

namespace {
graphics::Sprite createSprite(int x, int y, int width, int height, const std::string& image) {
  return graphics::Sprite(
      x,
      y,
      0,
      width,
      height,
      graphics::Resources::getInstance().getTexture(image));
}
}

namespace gui {

Image::Image(double x,
             double y,
             double width,
             double height,
             const std::string& image,
             graphics::GraphicsManager& graphicsManager)
  : m_graphicsManager{graphicsManager}
  , m_sprite{createSprite(x, y, width, height, image)} {
}

void
Image::draw() {
  m_graphicsManager.getSpriteBatcher().addBatch(m_sprite);
}

} /* namespace gui */
