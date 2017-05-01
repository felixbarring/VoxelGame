#ifndef SRC_GUI_WIDGET_BUTTON_H_
#define SRC_GUI_WIDGET_BUTTON_H_

#include <string>
#include <memory>

#include "../../graphics/graphicsManager.h"
#include "abstractWidget.h"

#include "../../graphics/sprite.h"

namespace widget {

/**
 * \brief A button that lets the user interact with the GUI.
 *
 * A button can be used to trigger an event when the user clicks on it.
 * Buttons can have a text/name that will be rendered and hence provide
 * the user with visual information about the purpose of the button.
 *
 * When the mouse pointer is inside the buttons border it will hint
 * that it is interactable by highlight it self.
 *
 */
class Button: public AbstractWidget {
public:

  Button(int id, int x, int y, int width, int height,
      graphics::GraphicsManager &graphicsManager,
      std::function<void(int)> observer, std::string name, int layer = 0);

  virtual ~Button() = default;

  /**
   * @return The name of the button.
   */
  std::string getName();

  void draw() override;

  void update(float timePassed) override;

  /**
   * @brief Makes the button behave as if had been clicked.
   *
   * This will cause the button to call the callback observer function
   * registered in the ctor.
   */
  void trigger();

protected:

  bool m_pointerInsideBorders{false};
  std::string m_name;
  std::function<void(int)> m_observer;

  std::shared_ptr<graphics::Sprite> m_sprite;
  std::shared_ptr<graphics::Sprite> m_highlight;
  std::shared_ptr<graphics::Sprite> m_text;
};

} /* namespace widget */

#endif /* SRC_GUI_WIDGET_BUTTON_H_ */
