
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

	Slider(int id, int x, int y, int width, int height, std::function<void(int, float)> observer);

	virtual ~Slider() {};

// ########################################################
// Member Functions########################################
// ########################################################

	void draw() override;

	void mouseClicked(int button, float x, float y) override;

	void mouseMoved(float x, float y) override;

	void keyPressed(int key, char c) override;

// ########################################################
// Instance Variables #####################################
// ########################################################

private:

	std::function<void(int, float)> observer;

	bool pointerInsideBorders = false;

	std::shared_ptr<graphics::Sprite> slider;
	std::shared_ptr<graphics::Sprite> knob;

	float knobPosition;
	float knobWidth;

	bool grabbed = false;

};

} /* namespace widget */

#endif /* SRC_GUI_WIDGET_SLIDER_H_ */
