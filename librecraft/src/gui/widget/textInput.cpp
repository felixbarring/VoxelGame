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

TextInput::TextInput(int id, int x, int y, unsigned width, int height, int layer)
		: AbstractWidget(id, x, y, width, height),
		  m_maxInputLength{width},
		  m_textHeight{height - 5}
		  {

	auto &res = Resources::getInstance();

	m_background = make_shared<Sprite>(x, y, 0, width, height, res.getTexture(config::gui_data::solidBlack));
	m_cursor = make_shared<Sprite>(x, y + 4, 0, 1, height - 8, res.getTexture(config::gui_data::solidWhite));

	auto &fontMeshBuilder = res.getFontMeshBuilder(
			config::font_data::fontLayout,
			config::font_data::fontAtlasWidth,
			config::font_data::fontAtlasHeight);

	m_text = make_shared<Sprite>(x, y + 5, 1, fontMeshBuilder.buldMeshForString(m_input, m_textHeight),
		res.getTexture(config::font_data::font));

}

// ########################################################
// Member Functions########################################
// ########################################################

void TextInput::setString(string str) {
	Resources &res = Resources::getInstance();
	m_input = str;

	FontMeshBuilder &fontMeshBuilder = res.getFontMeshBuilder(
			config::font_data::fontLayout,
			config::font_data::fontAtlasWidth,
			config::font_data::fontAtlasHeight);

	m_text = make_shared<Sprite>(m_xCoordinate, m_yCoordinate + 5, 1,
			fontMeshBuilder.buldMeshForString(m_input, m_textHeight), res.getTexture(config::font_data::font));

	// TODO Fix the cursor...
	auto strLenght = fontMeshBuilder.lenghtOfString(m_input, m_textHeight);
	m_cursor->setLocation(m_xCoordinate + strLenght, m_yCoordinate + 4, 0);


}

string TextInput::getString() {
	return m_input;
}

void TextInput::draw() {
	SpriteBatcher::getInstance().addBatch(m_background);

	if (m_cursorVissible && m_hasFocus)
		SpriteBatcher::getInstance().addBatch(m_cursor);

	SpriteBatcher::getInstance().addBatch(m_text);
}

void TextInput::update(float timePassed) {
	shared_ptr<Input> input = Input::getInstance();

	// Blinking cursor
	m_blinkTime += timePassed;
	if (m_blinkTime > m_blinkIntervall) {
		m_cursorVissible = !m_cursorVissible;
		m_blinkTime = 0;
	}

	if (input->action1Pressed)
		m_hasFocus = isInsideBorders(input->mouseVirtualAdjustedX, input->mouseVirtualAdjustedY);

	Resources &res = Resources::getInstance();
	// Need a better way to handle resources
	FontMeshBuilder &fontMeshBuilder = res.getFontMeshBuilder(
			config::font_data::fontLayout,
			config::font_data::fontAtlasWidth,
			config::font_data::fontAtlasHeight);

	if (input->keyWasTyped && m_hasFocus) {
		m_input.push_back(input->keyTyped);

		auto strLenght = fontMeshBuilder.lenghtOfString(m_input, m_textHeight);
		if (strLenght >	m_maxInputLength) {
			m_input.pop_back();
			return;
		}
		m_text = make_shared<Sprite>(m_xCoordinate, m_yCoordinate + 5, 1,
						fontMeshBuilder.buldMeshForString(m_input, m_height - 5),
						res.getTexture(config::font_data::font));

		// TODO Remove hardecoded shit
		m_cursor->setLocation(m_xCoordinate + strLenght, m_yCoordinate + 4, 0);

	} else {
		if (m_hasFocus && Input::getInstance()->eraseTextActive && m_input.size() > 0) {

			if (!Input::getInstance()->eraseTextPressed && m_accumulatedEraseTime < m_eraseDelay) {
				m_accumulatedEraseTime += timePassed;
				return;
			}

			m_accumulatedEraseTime = 0.0;
			m_input.pop_back();

			m_text = make_shared<Sprite>(m_xCoordinate, m_yCoordinate + 5, 1,
							fontMeshBuilder.buldMeshForString(m_input, m_height - 5),
							res.getTexture(config::font_data::font));

			auto strLenght = fontMeshBuilder.lenghtOfString(m_input, m_textHeight);
			m_cursor->setLocation(m_xCoordinate + strLenght, m_yCoordinate + 4, 0);
		}
	}

}

void TextInput::setFocus(bool value){
	m_hasFocus = value;
}

} /* namespace widget */
