
#include "textInput.h"

#include <iostream>

#include "../../graphics/spriteBatcher.h"
#include "../../graphics/mesh/meshElement.h"
#include "../../config/data.h"
#include "../../graphics/resources.h"

#include "../../util/input.h"

using namespace std;
using namespace graphics;

namespace widget {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

TextInput::TextInput(int id, int x, int y, int width, int height) :
	AbstractWidget(id, x, y, width, height)
{

	auto &res = Resources::getInstance();

	sprite.reset(new Sprite{x, y, 0, width, height,
		res.getTexture(config::gui_data::solidBlack)});

	FontMeshBuilder &fontMeshBuilder = res.getFontMeshBuilder(
			config::font_data::fontLayout,
			config::font_data::fontAtlasWidth,
			config::font_data::fontAtlasHeight);

	text.reset(new Sprite{x, y + 5, 1,
		fontMeshBuilder.buldMeshForString(input, height - 5),
		res.getTexture(config::font_data::font)});

}

// ########################################################
// Member Functions########################################
// ########################################################

void TextInput::draw()
{
	SpriteBatcher::getInstance().addBatch(sprite);

	// TODO Draw a blinking marker

	SpriteBatcher::getInstance().addBatch(text);

}

void TextInput::update()
{
	auto &res = Resources::getInstance();

	if (hasFocus && util::Input::getInstance()->eraseTextPressed &&
			input.size() > 0) {
		input.pop_back();

		FontMeshBuilder &fontMeshBuilder = res.getFontMeshBuilder(
					config::font_data::fontLayout,
					config::font_data::fontAtlasWidth,
					config::font_data::fontAtlasHeight);

		text.reset(new Sprite{x, y + 5, 1,
			fontMeshBuilder.buldMeshForString(input, height - 5),
				res.getTexture(config::font_data::font)});

	}

}

void TextInput::mouseClicked(int button, float x, float y)
{
	hasFocus = isInsideBorders(x, y);
}

void TextInput::mouseMoved(float x, float y)
{
	pointerInsideBorders = isInsideBorders(x, y);
}

void TextInput::keyTyped(char value)
{

	auto &res = Resources::getInstance();

	if (hasFocus)
		input.push_back(value);

	FontMeshBuilder &fontMeshBuilder = res.getFontMeshBuilder(
			config::font_data::fontLayout,
			config::font_data::fontAtlasWidth,
			config::font_data::fontAtlasHeight);

	text.reset(new Sprite{x, y + 5, 1,
		fontMeshBuilder.buldMeshForString(input, height - 5),
		res.getTexture(config::font_data::font)});

}

void TextInput::setString(string str)
{

	auto &res = Resources::getInstance();

	input = str;

	FontMeshBuilder &fontMeshBuilder = res.getFontMeshBuilder(
						config::font_data::fontLayout,
						config::font_data::fontAtlasWidth,
						config::font_data::fontAtlasHeight);

	text.reset(new Sprite{x, y + 5, 1,
		fontMeshBuilder.buldMeshForString(input, height - 5),
		res.getTexture(config::font_data::font)});

}

string TextInput::getString()
{
	return input;
}

} /* namespace widget */
