/*
 * selectableList.cpp
 *
 *  Created on: Jan 31, 2016
 *      Author: felix
 */

#include "selectableList.h"

#include "../../graphics/spriteBatcher.h"
#include "../../graphics/fontMeshBuilder.h"
#include "../../graphics/mesh/meshElement.h"
#include "../../config/data.h"
#include "../../graphics/resources.h"

#include "../../util/input.h"

#include <iostream>

using namespace std;
using namespace graphics;

namespace widget {

SelectableList::SelectableList(int id, int x, int y, int width, int height,
		function<void(int)> observer, int layer):
		AbstractWidget(id, x, y, width, height),
		m_layer{layer + 1}
{
	this->m_observer = observer;

	auto &res = Resources::getInstance();

	m_sprite.reset(new Sprite{x, y, layer, width, height,
		res.getTexture(config::gui_data::button)});

}

void SelectableList::addListItem(std::string item)
{

	m_buttons.push_back(shared_ptr<ToggleButton>(
			new ToggleButton(++idCounter,
					m_xCoordinate + 5,
					m_yCoordinate + m_height -
					(5 + 30 * (1 + m_buttons.size())),
					m_width - 10,
					30,
					[&](int id)
					{
						auto button = getButtonWithId(id);
						if (button->isToggled()) {
							if (m_currentlyToggled)
								m_currentlyToggled->toggle();

							m_currentlyToggled = button;
						} else {
							m_currentlyToggled.reset();
						}
					}
					, item, m_layer)));

}

void SelectableList::deleteListItem(std::string item)
{
}

void SelectableList::clear()
{
	m_buttons.clear();
}

std::string SelectableList::getSelectedListItem()
{
	if (m_currentlyToggled && m_currentlyToggled->isToggled())
		return m_currentlyToggled->getName();

	return "";
}


void SelectableList::reset()
{
	if (m_currentlyToggled && m_currentlyToggled->isToggled()) {
		m_currentlyToggled->toggle();
	}
	m_currentlyToggled.reset();
}

void SelectableList::draw()
{
	SpriteBatcher::getInstance().addBatch(m_sprite);
	for (auto b : m_buttons)
		b->draw();
}

void SelectableList::update()
{
	for (auto b : m_buttons)
		b->update();
}

shared_ptr<ToggleButton> SelectableList::getButtonWithId(int id)
{
	for (auto b : m_buttons) {
		if (b->getId() == id)
			return b;
	}
	return nullptr; // Should never happen
}

} /* namespace widget */
