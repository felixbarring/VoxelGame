#ifndef SRC_GUI_WIDGET_SLIDER_H_
#define SRC_GUI_WIDGET_SLIDER_H_

#include <memory>

#include "abstractWidget.h"
#include "../../graphics/sprite.h"

namespace widget {

/**
 * @brief A widget that can be used to change a value.
 *
 * The slider consists of a knob and a horizontal line that the knob can be
 * placed on. The knobs position can be changed by pressing on it and dragging
 * it on the line. The user can also just press on the line and the knob will
 * place itself on that position. When the knob is the farthest to the left, it
 * represents the value 0. When the knob is positioned the farthest to the right
 * it represents the value 1. It will linearly change value between these two
 * positions.
 */
class Slider: public AbstractWidget {
public:

  /**
   * @brief Constructs a slider instance.
   *
   * @param id The id can be used by the user to identify what Widget it was
   *           that called a callback observer function.
   * @param x The x position of the slider.
   * @param y The y position of the slider.
   * @param width The width of the slider.
   * @param height The height of the slider.
   * @param observer The callback function that will be called whenever
   *                 the knob changes position and hence the slider value.
   * @param layer The layer that the widget shall be rendered on.
   *              the knob will be drawn with layer + 1 and the line will be
   *              drawn with the layer value.
   */
  Slider(int id, int x, int y, int width, int height,
      std::function<void(int)> observer, int layer = 0);

  virtual ~Slider() = default;

  /**
   * @brief Sets the value of the slider.
   *
   * This will effectively change the position of the knob. The values that the
   * slider represents are in the range of [0, 1]. The value will be 0 when
   * the knob is farthest possible to the left, and 1 when farthest to the
   * right.
   *
   * @param value Should be in the range of [0, 1].
   */
  void setValue(double value);

  /**
   * @return Returns the value that the current knob position represents. Will
   *         be in the range of [0, 1].
   */
  double getValue();

  void draw() override;

  void update(float timePassed) override;

private:

  std::function<void(int)> m_observer;
  bool m_pointerInsideBorders{false};

  std::shared_ptr<graphics::Sprite> m_slider{};
  std::shared_ptr<graphics::Sprite> m_knob{};

  float m_knobPosition{};
  float m_knobWidth{};
  bool m_grabbed{false};
};

} /* namespace widget */

#endif /* SRC_GUI_WIDGET_SLIDER_H_ */
