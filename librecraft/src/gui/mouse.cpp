#include "mouse.h"

#include <glm.hpp>
#include <iostream>

#include "../graphics/resources.h"
#include "../util/input.h"
#include "../config/data.h"
#include "../graphics/graphicsManager.h"
#include "../graphics/spriteBatcher.h"

using namespace std;
using namespace graphics;

namespace gui {

Mouse::Mouse() {
  m_input = util::Input::getInstance();

  m_sprite.reset(
      new Sprite(0, 0, 100, m_width, m_height,
          Resources::getInstance().getTexture(config::gui_data::mouse)));
}

void Mouse::update() {
  double y = m_input->mouseYPosition - config::graphics_data::windowHeight;
  if (y < 0)
    y = -y;
  else
    y = -1;

  glm::vec2 mouse = gui::adjustMouse(config::graphics_data::virtualWidth,
      config::graphics_data::virtualHeight, config::graphics_data::windowWidth,
      config::graphics_data::windowHeight, m_input->mouseXPosition, y);

  m_input->mouseVirtualAdjustedX = mouse.x;
  m_input->mouseVirtualAdjustedY = mouse.y;

  m_sprite->setLocation(mouse.x, mouse.y - m_height);
}

void Mouse::draw() {
  GraphicsManager::getInstance().getSpriteBatcher().addBatch(m_sprite);
}

void Mouse::lock() {
  m_input->lockMouse();
  m_locked = true;
}

void Mouse::unlock() {
  m_input->unlockMouse();
  m_locked = false;
}

} /* namespace widget */
