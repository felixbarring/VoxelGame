#include "toggleButton.h"

#include <iostream>
#include <memory>

#include "graphics/graphicsManager.h"
#include "graphics/resources.h"
#include "graphics/sprite.h"
#include "graphics/spriteBatcher.h"
#include "util/input.h"

using namespace std;
using namespace graphics;

namespace widget {

ToggleButton::ToggleButton(double id,
                           double x,
                           double y,
                           double width,
                           double height,
                           graphics::GraphicsManager& graphicsManager,
                           function<void(int)> observer,
                           const string& name,
                           int layer,
                           Skin skin)
  : Button(id, x, y, width, height, graphicsManager, observer, name, layer)
  , m_skin{skin}
  , m_spriteToggled(
      x,
      y,
      layer,
      width,
      height,
      Resources::getInstance().getTexture(config::gui_data::buttonToggled)) {

  Resources& res = Resources::getInstance();

  FontMeshBuilder& fontMeshBuilder =
    res.getFontMeshBuilder(config::font_data::fontLayout,
                           config::font_data::fontAtlasWidth,
                           config::font_data::fontAtlasHeight);

  switch (skin) {
    case Skin::Regular: {
      m_spriteToggled = Sprite{x,
                               y,
                               layer,
                               width,
                               height,
                               res.getTexture(config::gui_data::buttonToggled)};
      break;
    }
    case Skin::CheckBox: {
      m_sprite = Sprite{
        x, y, layer, width, height, res.getTexture(config::gui_data::checkBox)};

      m_spriteToggled =
        Sprite{x,
               y,
               layer,
               width,
               height,
               res.getTexture(config::gui_data::checkBoxChecked)};

      m_text = Sprite{x + height,
                      y + 5,
                      layer + 1,
                      fontMeshBuilder.buldMeshForString(name, height - 5),
                      res.getTexture(config::font_data::font)};
      break;
    }
    case Skin::RadioButton: {
      m_sprite = Sprite{x,
                        y,
                        layer,
                        width,
                        height,
                        res.getTexture(config::gui_data::radioButton)};

      m_spriteToggled =
        Sprite{x,
               y,
               layer,
               width,
               height,
               res.getTexture(config::gui_data::radioButtonChecked)};

      m_text = Sprite{x + height,
                      y + 5,
                      layer + 1,
                      fontMeshBuilder.buldMeshForString(name, height - 5),
                      res.getTexture(config::font_data::font)};
      break;
    }
  }
}

bool
ToggleButton::isToggled() {
  return m_toggled;
}

void
ToggleButton::toggle() {
  m_toggled = !m_toggled;
}

void
ToggleButton::setUntoggled() {
  m_toggled = false;
}

void
ToggleButton::setToggled() {
  m_toggled = true;
}

void
ToggleButton::draw() {
  m_graphicsManager.getSpriteBatcher().addBatch(m_toggled ? m_spriteToggled
                                                          : m_sprite);
  m_graphicsManager.getSpriteBatcher().addBatch(m_text);

  if (m_skin == Skin::Regular && m_pointerInsideBorders)
    m_graphicsManager.getSpriteBatcher().addBatch(m_highlight);
}

void
ToggleButton::update(float) {
  shared_ptr<util::Input> input = util::Input::getInstance();
  m_pointerInsideBorders =
    isInsideBorders(input->mouseVirtualAdjustedX, input->mouseVirtualAdjustedY);

  bool shouldToggle = m_pointerInsideBorders && input->action1Pressed;

  if (m_skin == Skin::RadioButton && !m_toggled && shouldToggle) {
    m_observer(m_id);
  } else if (m_skin != Skin::RadioButton && shouldToggle) {
    toggle();
    m_observer(m_id);
  }
}

} /* namespace widget */
