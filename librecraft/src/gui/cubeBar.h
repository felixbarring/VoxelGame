
#ifndef SOURCE_DIRECTORY__SRC_GUI_CUBEBAR_H_
#define SOURCE_DIRECTORY__SRC_GUI_CUBEBAR_H_

#include <vector>

#include "graphics/graphicsManager.h"
#include "graphics/sprite.h"

namespace gui {

class CubeBar {
public:
  CubeBar(graphics::GraphicsManager& graphicsManager);

  void update(int selectedCube);

private:
  graphics::GraphicsManager& m_graphicsManager;
  std::vector<graphics::Sprite> m_cubeThumbnails;
  std::vector<graphics::Sprite> m_smallThumbnails;
};
}

#endif /* SOURCE_DIRECTORY__SRC_GUI_CUBEBAR_H_ */
