#include "cubeBar.h"

#include "config/data.h"
#include "gui/mouse.h"
#include "gui/terminal.h"
#include "util/fpsManager.h"
#include "util/soundPlayer.h"

using namespace std;
using namespace graphics;

const int yPlacement{5};
const int thumbnailWidth{40};
const int smallThumbnailWidth{30};
const int spacing{5};
const int smallThumbnailWidhtWithSpacing{smallThumbnailWidth + spacing};
const int layer{2};
const int selectedXLocation{static_cast<int>(
  config::graphics_data::virtualWidth / 2 - thumbnailWidth / 2)};
const int numberOfThunbnails{5};

namespace gui {

CubeBar::CubeBar(GraphicsManager& graphicsManager)
  : m_graphicsManager{graphicsManager} {

  for (int i{0}; i <= config::cube_data::LAST_CUBE_USED_FOR_BUILDING; ++i) {
    m_cubeThumbnails.push_back(Sprite(
      selectedXLocation,
      yPlacement,
      layer,
      thumbnailWidth,
      thumbnailWidth,
      Resources::getInstance().getTexture(config::cube_data::thumbnails[i])));
  }

  for (int i{0}; i <= config::cube_data::LAST_CUBE_USED_FOR_BUILDING; ++i) {
    m_smallThumbnails.push_back(Sprite(
      0,
      0,
      layer,
      smallThumbnailWidth,
      smallThumbnailWidth,
      Resources::getInstance().getTexture(config::cube_data::thumbnails[i])));
  }
}

void
CubeBar::update(int selectedCube) {
  m_graphicsManager.getSpriteBatcher().addBatch(m_cubeThumbnails[selectedCube]);

  for (int i{0}; i < numberOfThunbnails; ++i) {
    int current{selectedCube - (numberOfThunbnails - i)};
    if (current < 0)
      current += (config::cube_data::LAST_CUBE_USED_FOR_BUILDING + 1);

    Sprite& sprite = m_smallThumbnails[current];
    int xLocation = i * smallThumbnailWidhtWithSpacing +
                    (selectedXLocation -
                     (smallThumbnailWidhtWithSpacing * numberOfThunbnails));

    sprite.setLocation(xLocation, yPlacement);
    m_graphicsManager.getSpriteBatcher().addBatch(sprite);
  }

  for (int i{0}; i < numberOfThunbnails; ++i) {
    int current{selectedCube + i + 1};
    if (current > config::cube_data::LAST_CUBE_USED_FOR_BUILDING)
      current -= (config::cube_data::LAST_CUBE_USED_FOR_BUILDING + 1);

    Sprite& sprite{m_smallThumbnails[current]};
    int xLocation{i * smallThumbnailWidhtWithSpacing +
                  (selectedXLocation + thumbnailWidth + spacing)};

    sprite.setLocation(xLocation, yPlacement);
    m_graphicsManager.getSpriteBatcher().addBatch(sprite);
  }
}
}
