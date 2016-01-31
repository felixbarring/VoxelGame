
#include "slider.h"

#include "../../graphics/resources.h"

#include "../../graphics/spriteBatcher.h"
#include "../../graphics/fontMeshBuilder.h"
#include "../../graphics/mesh/meshElement.h"
#include "../../config/data.h"
#include "../../graphics/resources.h"

using namespace graphics;

namespace widget {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Slider::Slider(int id, int x, int y, int width, int height,
		std::function<void(int)> observer) :
	AbstractWidget(id, x, y, width, height)
{

	this->observer = observer;
	knobPosition = x;
	knobWidth = height;

	slider.reset(new Sprite{x, y, 1, width, height,
		Resources::getInstance().getTexture(config::gui_data::slider)});
	knob.reset(new Sprite{x, y, 2, height, height,
		Resources::getInstance().getTexture(config::gui_data::sliderKnob)});

}

// ########################################################
// Member Functions########################################
// ########################################################

void Slider::draw()
{
	SpriteBatcher::getInstance().addBatch(slider);
	SpriteBatcher::getInstance().addBatch(knob);
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

		knob->setLocation(knobPosition + knobWidth / 2,
				this->y + knobWidth / 2, 0);

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
