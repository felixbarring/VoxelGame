
#include "mouse.h"

#include <glm.hpp>

#include "../graphics/resources.h"
#include "../util/input.h"
#include "../config/data.h"
#include "../graphics/spriteBatcher.h"
#include "../gui/guiUtil.h"

using namespace std;
using namespace graphics;

namespace widget {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Mouse::Mouse()
{
	m_sprite.reset(new Sprite(380, 280, 100, m_width, m_height,
			Resources::getInstance().getTexture(config::gui_data::mouse)));
}

// ########################################################
// Member Functions########################################
// ########################################################

void Mouse::update()
{
	shared_ptr<util::Input> input = util::Input::getInstance();

	double y = input->mouseYPosition - config::graphics_data::windowHeight;
	if (y < 0)
		y = -y;
	else
		y = -1;

	glm::vec2 mouse = gui::adjustMouse(
			config::graphics_data::virtualWidth,
			config::graphics_data::virtualHeight,
			config::graphics_data::windowWidth,
			config::graphics_data::windowHeight,
			input->mouseXPosition, y);

	input->mouseVirtualAdjustedX = mouse.x;
	input->mouseVirtualAdjustedY = mouse.y;

	m_sprite->setLocation(
			mouse.x + 5, mouse.y - 5, 0);
}

void Mouse::draw()
{
	SpriteBatcher::getInstance().addBatch(m_sprite);
}

} /* namespace widget */
