
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
		function<void(int)> observer, string name, int layer) :
	AbstractWidget(id, x, y, width, height),
	m_name(name)
{
	this->m_observer = observer;

	auto &res = Resources::getInstance();

	m_sprite.reset(new Sprite(x, y, layer, width, height,
		res.getTexture(config::gui_data::button)));
	m_highlight.reset(new Sprite(x, y, layer + 1, width, height,
		res.getTexture(config::gui_data::highlight)));

	FontMeshBuilder &fontMeshBuilder = res.getFontMeshBuilder(
			config::font_data::fontLayout,
			config::font_data::fontAtlasWidth,
			config::font_data::fontAtlasHeight);

	m_text.reset(new Sprite(x, y + 5, layer + 1,
		fontMeshBuilder.buldMeshForString(name, height - 5),
		res.getTexture(config::font_data::font)));

}

// ########################################################
// Member Functions########################################
// ########################################################

std::string Button::getName()
{
	return m_name;
}

void Button::draw()
{
	if (m_pointerInsideBorders) {
		SpriteBatcher::getInstance().addBatch(m_sprite);
		SpriteBatcher::getInstance().addBatch(m_text);
		SpriteBatcher::getInstance().addBatch(m_highlight);
	} else {
		SpriteBatcher::getInstance().addBatch(m_sprite);
		SpriteBatcher::getInstance().addBatch(m_text);
	}
}

void Button::mouseClicked(int button, float x, float y)
{
	if(isInsideBorders(x,y))
		m_observer.operator ()(m_id);

}

void Button::mouseMoved(float x, float y)
{
	m_pointerInsideBorders = isInsideBorders(x, y);
}

void Button::keyTyped(char value)
{
	// not relevant for button, do nothing
}


} /* namespace widget */
