#include "CubeBar.h"

using namespace std;
using namespace graphics;

namespace gui {

CubeBar::CubeBar(GraphicsManager& graphicsManager)
  : m_graphicsManager{graphicsManager}
{
  for (int i = 0; i <= config::cube_data::LAST_CUBE_USED_FOR_BUILDING; ++i) {
    m_cubeThumbnails.push_back(make_shared<Sprite>(
      380,
      5,
      2,
      40,
      40,
      Resources::getInstance().getTexture(config::cube_data::thumbnails[i])));
  }

  for (int i = 0; i <= config::cube_data::LAST_CUBE_USED_FOR_BUILDING; ++i) {
    m_smallThumbnails.push_back(make_shared<Sprite>(
      0,
      0,
      2,
      30,
      30,
      Resources::getInstance().getTexture(config::cube_data::thumbnails[i])));
  }
}

void
CubeBar::update(int selectedCube) {
  m_graphicsManager.getSpriteBatcher().addBatch(
    *m_cubeThumbnails[selectedCube]);

  int numberofThunbnails{5};
  for (int i{0}; i < numberofThunbnails; ++i) {
    int current{selectedCube - (numberofThunbnails - i)};
    if (current < 0)
      current += (config::cube_data::LAST_CUBE_USED_FOR_BUILDING + 1);
    shared_ptr<Sprite> sprite = m_smallThumbnails[current];
    int xLocation = i * 35 + (380 - (35 * numberofThunbnails));
    sprite->setLocation(xLocation, 5);
    m_graphicsManager.getSpriteBatcher().addBatch(*sprite);
  }

  for (int i{0}; i < numberofThunbnails; ++i) {
    int current{selectedCube + i + 1};
    if (current > config::cube_data::LAST_CUBE_USED_FOR_BUILDING)
      current -= (config::cube_data::LAST_CUBE_USED_FOR_BUILDING + 1);
    shared_ptr<Sprite> sprite = m_smallThumbnails[current];
    int xLocation = i * 35 + (380 + 40 + 5);
    sprite->setLocation(xLocation, 5);
    m_graphicsManager.getSpriteBatcher().addBatch(*sprite);
  }
}

}
