
#include "widgetGroup.h"

#include <iostream>

#include "../../config/data.h"
#include "../../graphics/resources.h"

using namespace std;
using namespace graphics;

namespace widget {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

WidgetGroup::WidgetGroup(int id, int x, int y, int width, int height,
		function<void(int)> observer, int layer) :
	AbstractWidget(id, x, y, width, height)
{
	m_sprite.reset(new Sprite{x, y, 0, width, height,
		Resources::getInstance().getTexture(config::gui_data::guiBox)});
}

// ########################################################
// Member Functions########################################
// ########################################################

void WidgetGroup::addWidget(shared_ptr<IWidget> widget)
{
	m_widgets.push_back(widget);
}

void WidgetGroup::draw()
{
	SpriteBatcher::getInstance().addBatch(m_sprite);
	for (auto widget : m_widgets)
		widget->draw();
}

void WidgetGroup::update()
{
	for (auto widget : m_widgets)
		widget->update();
}

// TODO Maybe not a good idea :o
// Some widgets need to lose focus when the user clicks outside
// the widget group
void WidgetGroup::mouseClicked(int button, float x, float y)
{
	//if (isInsideBorders(x, y)) {
		for (auto widget : m_widgets)
			widget->mouseClicked(button, x, y);
	//}
}

void WidgetGroup::mouseMoved(float x, float y)
{
	if (isInsideBorders(x, y)) {
		for (auto widget : m_widgets)
			widget->mouseMoved(x, y);
	}
}

void WidgetGroup::keyTyped(char value)
{
	for (auto widget : m_widgets)
		widget->keyTyped(value);
}


} /* namespace demo */
