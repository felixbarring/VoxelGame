#ifndef SRC_GUI_WIDGET_TEXTINPUT_H_
#define SRC_GUI_WIDGET_TEXTINPUT_H_

#include "abstractWidget.h"

#include <memory>
#include <string>

#include "graphics/fontMeshBuilder.h"
#include "graphics/graphicsManager.h"
#include "graphics/sprite.h"

namespace widget {

/**
 *
 */
class TextInput : public AbstractWidget {
public:
  /**
   *
   * @param id
   * @param x
   * @param y
   * @param width
   * @param height
   * @param layer
   */
  TextInput(int id,
            int x,
            int y,
            unsigned width,
            int height,
            graphics::GraphicsManager& graphicsManager,
            int layer = 0);

  /**
   *
   */
  virtual ~TextInput() = default;

  /**
   *
   * @param str
   */
  void setString(std::string str);

  /**
   *
   * @return
   */
  std::string getString();

  /**
   *
   */
  void draw() override;

  /**
   *
   * @param timePassed
   */
  void update(float timePassed) override;

  /**
   *
   * @param value
   */
  void setFocus(bool value = true);

private:
  int m_layer{};
  bool m_hasFocus{false};
  std::string m_input{""};
  unsigned m_maxInputLength;

  float m_eraseDelay{0.1f};
  float m_accumulatedEraseTime{0.0f};

  float m_blinkIntervall{1.0 / 3.0}; // Blink three times / second
  float m_blinkTime{0.0};
  bool m_cursorVissible{true};

  int m_textHeight{};

  graphics::Sprite m_background;
  graphics::Sprite m_cursor;
  graphics::Sprite m_text;
};

} /* namespace widget */

#endif /* SRC_GUI_WIDGET_TEXTINPUT_H_ */
