#ifndef SRC_DEMO_TOGGLEBUTTON_H_
#define SRC_DEMO_TOGGLEBUTTON_H_

#include <algorithm>
#include <functional>
#include <vector>

#include "../../graphics/graphicsManager.h"
#include "button.h"

namespace widget {

class ToggleButton : public Button
{
public:
  /**
   * @brief Used to determine the visuals of the button.
   */
  enum class Skin
  {
    //!< Regular visuals representing a button with text drawn on it that can be
    //! check
    Regular,
    //!< CheckBox visuals representing a small box that can be checked and with
    //! text to the right
    CheckBox,
    //!< ReadioButton visuals representing a circle that can be check and with
    //! text to the right
    ReadioButton
  };

  ToggleButton(int id,
               int x,
               int y,
               int width,
               int height,
               graphics::GraphicsManager& graphicsManager,
               std::function<void(int)> observer,
               const std::string& name,
               int layer = 0,
               Skin skin = Skin::Regular);

  virtual ~ToggleButton() = default;

  bool isToggled();

  void toggle();

  void setUntoggled();

  void setToggled();

  void draw() override;

  void update(float timePassed) override;

private:
  bool m_toggled{ false };
  Skin m_skin;

  std::shared_ptr<graphics::Sprite> m_spriteToggled;
};

} /* namespace widget */

#endif /* SRC_DEMO_TOGGLEBUTTON_H_ */
