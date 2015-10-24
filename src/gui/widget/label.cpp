
#include "label.h"

#include "../../graphics/spriteBatcher.h"
#include "../../graphics/fontMeshBuilder.h"
#include "../../graphics/mesh/meshElement.h"
#include "../../config/data.h"
#include "../../graphics/resources.h"

namespace widget {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Label::Label(int x, int y, int width, int height, const std::string &name) :
		AbstractWidget(id, x, y, width, height)
{
	graphics::FontMeshBuilder &fontMeshBuilder = graphics::Resources::getInstance().getFontMeshBuilder(
			config::font_data::fontLayout, config::font_data::fontAtlasWidth, config::font_data::fontAtlasHeight);

	std::shared_ptr<mesh::MeshElement> fontMesh = fontMeshBuilder.buldMeshForString(name, height - 5);

	text.reset(new graphics::Sprite{x, y+5, 1, fontMesh, graphics::Resources::getInstance().getTexture(config::font_data::font)});

}

// ########################################################
// Member Functions########################################
// ########################################################

void Label::draw()
{
	graphics::SpriteBatcher::getInstance().addBatch(text);
}

void Label::mouseClicked(int button, float x, float y)
{
	// Nothing
}

void Label::mouseMoved(float x, float y)
{
	// Nothing
}

void Label::keyPressed(int key)
{
	// Nothing
}

void Label::keyTyped(char value)
{
	// Nothing
}


} /* namespace demo */
