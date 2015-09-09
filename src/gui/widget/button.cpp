
#include "button.h"

#include "../../graphics/spriteBatcher.h"
#include "../../graphics/fontMeshBuilder.h"
#include "../../graphics/textureResources.h"
#include "../../graphics/mesh/meshElement.h"
#include "../../config/data.h"

namespace widget {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Button::Button(int id, int x, int y, int width, int height, std::function<void(int)> observer, const std::string &name) :
	AbstractWidget(id, x, y, width, height)
{

	this->observer = observer;

	sprite.reset(new graphics::Sprite{x, y, 0, width, height, graphics::TextureResources::getInstance().getTexture(config::gui_data::guiBox)});
	spriteHover.reset(new graphics::Sprite{x, y, 0, width, height, graphics::TextureResources::getInstance().getTexture(config::gui_data::guiBox)});

	// Share these some how
	graphics::FontMeshBuilder fontBuilder{config::font_data::fontLayout,
			config::font_data::fontAtlasWidth, config::font_data::fontAtlasHeight};

	std::shared_ptr<mesh::MeshElement> blool = fontBuilder.buldMeshForString(name, height-5);

	text.reset(new graphics::Sprite{x, y+5, 1, blool, graphics::TextureResources::getInstance().getTexture(config::font_data::font)});

}

Button::~Button()
{
}

// ########################################################
// Member Functions########################################
// ########################################################

void Button::draw()
{
	if (pointerInsideBorders) {
		graphics::SpriteBatcher::getInstance().addBatch(text);
		graphics::SpriteBatcher::getInstance().addBatch(sprite);
		graphics::SpriteBatcher::getInstance().addBatch(spriteHover);
	} else {
		graphics::SpriteBatcher::getInstance().addBatch(text);
		graphics::SpriteBatcher::getInstance().addBatch(sprite);
	}
}

void Button::mouseClicked(int button, float x, float y)
{
	if(isInsideBorders(x,y)){
		observer.operator ()(id);
	}
}

void Button::mouseMoved(float x, float y)
{
	pointerInsideBorders = isInsideBorders(x, y);
}

void Button::keyPressed(int key, char c)
{
	// not relevant for button, do nothing
}


} /* namespace widget */
