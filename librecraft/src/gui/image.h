#ifndef SRC_GUI_IMAGE_H_
#define SRC_GUI_IMAGE_H_

#include <string>

#include "../graphics/graphicsManager.h"
#include "../graphics/sprite.h"

namespace gui {

class Image {
public:

  Image(double x, double y, double width, double height,
      const std::string image, graphics::GraphicsManager &graphicsManager);

  void draw();

private:

  graphics::GraphicsManager &m_graphicsManager;
  std::shared_ptr<graphics::Sprite> m_sprite;

};

} /* namespace gui */

#endif /* SRC_GUI_IMAGE_H_ */
