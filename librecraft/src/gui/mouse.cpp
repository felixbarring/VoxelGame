
#include "mouse.h"

#include <glm.hpp>

#include "../graphics/resources.h"
#include "../util/input.h"
#include "../config/data.h"
#include "../graphics/spriteBatcher.h"

using namespace std;
using namespace graphics;

namespace widget {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Mouse::Mouse()
{
	m_input = util::Input::getInstance();

	m_sprite.reset(new Sprite(380, 280, 100, m_width, m_height,
			Resources::getInstance().getTexture(config::gui_data::mouse)));
}

// ########################################################
// Member Functions########################################
// ########################################################

void Mouse::update()
{
	double y = m_input->mouseYPosition - config::graphics_data::windowHeight;
	if (y < 0)
		y = -y;
	else
		y = -1;

	glm::vec2 mouse = gui::adjustMouse(
			config::graphics_data::virtualWidth,
			config::graphics_data::virtualHeight,
			config::graphics_data::windowWidth,
			config::graphics_data::windowHeight,
			m_input->mouseXPosition, y);

	m_input->mouseVirtualAdjustedX = mouse.x;
	m_input->mouseVirtualAdjustedY = mouse.y;

	m_sprite->setLocation(
			mouse.x + m_width / 2, mouse.y - m_height / 2, 0);
}

void Mouse::draw()
{
	SpriteBatcher::getInstance().addBatch(m_sprite);
}

void Mouse::lock()
{
	m_input->lockMouse();
	m_locked = true;
}

void Mouse::unlock()
{
	m_input->unlockMouse();
	m_locked = false;
}

} /* namespace widget */
