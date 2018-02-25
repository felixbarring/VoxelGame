#include "widgetGroup.h"

#include <algorithm>
#include <iostream>

#include "../../config/data.h"
#include "../../graphics/graphicsManager.h"
#include "../../graphics/resources.h"

using namespace std;
using namespace graphics;

namespace widget {

Sprite
createWidgetGroupSprite(int x,
                        int y,
                        unsigned layer,
                        int width,
                        int height,
                        const string kek) {
  return Sprite{static_cast<double>(x),
                static_cast<double>(y),
                layer,
                static_cast<double>(width),
                static_cast<double>(height),
                Resources::getInstance().getTexture(kek)};
}

WidgetGroup::WidgetGroup(int id,
                         int x,
                         int y,
                         int width,
                         int height,
                         graphics::GraphicsManager& graphicsManager,
                         unsigned layer,
                         bool transparentBackground)
  : AbstractWidget(id, x, y, width, height, graphicsManager)
  , m_sprite{createWidgetGroupSprite(x,
                                     y,
                                     layer,
                                     width,
                                     height,
                                     transparentBackground
                                       ? config::gui_data::transparentGuiBox
                                       : config::gui_data::guiBox)} {
}

void
WidgetGroup::addWidget(IWidget& widget) {
  m_widgets.push_back(&widget);
}

void
WidgetGroup::addWidget(vector<IWidget*> widgets) {
  for_each(widgets.begin(), widgets.end(), [this](IWidget* w) {
    m_widgets.push_back(w);
  });
}

void
WidgetGroup::draw() {
  m_graphicsManager.getSpriteBatcher().addBatch(m_sprite);
  for (auto& widget : m_widgets)
    widget->draw();
}

void
WidgetGroup::update(float timePassed) {
  for (auto widget : m_widgets)
    widget->update(timePassed);
}

} /* namespace demo */
