#include "widgetGroup.h"

#include <algorithm>
#include <iostream>

#include "../../config/data.h"
#include "../../graphics/resources.h"
#include "../../graphics/graphicsManager.h"

using namespace std;
using namespace graphics;

namespace widget {

WidgetGroup::WidgetGroup(int id, int x, int y, int width, int height,
    graphics::GraphicsManager &graphicsManager, unsigned layer,
    bool transparentBackground)
    : AbstractWidget(id, x, y, width, height, graphicsManager)
{
  if (transparentBackground) {
    m_sprite.reset(
        new Sprite{static_cast<double>(x), static_cast<double>(y), layer,
            static_cast<double>(width), static_cast<double>(height),
            Resources::getInstance().getTexture(
                config::gui_data::transparentGuiBox)});
  } else {
    m_sprite.reset(new Sprite{static_cast<double>(x), static_cast<double>(y),
        layer, static_cast<double>(width), static_cast<double>(height),
        Resources::getInstance().getTexture(config::gui_data::guiBox)});
  }
}

void WidgetGroup::addWidget(shared_ptr<IWidget> widget) {
  m_widgets.push_back(widget);
}

void WidgetGroup::addWidget(std::vector<shared_ptr<IWidget>> widgets) {
  for_each(widgets.begin(), widgets.end(),
      [this] (shared_ptr<IWidget> w) {m_widgets.push_back(w);});
}

void WidgetGroup::draw() {
  m_graphicsManager.getSpriteBatcher().addBatch(m_sprite);
  for (auto widget : m_widgets)
    widget->draw();
}

void WidgetGroup::update(float timePassed) {
  for (auto widget : m_widgets)
    widget->update(timePassed);
}

} /* namespace demo */
