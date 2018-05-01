#include "selectableList.h"

#include "config/data.h"
#include "graphics/fontMeshBuilder.h"
#include "graphics/mesh/meshElement.h"
#include "graphics/resources.h"
#include "graphics/spriteBatcher.h"
#include "graphics/graphicsManager.h"

#include "util/input.h"

#include <functional>
#include <iostream>

using namespace std;
using namespace graphics;

namespace {

Sprite
createSprite(int x, int y, unsigned layer, int width, int height) {
  return Sprite(static_cast<double>(x),
                static_cast<double>(y),
                layer,
                static_cast<double>(width),
                static_cast<double>(height),
                Resources::getInstance().getTexture(config::gui_data::button));
}
}

namespace widget {

SelectableList::SelectableList(int id,
                               int x,
                               int y,
                               int width,
                               int height,
                               graphics::GraphicsManager& graphicsManager,
                               function<void(int)> observer,
                               unsigned layer)
  : AbstractWidget(id,
                   static_cast<double>(x),
                   static_cast<double>(y),
                   width,
                   height,
                   graphicsManager)
  , m_layer{layer + 1}
  , m_observer{observer}
  , m_sprite{createSprite(x, y, layer, width, height)} {
}

void
SelectableList::addListItem(std::string item) {

  // TODO Do not add item that already exists.
  int x{m_xCoordinate + 5};
  int y = m_yCoordinate + m_height - (5 + 30 * (1 + m_buttons.size()));
  int width{m_width - 10};
  int height{30};
  auto func = [&](int id) {
    ToggleButton* button = getButtonWithId(id);
    if (button->isToggled()) {
      // If there already is a button toggled, untoggle it.
      if (m_currentlyToggled)
        m_currentlyToggled->toggle();

      m_currentlyToggled = button;
    } else {
      // No button is toggled.
      m_currentlyToggled = nullptr;
    }
  };

  ToggleButton button = ToggleButton(
    ++idCounter, x, y, width, height, m_graphicsManager, func, item, m_layer);
  m_buttons.push_back(std::move(button));
}

void
SelectableList::deleteListItem(std::string) {
  // TODO Implement
}

void
SelectableList::clear() {
  m_buttons.clear();
}

std::string
SelectableList::getSelectedListItem() {
  if (m_currentlyToggled && m_currentlyToggled->isToggled())
    return m_currentlyToggled->getName();

  return "";
}

void
SelectableList::reset() {
  if (m_currentlyToggled && m_currentlyToggled->isToggled())
    m_currentlyToggled->toggle();
}

void
SelectableList::draw() {
  m_graphicsManager.getSpriteBatcher().addBatch(m_sprite);
  for (auto& b : m_buttons)
    b.draw();
}

void
SelectableList::update(float timePassed) {
  for (auto& b : m_buttons)
    b.update(timePassed);
}

ToggleButton*
SelectableList::getButtonWithId(int id) {
  for (auto& b : m_buttons) {
    if (b.getId() == id)
      return &b;
  }
  return nullptr; // Should never happen
}

} /* namespace widget */
