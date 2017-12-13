#ifndef SRC_GUI_MOUSE_H_
#define SRC_GUI_MOUSE_H_

#include <memory>

#include "../graphics/graphicsManager.h"
#include "../graphics/resources.h"
#include "../graphics/sprite.h"
#include "../gui/guiUtil.h"
#include "../util/input.h"

using graphics::Resources;

namespace gui {

class Mouse {
public:
  explicit Mouse(graphics::GraphicsManager& graphicsManager);

  void update();

  void draw();

  void lock();

  void unlock();

private:
  graphics::GraphicsManager& m_graphicsManager;

  std::shared_ptr<util::Input> m_input;
  bool m_locked{ false };

  int m_width{ 10 };
  int m_height{ 10 };

  graphics::Sprite m_sprite{ 0,
                             0,
                             100,
                             static_cast<double>(m_width),
                             static_cast<double>(m_height),
                             Resources::getInstance().getTexture(
                               config::gui_data::mouse) };
};

} /* namespace widget */

#endif /* SRC_GUI_MOUSE_H_ */
