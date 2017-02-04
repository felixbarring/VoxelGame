#include "slider.h"

#include "../../graphics/resources.h"

#include "../../graphics/spriteBatcher.h"
#include "../../graphics/fontMeshBuilder.h"
#include "../../graphics/mesh/meshElement.h"
#include "../../config/data.h"
#include "../../graphics/graphicsManager.h"
#include "../../graphics/resources.h"
#include "../../util/input.h"

using namespace std;
using namespace graphics;
using namespace util;

namespace widget {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Slider::Slider(int id, int x, int y, int width, int height,	std::function<void(int)> observer, int layer)
    : AbstractWidget(id, x, y, width, height)
{

	this->m_observer = observer;
	m_knobPosition = x;
	m_knobWidth = height;

	m_slider.reset(new Sprite(x, y, layer, width, height, Resources::getInstance().getTexture(
							config::gui_data::slider)));
	m_knob.reset(new Sprite(x, y, layer + 1, height, height, Resources::getInstance().getTexture(
							config::gui_data::sliderKnob)));

}

// ########################################################
// Member Functions########################################
// ########################################################

float Slider::getValue() {
	return (m_knobPosition - this->m_xCoordinate) / (m_width - m_knobWidth);
}

void Slider::draw() {
    GraphicsManager::getInstance().getSpriteBatcher().addBatch(m_slider);
    GraphicsManager::getInstance().getSpriteBatcher().addBatch(m_knob);
}

void Slider::update(float) {
	shared_ptr<Input> input = Input::getInstance();
	m_pointerInsideBorders = isInsideBorders(input->mouseVirtualAdjustedX,
			input->mouseVirtualAdjustedY);

	m_grabbed = input->action1Active && m_pointerInsideBorders;

	if (m_grabbed) {
		m_knobPosition = input->mouseVirtualAdjustedX - m_knobWidth / 2;

		if (m_knobPosition < this->m_xCoordinate)
			m_knobPosition = this->m_xCoordinate;

		if (m_knobPosition > this->m_xCoordinate + this->m_width - m_knobWidth)
			m_knobPosition = this->m_xCoordinate + this->m_width - m_knobWidth;

		m_knob->setLocation(m_knobPosition, this->m_yCoordinate);

		m_observer.operator ()(m_id);
	}

}

} /* namespace widget */
