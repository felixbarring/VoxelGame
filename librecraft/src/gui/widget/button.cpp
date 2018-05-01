#include "button.h"

#include <functional>

#include "config/data.h"
#include "graphics/fontMeshBuilder.h"
#include "graphics/graphicsManager.h"
#include "graphics/mesh/meshElement.h"
#include "graphics/resources.h"
#include "graphics/spriteBatcher.h"

#include "util/input.h"
#include "util/soundPlayer.h"

using namespace std;
using namespace graphics;
using namespace util;

namespace widget {

graphics::FontMeshBuilder&
crateFontMeshBuilder() {
  FontMeshBuilder& fontMeshBuilder =
    Resources::getInstance().getFontMeshBuilder(
      config::font_data::fontLayout,
      config::font_data::fontAtlasWidth,
      config::font_data::fontAtlasHeight);
  return fontMeshBuilder;
}

Button::Button(int id,
               int x,
               int y,
               int width,
               int height,
               graphics::GraphicsManager& graphicsManager,
               function<void(int)> observer,
               string name,
               int layer)
  : AbstractWidget(id, x, y, width, height, graphicsManager)
  , m_name(name)
  , m_observer{observer}
  , m_sprite{x,
             y,
             layer,
             width,
             height,
             Resources::getInstance().getTexture(config::gui_data::button)}
  , m_highlight{x,
                y,
                layer + 1,
                width,
                height,
                Resources::getInstance().getTexture(
                  config::gui_data::highlight)}
  , m_text{x,
           y + 5,
           layer + 1,
           crateFontMeshBuilder().buldMeshForString(name, height - 5),
           Resources::getInstance().getTexture(config::font_data::font)} {
}

std::string
Button::getName() {
  return m_name;
}

void
Button::draw() {
  SpriteBatcher& spriteBatcher{m_graphicsManager.getSpriteBatcher()};

  if (m_pointerInsideBorders) {
    spriteBatcher.addBatch(m_sprite);
    spriteBatcher.addBatch(m_text);
    spriteBatcher.addBatch(m_highlight);
  } else {
    spriteBatcher.addBatch(m_sprite);
    spriteBatcher.addBatch(m_text);
  }
}

void
Button::update(float) {
  shared_ptr<Input> input{Input::getInstance()};

  m_pointerInsideBorders =
    isInsideBorders(input->mouseVirtualAdjustedX, input->mouseVirtualAdjustedY);
  if (m_pointerInsideBorders && input->action1Pressed)
    trigger();
}

void
Button::trigger() {
  m_observer(m_id);
}

} /* namespace widget */
