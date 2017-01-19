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

#include "../../graphics/graphicsManager.h"
using namespace std;
using namespace graphics;

namespace widget {

SelectableList::SelectableList(int id, int x, int y, int width, int height, function<void(int)> observer,
    unsigned layer)
    : AbstractWidget(id, static_cast<double>(x), static_cast<double>(y), width, height), m_layer{layer + 1}
{
	this->m_observer = observer;

	auto &res = Resources::getInstance();

	m_sprite = make_shared<Sprite>(static_cast<double>(x), static_cast<double>(y), layer, static_cast<double>(width),
	        static_cast<double>(height), res.getTexture(config::gui_data::button));
}

void SelectableList::addListItem(std::string item) {
    auto x = m_xCoordinate + 5;
    auto y = m_yCoordinate + m_height - (5 + 30 * (1 + m_buttons.size()));
    auto width = m_width - 10;
    auto height = 30;
    auto func = [&](int id)
    {
        auto button = getButtonWithId(id);
        if (button->isToggled()) {
            // If there already is a button toggled, untoggle it.
            if (m_currentlyToggled)
                m_currentlyToggled->toggle();

            m_currentlyToggled = button;
        } else {
            // No button is toggled.
            m_currentlyToggled.reset();
        }
    };

    auto button = make_shared<ToggleButton>(++idCounter, x, y, width, height, func, item, m_layer);
	m_buttons.push_back(std::move(button));
}

void SelectableList::deleteListItem(std::string item) {
}

void SelectableList::clear() {
	m_buttons.clear();
}

std::string SelectableList::getSelectedListItem() {
	if (m_currentlyToggled && m_currentlyToggled->isToggled())
		return m_currentlyToggled->getName();

	return "";
}

void SelectableList::reset() {
	if (m_currentlyToggled && m_currentlyToggled->isToggled())
		m_currentlyToggled->toggle();

	m_currentlyToggled.reset();
}

void SelectableList::draw() {
    GraphicsManager::getInstance().getSpriteBatcher().addBatch(m_sprite);
	for (auto b : m_buttons)
		b->draw();
}

void SelectableList::update(float timePassed) {
	for (auto b : m_buttons)
		b->update(timePassed);
}

shared_ptr<ToggleButton> SelectableList::getButtonWithId(int id) {
	for (auto b : m_buttons) {
		if (b->getId() == id)
			return b;
	}
	return nullptr; // Should never happen
}

} /* namespace widget */
