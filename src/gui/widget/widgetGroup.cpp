
#include "widgetGroup.h"

#include <iostream>

#include "../../config/data.h"
#include "../../graphics/resources.h"

namespace widget {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

WidgetGroup::WidgetGroup(int id, int x, int y, int width, int height, std::function<void(int)> observer) :
	AbstractWidget(id, x, y, width, height)
{
	sprite.reset(new graphics::Sprite{x, y, 0, width, height, graphics::Resources::getInstance().getTexture(config::gui_data::guiBox)});
}

// ########################################################
// Member Functions########################################
// ########################################################

void WidgetGroup::addWidget(std::shared_ptr<IWidget> widget)
{
	widgets.push_back(widget);
}

void WidgetGroup::draw()
{

	graphics::SpriteBatcher::getInstance().addBatch(sprite);

	for (std::shared_ptr<widget::IWidget> widget : widgets) {
		widget->draw();
	}
}

// TODO Maybe not a good idea :o
// Some widgets need to lose focus when the user clicks outside
// the widget group
void WidgetGroup::mouseClicked(int button, float x, float y)
{
	if (isInsideBorders(x, y)) {
		for (std::shared_ptr<widget::IWidget> widget : widgets) {
			widget->mouseClicked(button, x, y);
		}
	}
}

void WidgetGroup::mouseMoved(float x, float y)
{
	if (isInsideBorders(x, y)) {
		for (std::shared_ptr<widget::IWidget> widget : widgets) {
			widget->mouseMoved(x, y);
		}
	}
}

void WidgetGroup::keyPressed(int key)
{
	for (std::shared_ptr<widget::IWidget> widget : widgets) {
		widget->keyPressed(key);
	}
}

void WidgetGroup::keyTyped(char value)
{
	for (std::shared_ptr<widget::IWidget> widget : widgets) {
		widget->keyTyped(value);
	}
}


} /* namespace demo */