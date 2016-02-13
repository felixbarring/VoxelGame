#include "textInput.h"

#include <iostream>

#include "../../graphics/spriteBatcher.h"
#include "../../graphics/mesh/meshElement.h"
#include "../../config/data.h"
#include "../../graphics/resources.h"

#include "../../util/input.h"

using namespace std;
using namespace graphics;
using namespace util;

namespace widget {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

TextInput::TextInput(int id, int x, int y, unsigned width, int height,
		int layer)
		: AbstractWidget(id, x, y, width, height), m_maxInputLength {width} {

	Resources &res = Resources::getInstance();

	m_sprite.reset(
			new Sprite {x, y, 0, width, height, res.getTexture(
					config::gui_data::solidBlack)});

	FontMeshBuilder &fontMeshBuilder = res.getFontMeshBuilder(
			config::font_data::fontLayout, config::font_data::fontAtlasWidth,
			config::font_data::fontAtlasHeight);

	m_text.reset(
			new Sprite {x, y + 5, 1, fontMeshBuilder.buldMeshForString(m_input,
					height - 5), res.getTexture(config::font_data::font)});

}

// ########################################################
// Member Functions########################################
// ########################################################

void TextInput::setString(string str) {
	Resources &res = Resources::getInstance();
	m_input = str;

	FontMeshBuilder &fontMeshBuilder = res.getFontMeshBuilder(
			config::font_data::fontLayout, config::font_data::fontAtlasWidth,
			config::font_data::fontAtlasHeight);

	m_text.reset(
			new Sprite(m_xCoordinate, m_yCoordinate + 5, 1,
					fontMeshBuilder.buldMeshForString(m_input, m_height - 5),
					res.getTexture(config::font_data::font)));

}

string TextInput::getString() {
	return m_input;
}

void TextInput::draw() {
	SpriteBatcher::getInstance().addBatch(m_sprite);
	// TODO Draw a blinking marker
	SpriteBatcher::getInstance().addBatch(m_text);
}

void TextInput::update(float timePassed) {
	shared_ptr<Input> input = Input::getInstance();
	m_hasFocus = isInsideBorders(input->mouseVirtualAdjustedX,
			input->mouseVirtualAdjustedY);

	Resources &res = Resources::getInstance();
	// Need a better way to handle resources
	FontMeshBuilder &fontMeshBuilder = res.getFontMeshBuilder(
			config::font_data::fontLayout, config::font_data::fontAtlasWidth,
			config::font_data::fontAtlasHeight);

	if (input->keyWasTyped && m_hasFocus) {
		m_input.push_back(input->keyTyped);
		if (fontMeshBuilder.lenghtOfString(m_input, m_height)
				> m_maxInputLength) {
			m_input.pop_back();
			return;
		}
		m_text.reset(
				new Sprite(m_xCoordinate, m_yCoordinate + 5, 1,
						fontMeshBuilder.buldMeshForString(m_input,
								m_height - 5),
						res.getTexture(config::font_data::font)));
	} else {
		if (m_hasFocus && Input::getInstance()->eraseTextActive
				&& m_input.size() > 0) {

			if (!Input::getInstance()->eraseTextPressed
					&& m_accumulatedEraseTime < m_eraseDelay) {
				m_accumulatedEraseTime += timePassed;
				return;
			}

			m_accumulatedEraseTime = 0.0;

			m_input.pop_back();
			m_text.reset(
					new Sprite(m_xCoordinate, m_yCoordinate + 5, 1,
							fontMeshBuilder.buldMeshForString(m_input,
									m_height - 5),
							res.getTexture(config::font_data::font)));
		}
	}

}

} /* namespace widget */
