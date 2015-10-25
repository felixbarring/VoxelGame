
#include "textInput.h"

#include <iostream>

#include "../../graphics/spriteBatcher.h"
#include "../../graphics/mesh/meshElement.h"
#include "../../config/data.h"
#include "../../graphics/resources.h"

#include "../../util/input.h"

namespace widget {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

TextInput::TextInput(int id, int x, int y, int width, int height) :
	AbstractWidget(id, x, y, width, height)
{
	sprite.reset(new graphics::Sprite{x, y, 0, width, height, graphics::Resources::getInstance().getTexture(config::gui_data::guiBox)});

	graphics::FontMeshBuilder &fontMeshBuilder = graphics::Resources::getInstance().getFontMeshBuilder(
			config::font_data::fontLayout, config::font_data::fontAtlasWidth, config::font_data::fontAtlasHeight);

	text.reset(new graphics::Sprite{x, y+5, 1, fontMeshBuilder.buldMeshForString(input, height-5),
		graphics::Resources::getInstance().getTexture(config::font_data::font)});

}

// ########################################################
// Member Functions########################################
// ########################################################

void TextInput::draw()
{

	if (util::Input::getInstance()->eraseTextPressed && input.size() > 0) {
		input.pop_back();

		graphics::FontMeshBuilder &fontMeshBuilder = graphics::Resources::getInstance().getFontMeshBuilder(
					config::font_data::fontLayout, config::font_data::fontAtlasWidth, config::font_data::fontAtlasHeight);

		text.reset(new graphics::Sprite{x, y+5, 1, fontMeshBuilder.buldMeshForString(input, height-5),
				graphics::Resources::getInstance().getTexture(config::font_data::font)});

	}

	graphics::SpriteBatcher::getInstance().addBatch(sprite);

	if (pointerInsideBorders || hasFocus) {
		graphics::SpriteBatcher::getInstance().addBatch(sprite);
	}

	graphics::SpriteBatcher::getInstance().addBatch(text);

}

void TextInput::mouseClicked(int button, float x, float y)
{
	hasFocus = isInsideBorders(x, y);
}

void TextInput::mouseMoved(float x, float y)
{
	pointerInsideBorders = isInsideBorders(x, y);
}

void TextInput::keyPressed(int key)
{
	// Check if its the remove button, if so -> remove
}

void TextInput::keyTyped(char value)
{

	std::cout << value << "\n";

	if (hasFocus) {
		input.push_back(value);
	}

	graphics::FontMeshBuilder &fontMeshBuilder = graphics::Resources::getInstance().getFontMeshBuilder(
			config::font_data::fontLayout, config::font_data::fontAtlasWidth, config::font_data::fontAtlasHeight);

	text.reset(new graphics::Sprite{x, y+5, 1, fontMeshBuilder.buldMeshForString(input, height-5),
			graphics::Resources::getInstance().getTexture(config::font_data::font)});

}

} /* namespace widget */
