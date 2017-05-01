#include "textInput.h"

#include <iostream>

#include "../../graphics/spriteBatcher.h"
#include "../../graphics/mesh/meshElement.h"
#include "../../config/data.h"
#include "../../graphics/graphicsManager.h"
#include "../../graphics/resources.h"

#include "../../util/input.h"

using namespace std;
using namespace graphics;
using namespace util;

using namespace config;
using namespace gui_data;
using namespace font_data;

namespace widget {

TextInput::TextInput(int id, int x, int y, unsigned width, int height,
    graphics::GraphicsManager &graphicsManager, int layer)
    : AbstractWidget(id, x, y, width, height, graphicsManager)
    , m_layer{layer}
    , m_maxInputLength{width}
    , m_textHeight{height - s_textHightDifference}
{

  auto &res = Resources::getInstance();

  m_background = make_shared<Sprite>(x, y, m_layer, width, height,
      res.getTexture(solidBlack));
  m_cursor = make_shared<Sprite>(x, y + s_cursorSpacing, m_layer + 1, 1,
      height - 2 * s_cursorSpacing, res.getTexture(solidWhite));

  auto &fontMeshBuilder = res.getFontMeshBuilder(fontLayout, fontAtlasWidth,
      fontAtlasHeight);

  auto fontMesh = fontMeshBuilder.buldMeshForString(m_input, m_textHeight);
  m_text = make_shared<Sprite>(x, y + s_textHightDifference, layer + 1,
      move(fontMesh), res.getTexture(font));

}

void TextInput::setString(string str) {
  Resources &res = Resources::getInstance();
  m_input = str;

  auto &fontMeshBuilder = res.getFontMeshBuilder(fontLayout, fontAtlasWidth,
      fontAtlasHeight);

  auto fontMesh = fontMeshBuilder.buldMeshForString(m_input, m_textHeight);
  m_text = make_shared<Sprite>(m_xCoordinate,
      m_yCoordinate + s_textHightDifference, m_layer + 1, fontMesh,
      res.getTexture(font));

  auto strLength = fontMeshBuilder.lenghtOfString(m_input, m_textHeight);
  m_cursor->setLocation(m_xCoordinate + strLength,
      m_yCoordinate + s_cursorSpacing);
}

string TextInput::getString() {
  return m_input;
}

void TextInput::draw() {
  m_graphicsManager.getSpriteBatcher().addBatch(m_background);

  if (m_cursorVissible && m_hasFocus)
    m_graphicsManager.getSpriteBatcher().addBatch(m_cursor);

  m_graphicsManager.getSpriteBatcher().addBatch(m_text);
}

void TextInput::update(float timePassed) {
  shared_ptr<Input> input = Input::getInstance();

  // Blinking cursor
  m_blinkTime += timePassed;
  if (m_blinkTime > m_blinkIntervall) {
    m_cursorVissible = !m_cursorVissible;
    m_blinkTime = 0;
  }

  if (input->action1Pressed)
    m_hasFocus = isInsideBorders(input->mouseVirtualAdjustedX,
        input->mouseVirtualAdjustedY);

  // Need a better way to handle resources
  Resources &res = Resources::getInstance();
  FontMeshBuilder &fontMeshBuilder = res.getFontMeshBuilder(fontLayout,
      fontAtlasWidth, fontAtlasHeight);

  if (input->keyWasTyped && m_hasFocus) {
    m_input.push_back(input->keyTyped);

    auto strLength = fontMeshBuilder.lenghtOfString(m_input, m_textHeight);
    if (strLength > m_maxInputLength) {
      m_input.pop_back();
      return;
    }

    auto fontMesh = fontMeshBuilder.buldMeshForString(m_input,
        m_height - s_textHightDifference);
    m_text = make_shared<Sprite>(m_xCoordinate,
        m_yCoordinate + s_textHightDifference, m_layer + 1, move(fontMesh),
        res.getTexture(font));

    m_cursor->setLocation(m_xCoordinate + strLength,
        m_yCoordinate + s_cursorSpacing);

  } else {
    if (m_hasFocus && Input::getInstance()->eraseTextActive
        && m_input.size() > 0) {

      if (!Input::getInstance()->eraseTextPressed
          && m_accumulatedEraseTime < m_eraseDelay) {
        m_accumulatedEraseTime += timePassed;
        return;
      }

      m_accumulatedEraseTime = 0.0;
      m_input.pop_back();

      auto fontMesh = fontMeshBuilder.buldMeshForString(m_input,
          m_height - s_textHightDifference);
      m_text = make_shared<Sprite>(m_xCoordinate,
          m_yCoordinate + s_textHightDifference, m_layer + 1, fontMesh,
          res.getTexture(font));

      auto strLenght = fontMeshBuilder.lenghtOfString(m_input, m_textHeight);
      m_cursor->setLocation(m_xCoordinate + strLenght,
          m_yCoordinate + s_cursorSpacing);
    }
  }

}

void TextInput::setFocus(bool value) {
  m_hasFocus = value;
}

} /* namespace widget */
