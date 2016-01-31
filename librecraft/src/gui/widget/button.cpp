
#include "button.h"

#include "../../graphics/spriteBatcher.h"
#include "../../graphics/fontMeshBuilder.h"
#include "../../graphics/mesh/meshElement.h"
#include "../../config/data.h"
#include "../../graphics/resources.h"

using namespace std;
using namespace graphics;

namespace widget {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Button::Button(int id, int x, int y, int width, int height,
		function<void(int)> observer, const string &name, int layer) :
	AbstractWidget(id, x, y, width, height)
{
	this->observer = observer;

	auto &res = Resources::getInstance();

	sprite.reset(new Sprite{x, y, layer, width, height,
		res.getTexture(config::gui_data::button)});
	highlight.reset(new Sprite{x, y, layer + 1, width, height,
		res.getTexture(config::gui_data::highlight)});

	FontMeshBuilder &fontMeshBuilder = res.getFontMeshBuilder(
			config::font_data::fontLayout,
			config::font_data::fontAtlasWidth,
			config::font_data::fontAtlasHeight);

	text.reset(new Sprite{x, y + 5, layer + 1,
		fontMeshBuilder.buldMeshForString(name, height - 5),
		res.getTexture(config::font_data::font)});

}

// ########################################################
// Member Functions########################################
// ########################################################

void Button::draw()
{
	if (pointerInsideBorders) {
		SpriteBatcher::getInstance().addBatch(text);
		SpriteBatcher::getInstance().addBatch(sprite);
		SpriteBatcher::getInstance().addBatch(highlight);
	} else {
		SpriteBatcher::getInstance().addBatch(text);
		SpriteBatcher::getInstance().addBatch(sprite);
	}
}

void Button::mouseClicked(int button, float x, float y)
{
	if(isInsideBorders(x,y))
		observer.operator ()(id);

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
