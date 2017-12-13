#include "image.h"

#include "../graphics/graphicsManager.h"
#include "../graphics/resources.h"
#include "../graphics/spriteBatcher.h"

namespace gui {

Image::Image(double x,
             double y,
             double width,
             double height,
             const std::string& image,
             graphics::GraphicsManager& graphicsManager)
  : m_graphicsManager{ graphicsManager } {
  m_sprite.reset(new graphics::Sprite{
    x,
    y,
    0,
    width,
    height,
    graphics::Resources::getInstance().getTexture(image) });
}

void
Image::draw() {
  m_graphicsManager.getSpriteBatcher().addBatch(*m_sprite);
}

} /* namespace gui */
