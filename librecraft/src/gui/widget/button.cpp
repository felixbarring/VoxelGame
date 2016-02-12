
#include "button.h"

#include "../../graphics/spriteBatcher.h"
#include "../../graphics/fontMeshBuilder.h"
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
	SpriteBatcher &spriteBatcher(SpriteBatcher::getInstance());

	if (m_pointerInsideBorders) {
		spriteBatcher.addBatch(m_sprite);
		spriteBatcher.addBatch(m_text);
		spriteBatcher.addBatch(m_highlight);
	} else {
		spriteBatcher.addBatch(m_sprite);
		spriteBatcher.addBatch(m_text);
	}
}

void Button::update(float timePassed)
{
	shared_ptr<Input> input{Input::getInstance()};

	m_pointerInsideBorders = isInsideBorders(
			input->mouseVirtualAdjustedX,
			input->mouseVirtualAdjustedY);

	if (m_pointerInsideBorders && input->action1Pressed)
		m_observer.operator ()(m_id);
}


} /* namespace widget */
