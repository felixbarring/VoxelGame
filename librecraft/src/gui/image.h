#ifndef SRC_GUI_IMAGE_H_
#define SRC_GUI_IMAGE_H_

#include <string>

#include "../graphics/sprite.h"

namespace gui {

class Image {
public:

  Image(double x, double y, double width, double height,
      const std::string image);

  void draw();

private:

  std::shared_ptr<graphics::Sprite> sprite;

};

} /* namespace gui */

#endif /* SRC_GUI_IMAGE_H_ */
