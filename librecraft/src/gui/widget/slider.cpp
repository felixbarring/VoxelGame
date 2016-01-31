
#include "slider.h"

#include "../../graphics/resources.h"

#include "../../graphics/spriteBatcher.h"
#include "../../graphics/fontMeshBuilder.h"
#include "../../graphics/mesh/meshElement.h"
#include "../../config/data.h"
#include "../../graphics/resources.h"

namespace widget {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Slider::Slider(int id, int x, int y, int width, int height, std::function<void(int)> observer) :
	AbstractWidget(id, x, y, width, height)
{

	this->observer = observer;
	knobPosition = x;
	knobWidth = height;

	slider.reset(new graphics::Sprite{x, y, 1, width, height,
		graphics::Resources::getInstance().getTexture(config::gui_data::slider)});
	knob.reset(new graphics::Sprite{x, y, 2, height, height,
		graphics::Resources::getInstance().getTexture(config::gui_data::sliderKnob)});

}

// ########################################################
// Member Functions########################################
// ########################################################

void Slider::draw()
{
	graphics::SpriteBatcher::getInstance().addBatch(slider);
	graphics::SpriteBatcher::getInstance().addBatch(knob);
}
//

void Slider::mouseClicked(int button, float x, float y)
{
	grabbed = !grabbed && pointerInsideBorders;
}

void Slider::mouseMoved(float x, float y)
{
	pointerInsideBorders = isInsideBorders(x, y);
	grabbed = grabbed && pointerInsideBorders;

	if (grabbed) {
		knobPosition = x - knobWidth / 2;

		if (knobPosition < this->x ) {
			knobPosition = this->x;
		}

		if (knobPosition > this->x + this->width - knobWidth) {
			knobPosition = this->x + this->width - knobWidth;
		}

		knob->setLocation(knobPosition + knobWidth / 2, this->y + knobWidth / 2, 0);

		observer.operator ()(id);
	}

}

void Slider::keyTyped(char value)
{
	// not relevant for button, do nothing
}

float Slider::getValue()
{
	return (knobPosition - this->x) / (width - knobWidth);
}

} /* namespace widget */
