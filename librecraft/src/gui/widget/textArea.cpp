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

	for (auto &s : m_rows)
		spriteBatcher.addBatch(s.second);

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

	int y = m_yCoordinate + m_height - (m_rows.size() + 1) * m_fontHeight;

	m_rows.push_back(std::pair<string, shared_ptr<Sprite>>(
			str,
			make_shared<Sprite>(m_xCoordinate,
			y, m_layer + 1,
			fontMeshBuilder.buldMeshForString(str, m_fontHeight),
			res.getTexture(config::font_data::font))));

	if (y < m_yCoordinate) {
		m_rows.pop_front();
		for (auto sprite : m_rows)
			sprite.second->move(0, m_fontHeight);
	}
}

} /* namespace widget */
