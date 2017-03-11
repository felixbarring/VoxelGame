#ifndef SRC_GUI_MOUSE_H_
#define SRC_GUI_MOUSE_H_

#include <memory>

#include "../graphics/sprite.h"
#include "../gui/guiUtil.h"
#include "../util/input.h"

namespace gui {

class Mouse {
public:

  Mouse();

  void update();

  void draw();

  void lock();

  void unlock();

private:

  std::shared_ptr<util::Input> m_input;
  bool m_locked{false};

  std::shared_ptr<graphics::Sprite> m_sprite;
  int m_width{10};
  int m_height{10};
};

} /* namespace widget */

#endif /* SRC_GUI_MOUSE_H_ */
