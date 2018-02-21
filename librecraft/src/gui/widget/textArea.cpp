#include "textArea.h"

#include <functional>

#include "../../graphics/graphicsManager.h"
#include "../../graphics/resources.h"
#include "../../graphics/sprite.h"
#include "../../graphics/spriteBatcher.h"

using namespace std;
using namespace graphics;

namespace widget {

TextArea::TextArea(int id,
                   int x,
                   int y,
                   int width,
                   int height,
                   graphics::GraphicsManager& graphicsManager,
                   std::function<void(int)> observer,
                   int layer)
  : AbstractWidget(id, x, y, width, height, graphicsManager)
  , m_layer{layer} {
  this->m_observer = observer;
  auto& res = Resources::getInstance();

  m_textArea =
    make_shared<Sprite>(x,
                        y,
                        layer,
                        width,
                        height,
                        res.getTexture(config::gui_data::transparentGuiBox));
}

void
TextArea::draw() {
  SpriteBatcher& spriteBatcher{m_graphicsManager.getSpriteBatcher()};
  spriteBatcher.addBatch(*m_textArea);

  for (auto& s : m_rows)
    spriteBatcher.addBatch(*s.second);
}

void
TextArea::update(float) {
  // Nothing to do yet
}

void
TextArea::add(string str) {
  if (m_rows.size()) {
    str = m_rows.back().first + str;
    m_rows.pop_back();
  }
  addLine(str);
}

void
TextArea::addLine(string str) {
  if (str.empty())
    return;

  auto& res = Resources::getInstance();
  FontMeshBuilder& fontMeshBuilder =
    res.getFontMeshBuilder(config::font_data::fontLayout,
                           config::font_data::fontAtlasWidth,
                           config::font_data::fontAtlasHeight);

  int y = m_yCoordinate + m_height - (m_rows.size() + 1) * m_fontHeight;

  int split = fontMeshBuilder.splitStringAt(str, m_fontHeight, m_width);
  string cutOff;
  if (split != -1) {
    cutOff = str.substr(split, str.size() - 1);
    str = str.substr(0, split);
  }

  m_rows.push_back(std::pair<string, shared_ptr<Sprite>>(
    str,
    make_shared<Sprite>(m_xCoordinate,
                        y,
                        m_layer + 1,
                        fontMeshBuilder.buldMeshForString(str, m_fontHeight),
                        res.getTexture(config::font_data::font))));

  if (y < m_yCoordinate) {
    m_rows.pop_front();
    for (auto sprite : m_rows)
      sprite.second->move(0, m_fontHeight);
  }

  if (!cutOff.empty())
    addLine(cutOff);
}

} /* namespace widget */
