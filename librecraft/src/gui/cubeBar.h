
#ifndef SOURCE_DIRECTORY__SRC_GUI_CUBEBAR_H_
#define SOURCE_DIRECTORY__SRC_GUI_CUBEBAR_H_

#include <vector>

#include "../config/data.h"
#include "../graphics/graphicsManager.h"
#include "../gui/mouse.h"
#include "../gui/terminal.h"
#include "../util/fpsManager.h"
#include "../util/soundPlayer.h"

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
