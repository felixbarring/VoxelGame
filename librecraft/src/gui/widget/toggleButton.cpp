/*
 * toggleButton.cpp
 *
 *  Created on: Feb 2, 2016
 *      Author: felix
 */

#include "toggleButton.h"

#include "../../graphics/spriteBatcher.h"

using namespace std;
using namespace graphics;

namespace widget {

ToggleButton::ToggleButton(int id, int x, int y, int width, int height,
		function<void(int)> observer,
		const string &name, int layer) :
			Button(id, x, y, width, height, observer, name, layer)
{

}


bool ToggleButton::isToggled()
{
	return m_toggled;
}

void ToggleButton::toggle()
{
	m_toggled = !m_toggled;
}

void ToggleButton::draw()
{
	if (m_pointerInsideBorders || m_toggled) {
		SpriteBatcher::getInstance().addBatch(m_sprite);
		SpriteBatcher::getInstance().addBatch(m_text);
		SpriteBatcher::getInstance().addBatch(m_highlight);
	} else {
		SpriteBatcher::getInstance().addBatch(m_sprite);
		SpriteBatcher::getInstance().addBatch(m_text);
	}
}

void ToggleButton::update()
{
//	if(isInsideBorders(x,y)) {
//		m_toggled = !m_toggled;
//		m_observer.operator ()(m_id);
//	}
}

//void ToggleButton::mouseClicked(int button, float x, float y)
//{
//	if(isInsideBorders(x,y)) {
//		m_toggled = !m_toggled;
//		m_observer.operator ()(m_id);
//	}
//}

} /* namespace widget */
