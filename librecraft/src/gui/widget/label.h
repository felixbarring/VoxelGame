#ifndef SRC_GUI_WIDGET_LABEL_H_
#define SRC_GUI_WIDGET_LABEL_H_

#include <string>

#include "../../graphics/graphicsManager.h"
#include "abstractWidget.h"

#include "../../graphics/sprite.h"

namespace widget {

/**
 * @brief A label can be used to provide information in GUIs by displaying text.
 */
class Label : public AbstractWidget
{
public:
  /**
   * @brief Constructs a Label instance.
   *
   * @param x The x position of the label.
   * @param y The y position of the label.
   * @param width The width of the label.
   * @param height The height of the label.
   * @param name The name that will be rendered by the label.
   * @param layer The layer that the label shall be drawn at.
   */
  Label(int x,
        int y,
        int width,
        int height,
        const std::string& name,
        graphics::GraphicsManager& graphicsManager,
        unsigned layer = 0);

  virtual ~Label() = default;

  void draw() override;

private:
  std::shared_ptr<graphics::Sprite> m_text;
};

} /* namespace widget */

#endif /* SRC_GUI_WIDGET_LABEL_H_ */
