
#include "button.h"

#include <iostream>

#include "../../graphics/spriteBatcher.h"
#include "../../graphics/fontMeshBuilder.h"
#include "../../graphics/mesh/meshElement.h"
#include "../../config/data.h"
#include "../../graphics/resources.h"

namespace widget {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Button::Button(int id, int x, int y, int width, int height, std::function<void(int)> observer, const std::string &name) :
	AbstractWidget(id, x, y, width, height)
{

	this->observer = observer;

	sprite.reset(new graphics::Sprite{x, y, 0, width, height, graphics::Resources::getInstance().getTexture(config::gui_data::button)});
	highlight.reset(new graphics::Sprite{x, y, 0, width, height, graphics::Resources::getInstance().getTexture(config::gui_data::highlight)});

	graphics::FontMeshBuilder &fontMeshBuilder = graphics::Resources::getInstance().getFontMeshBuilder(
			config::font_data::fontLayout, config::font_data::fontAtlasWidth, config::font_data::fontAtlasHeight);

	text.reset(new graphics::Sprite{x, y+5, 1, fontMeshBuilder.buldMeshForString(name, height-5),
		graphics::Resources::getInstance().getTexture(config::font_data::font)});

}

// ########################################################
// Member Functions########################################
// ########################################################

void Button::draw()
{
	if (pointerInsideBorders) {
		graphics::SpriteBatcher::getInstance().addBatch(text);
		graphics::SpriteBatcher::getInstance().addBatch(sprite);
		graphics::SpriteBatcher::getInstance().addBatch(highlight);
	} else {
		graphics::SpriteBatcher::getInstance().addBatch(text);
		graphics::SpriteBatcher::getInstance().addBatch(sprite);
	}
}

void Button::mouseClicked(int button, float x, float y)
{
	if(isInsideBorders(x,y)){
		observer.operator ()(id);
	}
}

void Button::mouseMoved(float x, float y)
{
	pointerInsideBorders = isInsideBorders(x, y);
}

void Button::keyTyped(char value)
{
	// not relevant for button, do nothing
}


} /* namespace widget */
