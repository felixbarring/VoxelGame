
#include "button.h"

#include "../../graphics/spriteBatcher.h"
#include "../../graphics/textureResources.h"
#include "../../config/data.h"

namespace widget {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Button::Button(int id, int x, int y, int width, int height) :
	AbstractWidget(id, x, y, width, height)
{
	sprite.reset(new graphics::Sprite{x, y, 0, width, height, graphics::TextureResources::getInstance().getTexture(0)});
	spriteHover.reset(new graphics::Sprite{x, y, 0, width, height, graphics::TextureResources::getInstance().getTexture(1)});
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
		graphics::SpriteBatcher::getInstance().addBatch(spriteHover);
	} else {
		graphics::SpriteBatcher::getInstance().addBatch(sprite);
	}
}

void Button::mouseClicked(int button, float x, float y)
{
	if(isInsideBorders(x,y)){

		// Communicate to listener
		//BUTTON_LISTENER.onButtonPressed(ID);

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
