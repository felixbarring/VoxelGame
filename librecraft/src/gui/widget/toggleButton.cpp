/*
 * toggleButton.cpp
 *
 *  Created on: Feb 2, 2016
 *      Author: felix
 */

#include "toggleButton.h"

#include <memory>
#include <iostream>

#include "../../graphics/resources.h"
#include "../../graphics/sprite.h"
#include "../../graphics/spriteBatcher.h"
#include "../../util/input.h"

using namespace std;
using namespace graphics;

namespace widget {

ToggleButton::ToggleButton(int id, int x, int y, int width, int height,	function<void(int)> observer,
        const string &name, int layer, Skin skin)
    : Button(id, x, y, width, height, observer, name, layer)
{
    auto &res = Resources::getInstance();

    switch (skin) {
        case Skin::Regular: {
            m_spriteToggled.reset(new Sprite(x, y, layer, width, height, res.getTexture(config::gui_data::buttonToggled)));
            break;
        }
        case Skin::CheckBox: {
            m_sprite = make_shared<Sprite>(x, y, layer, width, height, res.getTexture(config::gui_data::checkBox));
            m_spriteToggled.reset(new Sprite(x, y, layer, width, height, res.getTexture(config::gui_data::checkBoxChecked)));
            break;
        }
        case Skin::ReadioButton: {
            // TODO Implement
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

void ToggleButton::draw() {
    shared_ptr<Sprite> sprite;
    if (m_toggled)
        sprite = m_spriteToggled;
    else
        sprite = m_sprite;

    SpriteBatcher::getInstance().addBatch(sprite);
    SpriteBatcher::getInstance().addBatch(m_text);

    if (m_pointerInsideBorders)
		SpriteBatcher::getInstance().addBatch(m_highlight);
}

void ToggleButton::update(float timePassed) {
	shared_ptr<util::Input> input = util::Input::getInstance();

	m_pointerInsideBorders = isInsideBorders(input->mouseVirtualAdjustedX, input->mouseVirtualAdjustedY);

	if (m_pointerInsideBorders && input->action1Pressed) {
		toggle();
		m_observer(m_id);
	}

}

} /* namespace widget */
