#include "button.h"

#include <functional>

#include "../../graphics/spriteBatcher.h"
#include "../../graphics/fontMeshBuilder.h"
#include "../../graphics/mesh/meshElement.h"
#include "../../config/data.h"
#include "../../graphics/graphicsManager.h"
#include "../../graphics/resources.h"

#include "../../util/input.h"
#include "../../util/soundPlayer.h"

using namespace std;
using namespace graphics;
using namespace util;

namespace widget {

Button::Button(int id, int x, int y, int width, int height,
    graphics::GraphicsManager &graphicsManager,
    function<void(int)> observer, string name, int layer
)
  : AbstractWidget(id, x, y, width, height, graphicsManager)
  , m_name(name)
  , m_observer{observer}
{
  auto &res = Resources::getInstance();

  m_sprite.reset(new Sprite(x, y, layer, width, height,
    res.getTexture(config::gui_data::button)));
  m_highlight.reset(new Sprite(x, y, layer + 1, width, height,
    res.getTexture(config::gui_data::highlight)));

  FontMeshBuilder &fontMeshBuilder = res.getFontMeshBuilder(
      config::font_data::fontLayout, config::font_data::fontAtlasWidth,
      config::font_data::fontAtlasHeight);

  m_text.reset(new Sprite(x, y + 5, layer + 1,
      fontMeshBuilder.buldMeshForString(name, height - 5),
      res.getTexture(config::font_data::font)));
}

std::string Button::getName() {
  return m_name;
}

void Button::draw() {
  SpriteBatcher &spriteBatcher{m_graphicsManager.getSpriteBatcher()};

  if (m_pointerInsideBorders) {
    spriteBatcher.addBatch(m_sprite);
    spriteBatcher.addBatch(m_text);
    spriteBatcher.addBatch(m_highlight);
  } else {
    spriteBatcher.addBatch(m_sprite);
    spriteBatcher.addBatch(m_text);
  }
}

void Button::update(float) {
  shared_ptr<Input> input{Input::getInstance()};

  m_pointerInsideBorders = isInsideBorders(input->mouseVirtualAdjustedX,
      input->mouseVirtualAdjustedY);
  if (m_pointerInsideBorders && input->action1Pressed)
    trigger();
}

void Button::trigger() {
  m_observer(m_id);
}

} /* namespace widget */
