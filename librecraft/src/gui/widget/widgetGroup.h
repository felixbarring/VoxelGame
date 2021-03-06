#ifndef SRC_GUI_WIDGET_WIDGETGROUP_H_
#define SRC_GUI_WIDGET_WIDGETGROUP_H_

#include <memory>
#include <vector>

#include "graphics/graphicsManager.h"
#include "graphics/sprite.h"

#include "abstractWidget.h"
#include "iWidget.h"

namespace widget {

/**
 * @brief Handles a group of widgets.
 *
 * A simple layer that redirects all function calls to its widgets.
 * Useful for handling groups of widgets that work together.
 *
 */
class WidgetGroup : public AbstractWidget {
public:
  /**
   * @brief Constructs a WidgetGroup.
   */
  WidgetGroup(int id,
              int x,
              int y,
              int width,
              int height,
              graphics::GraphicsManager& graphicsManager,
              unsigned layer = 0,
              bool transparentBackground = true);

  virtual ~WidgetGroup() = default;

  /**
   * @brief Adds a widget to this group.
   *
   * All other functions in this class will act on the Widgets
   * added by this function.
   *
   * @param widget The widget that will be added to this group
   */
  void addWidget(IWidget& widget);

  /**
   * @brief Adds a vector of widgets to this group.
   *
   * All other functions in this class will act on the Widgets
   * added by this function.
   *
   * @param widgets The widgets that shall be added to this group
   */
  void addWidget(std::vector<IWidget*> widgets);

  void draw() override;

  void update(float timePassed) override;

private:
  graphics::Sprite m_sprite;
  std::vector<IWidget*> m_widgets;
  std::vector<IWidget> m_widgetsOwned;
};

} /* namespace widget */

#endif /* SRC_GUI_WIDGET_WIDGETGROUP_H_ */
