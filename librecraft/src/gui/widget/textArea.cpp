#include "textArea.h"

#include "../../graphics/sprite.h"
#include "../../graphics/resources.h"

using namespace std;
using namespace graphics;

namespace widget {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

TextArea::TextArea(int id, int x, int y, int width, int height,
		std::function<void(int)> observer, int layer)
		: AbstractWidget(id, x, y, width, height),
		  m_layer{layer}{
	this->m_observer = observer;
	auto &res = Resources::getInstance();

	m_textArea.reset(
			new Sprite(x, y, layer, width, height,
					res.getTexture(config::gui_data::guiBox)));
}

// ########################################################
// Member Functions########################################
// ########################################################

void TextArea::draw() {
	SpriteBatcher &spriteBatcher(SpriteBatcher::getInstance());

	spriteBatcher.addBatch(m_textArea);

	for (auto s : m_sprites)
		spriteBatcher.addBatch(s);

}

void TextArea::update(float timePassed) {

}

void TextArea::add(string str) {

}

void TextArea::addLine(string str) {
	if (str.empty())
		return;

	auto &res = Resources::getInstance();
	FontMeshBuilder &fontMeshBuilder = res.getFontMeshBuilder(
				config::font_data::fontLayout,
				config::font_data::fontAtlasWidth,
				config::font_data::fontAtlasHeight);

	m_rows.push_back(str);

	int y = m_yCoordinate + m_height - (m_rows.size()) * m_fontHeight;
	if (y < m_yCoordinate) {
		// TODO Scroll Down :-)
		return;
	}

	m_sprites.push_back(make_shared<Sprite>(m_xCoordinate,
			m_yCoordinate + m_height - (m_rows.size()) * m_fontHeight,
			m_layer + 1,
			fontMeshBuilder.buldMeshForString(str, m_fontHeight),
			res.getTexture(config::font_data::font)));
}

} /* namespace widget */
