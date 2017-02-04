
#include "toggleButton.h"

#include <memory>
#include <iostream>

#include "../../graphics/graphicsManager.h"
#include "../../graphics/resources.h"
#include "../../graphics/sprite.h"
#include "../../graphics/spriteBatcher.h"
#include "../../util/input.h"

using namespace std;
using namespace graphics;

namespace widget {

ToggleButton::ToggleButton(int id, int x, int y, int width, int height,	function<void(int)> observer,
        const string &name, int layer, Skin skin)
    : Button(id, x, y, width, height, observer, name, layer),
      m_skin{skin}
{
    auto &res = Resources::getInstance();

    FontMeshBuilder &fontMeshBuilder = res.getFontMeshBuilder(
        config::font_data::fontLayout,
        config::font_data::fontAtlasWidth,
        config::font_data::fontAtlasHeight);

    switch (skin) {
        case Skin::Regular: {
            m_spriteToggled.reset(new Sprite(x, y, layer, width, height, res.getTexture(config::gui_data::buttonToggled)));
            break;
        }
        case Skin::CheckBox: {
            m_sprite = make_shared<Sprite>(x, y, layer, width, height, res.getTexture(config::gui_data::checkBox));
            m_spriteToggled.reset(new Sprite(x, y, layer, width, height,
                res.getTexture(config::gui_data::checkBoxChecked)));
            m_text.reset(new Sprite(x + height, y + 5, layer + 1, fontMeshBuilder.buldMeshForString(name, height - 5),
                res.getTexture(config::font_data::font)));
            break;
        }
        case Skin::ReadioButton: {
            m_sprite = make_shared<Sprite>(x, y, layer, width, height, res.getTexture(config::gui_data::radioButton));
            m_spriteToggled.reset(new Sprite(x, y, layer, width, height,
                res.getTexture(config::gui_data::radioButtonChecked)));
            m_text.reset(new Sprite(x + height, y + 5, layer + 1, fontMeshBuilder.buldMeshForString(name, height - 5),
                res.getTexture(config::font_data::font)));
            break;
        }
    }
}

bool ToggleButton::isToggled() {
    return m_toggled;
}

void ToggleButton::toggle() {
    m_toggled = !m_toggled;
}

void ToggleButton::setUntoggled() {
    m_toggled = false;
}

void ToggleButton::setToggled() {
    m_toggled = true;
}

void ToggleButton::draw() {
    GraphicsManager::getInstance().getSpriteBatcher().addBatch(m_toggled ? m_spriteToggled : m_sprite);
    GraphicsManager::getInstance().getSpriteBatcher().addBatch(m_text);

    if (m_skin == Skin::Regular && m_pointerInsideBorders)
        GraphicsManager::getInstance().getSpriteBatcher().addBatch(m_highlight);
}

void ToggleButton::update(float) {
    shared_ptr<util::Input> input = util::Input::getInstance();
    m_pointerInsideBorders = isInsideBorders(input->mouseVirtualAdjustedX, input->mouseVirtualAdjustedY);

    auto shouldToggle = m_pointerInsideBorders && input->action1Pressed;

    if (m_skin == Skin::ReadioButton && !m_toggled && shouldToggle) {
        m_observer(m_id);
    } else if (m_skin != Skin::ReadioButton && shouldToggle) {
        toggle();
        m_observer(m_id);
    }

}

} /* namespace widget */
