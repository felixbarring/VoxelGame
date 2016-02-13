/*
 * toggleButton.cpp
 *
 *  Created on: Feb 2, 2016
 *      Author: felix
 */

#include "toggleButton.h"

#include <memory>
#include <iostream>

#include "../../graphics/spriteBatcher.h"

#include "../../util/input.h"

using namespace std;
using namespace graphics;

namespace widget {

ToggleButton::ToggleButton(int id, int x, int y, int width, int height,
		function<void(int)> observer, const string &name, int layer)
		: Button(id, x, y, width, height, observer, name, layer) {
}

bool ToggleButton::isToggled() {
	return m_toggled;
}

void ToggleButton::toggle() {
	m_toggled = !m_toggled;
}

void ToggleButton::draw() {
	if (m_pointerInsideBorders) {
		SpriteBatcher::getInstance().addBatch(m_sprite);
		SpriteBatcher::getInstance().addBatch(m_text);
		SpriteBatcher::getInstance().addBatch(m_highlight);
	} else {
		SpriteBatcher::getInstance().addBatch(m_sprite);
		SpriteBatcher::getInstance().addBatch(m_text);
	}

	if (m_toggled)
		SpriteBatcher::getInstance().addBatch(m_highlight);

}

void ToggleButton::update(float timePassed) {
	shared_ptr<util::Input> input = util::Input::getInstance();

	m_pointerInsideBorders = isInsideBorders(input->mouseVirtualAdjustedX,
			input->mouseVirtualAdjustedY);

	if (m_pointerInsideBorders && input->action1Pressed) {
		toggle();
		m_observer.operator ()(m_id);
	}

}

} /* namespace widget */
