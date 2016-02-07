
/**
 *  \file [slider.h]
 *  \brief A slider can be used to input values in a gui
 *  \author Felix Bärring
 *
 *  Copyright (c) 2015, Felix Bärring. All rights reserved.
 */

#ifndef SRC_GUI_WIDGET_SLIDER_H_
#define SRC_GUI_WIDGET_SLIDER_H_

#include <memory>

#include "abstractWidget.h"
#include "../../graphics/sprite.h"


namespace widget {

/**
 * \class Slider
 *
 * \author Felix Bärring
 */
class Slider : public AbstractWidget {
public:


// ########################################################
// Constructor/Destructor #################################
// ########################################################

	Slider(int id, int x, int y, int width, int height,
			std::function<void(int)> observer, int layer = 0);

	virtual ~Slider() {};

// ########################################################
// Member Functions########################################
// ########################################################

	float getValue();

	void draw() override;

	void update(float timePassed) override;

// ########################################################
// Instance Variables #####################################
// ########################################################

private:

	std::function<void(int)> m_observer;

	bool m_pointerInsideBorders{false};

	std::shared_ptr<graphics::Sprite> m_slider;
	std::shared_ptr<graphics::Sprite> m_knob;

	float m_knobPosition;
	float m_knobWidth;

	bool m_grabbed{false};

};

} /* namespace widget */

#endif /* SRC_GUI_WIDGET_SLIDER_H_ */
