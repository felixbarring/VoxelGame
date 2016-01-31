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

using namespace std;
using namespace graphics;

namespace widget {

SelectableList::SelectableList(int id, int x, int y, int width, int height,
		function<void(int)> observer):
		AbstractWidget(id, x, y, width, height)
{
	this->observer = observer;

	auto &res = Resources::getInstance();

	m_sprite.reset(new Sprite{x, y, 0, width, height,
		res.getTexture(config::gui_data::button)});

}

void SelectableList::addListItem(std::string item)
{
	m_buttons.push_back(Button{666, x + 5,
		y + height - (5 + 30 * (1 + m_buttons.size())),
		width - 10, 30, observer, item, 2});
}

void SelectableList::deleteListItem(std::string item)
{
	// TODO
}

void SelectableList::draw()
{
	SpriteBatcher::getInstance().addBatch(m_sprite);
	for (auto b : m_buttons)
		b.draw();
}

void SelectableList::mouseClicked(int button, float x, float y)
{

}

void SelectableList::mouseMoved(float x, float y)
{
	for (auto b : m_buttons)
		b.mouseMoved(x, y);
}

void SelectableList::keyTyped(char value)
{

}

} /* namespace widget */
