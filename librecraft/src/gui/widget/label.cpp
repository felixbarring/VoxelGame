#include "label.h"

#include "../../graphics/spriteBatcher.h"
#include "../../graphics/fontMeshBuilder.h"
#include "../../graphics/mesh/meshElement.h"
#include "../../config/data.h"
#include "../../graphics/resources.h"

using namespace std;
using namespace graphics;

namespace widget {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Label::Label(int x, int y, int width, int height, const string &name, unsigned layer)
    : AbstractWidget(m_id, x, y, width, height)
{

	auto &res = Resources::getInstance();

	FontMeshBuilder &fontMeshBuilder = res.getFontMeshBuilder(
			config::font_data::fontLayout, config::font_data::fontAtlasWidth,
			config::font_data::fontAtlasHeight);

	shared_ptr<mesh::MeshElement> fontMesh = fontMeshBuilder.buldMeshForString(name, height - 5);

	m_text.reset(new Sprite{static_cast<double>(x), y + 5.0, layer, fontMesh,
	    res.getTexture(config::font_data::font)});

}

// ########################################################
// Member Functions########################################
// ########################################################

void Label::draw() {
	SpriteBatcher::getInstance().addBatch(m_text);
}

} /* namespace demo */
