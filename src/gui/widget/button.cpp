
#include "button.h"

#include "../../../graphics/spriteBatcher.h"

namespace widget {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Button::Button(int id, int x, int y, int width, int height) :
	AbstractWidget(id, x, y, width, height)
{
}

Button::~Button()
{

}

// ########################################################
// Member Functions########################################
// ########################################################

void Button::draw()
{
	//graphics::SpriteBatcher::getInstance().addBatch();
}

void Button::mouseClicked(int button, float x, float y)
{
	if(isInsideBorders(x,y)){

		// Do shit
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
