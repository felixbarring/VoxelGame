#ifndef SRC_GUI_WIDGET_TEXTAREA_H_
#define SRC_GUI_WIDGET_TEXTAREA_H_

#include <functional>
#include <list>
#include <memory>
#include <string>

#include "../../graphics/sprite.h"
#include "abstractWidget.h"

namespace widget {

class TextArea : public AbstractWidget {
public:
  TextArea(int id,
           int x,
           int y,
           int width,
           int height,
           graphics::GraphicsManager& graphicsManager,
           std::function<void(int)> observer,
           int layer);

  virtual ~TextArea() = default;

  void draw() override;

  void update(float timePassed) override;

  /**
   * \brief Adds the content on the current line
   *
   * If the content does not fit, the first word that goes
   * outside the bounds will be placed on a new row together
   * with the remaining words. This repeats if the added string
   * requires several rows.
   *
   * @param str The string to be added to the text area
   */
  void add(std::string str);

  /**
   * \brief Adds the content on a new line
   *
   * @param str The string that will be added on a new line
   */
  void addLine(std::string str);

private:
  const unsigned m_fontHeight{20};

  int m_layer;

  std::function<void(int)> m_observer;

  std::shared_ptr<graphics::Sprite> m_textArea;
  std::list<std::pair<std::string, std::shared_ptr<graphics::Sprite>>> m_rows;
};
}
/* namespace widget */

#endif /* SRC_GUI_WIDGET_TEXTAREA_H_ */
