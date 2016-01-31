
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

Label::Label(int x, int y, int width, int height, const string &name) :
		AbstractWidget(id, x, y, width, height)
{

	auto &res = Resources::getInstance();

	FontMeshBuilder &fontMeshBuilder = res.getFontMeshBuilder(
			config::font_data::fontLayout,
			config::font_data::fontAtlasWidth,
			config::font_data::fontAtlasHeight);

	shared_ptr<mesh::MeshElement> fontMesh =
			fontMeshBuilder.buldMeshForString(name, height - 5);

	text.reset(new Sprite{x, y+5, 1, fontMesh,
		res.getTexture(config::font_data::font)});

}

// ########################################################
// Member Functions########################################
// ########################################################

void Label::draw()
{
	SpriteBatcher::getInstance().addBatch(text);
}

void Label::mouseClicked(int button, float x, float y)
{
	// Nothing
}

void Label::mouseMoved(float x, float y)
{
	// Nothing
}

void Label::keyTyped(char value)
{
	// Nothing
}


} /* namespace demo */
