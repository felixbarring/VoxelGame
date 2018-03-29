#include "textInput.h"

#include <iostream>

#include "../../config/data.h"
#include "../../graphics/graphicsManager.h"
#include "../../graphics/mesh/meshElement.h"
#include "../../graphics/resources.h"
#include "../../graphics/spriteBatcher.h"

#include "../../util/input.h"

using namespace std;
using namespace graphics;
using namespace util;

using namespace config;
using namespace gui_data;
using namespace font_data;

namespace widget {

static constexpr int s_cursorSpacing{4};
static constexpr int s_textHightDifference{5};

Sprite
createBackground(int x, int y, unsigned layer, int width, int height) {
  return Sprite(x,
                y,
                layer,
                width,
                height,
                Resources::getInstance().getTexture(solidBlack));
}

Sprite
createCursor(int x, int y, unsigned layer, int height) {
  return Sprite(x,
                y + s_cursorSpacing,
                layer + 1,
                1,
                height - 2 * s_cursorSpacing,
                Resources::getInstance().getTexture(solidWhite));
}

Sprite
createTextTextInput(int x, int y, unsigned layer, int textHeight) {
  FontMeshBuilder& fontMeshBuilder =
    Resources::getInstance().getFontMeshBuilder(
      fontLayout, fontAtlasWidth, fontAtlasHeight);

  shared_ptr<mesh::MeshElement> fontMesh =
    fontMeshBuilder.buldMeshForString("", textHeight);

  return Sprite(x,
                y + s_textHightDifference,
                layer + 1,
                move(fontMesh),
                Resources::getInstance().getTexture(font));
}

TextInput::TextInput(int id,
                     int x,
                     int y,
                     unsigned width,
                     int height,
                     graphics::GraphicsManager& graphicsManager,
                     int layer)
  : AbstractWidget(id, x, y, width, height, graphicsManager)
  , m_layer{layer}
  , m_maxInputLength{width}
  , m_textHeight{height - s_textHightDifference}
  , m_background{createBackground(x, y, layer, width, height)}
  , m_cursor{createCursor(x, y, layer, height)}
  , m_text{createTextTextInput(x, y, layer, m_textHeight)} {
}

void
TextInput::setString(string str) {
  Resources& res = Resources::getInstance();
  m_input = str;

  FontMeshBuilder& fontMeshBuilder =
    res.getFontMeshBuilder(fontLayout, fontAtlasWidth, fontAtlasHeight);

  shared_ptr<mesh::MeshElement> fontMesh =
    fontMeshBuilder.buldMeshForString(m_input, m_textHeight);
  m_text = Sprite(m_xCoordinate,
                  m_yCoordinate + s_textHightDifference,
                  m_layer + 1,
                  fontMesh,
                  res.getTexture(font));

  double strLength = fontMeshBuilder.lenghtOfString(m_input, m_textHeight);
  m_cursor.setLocation(m_xCoordinate + strLength,
                       m_yCoordinate + s_cursorSpacing);
}

string
TextInput::getString() {
  return m_input;
}

void
TextInput::draw() {
  m_graphicsManager.getSpriteBatcher().addBatch(m_background);

  if (m_cursorVissible && m_hasFocus)
    m_graphicsManager.getSpriteBatcher().addBatch(m_cursor);

  m_graphicsManager.getSpriteBatcher().addBatch(m_text);
}

void
TextInput::update(float timePassed) {
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
  Resources& res = Resources::getInstance();
  FontMeshBuilder& fontMeshBuilder =
    res.getFontMeshBuilder(fontLayout, fontAtlasWidth, fontAtlasHeight);

  if (input->keyWasTyped && m_hasFocus) {
    m_input.push_back(input->keyTyped);

    float strLength = fontMeshBuilder.lenghtOfString(m_input, m_textHeight);
    if (strLength > m_maxInputLength) {
      m_input.pop_back();
      return;
    }

    m_text = Sprite(m_xCoordinate,
                    m_yCoordinate + s_textHightDifference,
                    m_layer + 1,
                    fontMeshBuilder.buldMeshForString(
                          m_input, m_height - s_textHightDifference),
                    res.getTexture(font));

    m_cursor.setLocation(m_xCoordinate + strLength,
                         m_yCoordinate + s_cursorSpacing);

  } else {
    if (m_hasFocus && Input::getInstance()->eraseTextActive &&
        m_input.size() > 0) {

      if (!Input::getInstance()->eraseTextPressed &&
          m_accumulatedEraseTime < m_eraseDelay) {
        m_accumulatedEraseTime += timePassed;
        return;
      }

      m_accumulatedEraseTime = 0.0;
      m_input.pop_back();

      m_text = Sprite(m_xCoordinate,
                      m_yCoordinate + s_textHightDifference,
                      m_layer + 1,
                      fontMeshBuilder.buldMeshForString(
                        m_input, m_height - s_textHightDifference),
                      res.getTexture(font));

      float strLenght = fontMeshBuilder.lenghtOfString(m_input, m_textHeight);
      m_cursor.setLocation(m_xCoordinate + strLenght,
                           m_yCoordinate + s_cursorSpacing);
    }
  }
}

void
TextInput::setFocus(bool value) {
  m_hasFocus = value;
}

} /* namespace widget */
