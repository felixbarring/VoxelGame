#include "label.h"

#include "../../config/data.h"
#include "../../graphics/fontMeshBuilder.h"
#include "../../graphics/graphicsManager.h"
#include "../../graphics/mesh/meshElement.h"
#include "../../graphics/resources.h"
#include "../../graphics/spriteBatcher.h"

using namespace std;
using namespace graphics;

namespace widget {

graphics::Sprite
createLabelText(int x, int y, int height, string name, unsigned layer) {
  auto& res = Resources::getInstance();

  FontMeshBuilder& fontMeshBuilder =
    res.getFontMeshBuilder(config::font_data::fontLayout,
                           config::font_data::fontAtlasWidth,
                           config::font_data::fontAtlasHeight);

  shared_ptr<mesh::MeshElement> fontMesh =
    fontMeshBuilder.buldMeshForString(name, height - 5);

  return Sprite{static_cast<double>(x),
                y + 5.0,
                layer,
                fontMesh,
                res.getTexture(config::font_data::font)};
}

Label::Label(int x,
             int y,
             int width,
             int height,
             const string& name,
             graphics::GraphicsManager& graphicsManager,
             unsigned layer)
  : AbstractWidget(1, x, y, width, height, graphicsManager)
  , m_text{createLabelText(x, y, height, name, layer)} {
}

void
Label::draw() {
  m_graphicsManager.getSpriteBatcher().addBatch(m_text);
}

} /* namespace demo */
