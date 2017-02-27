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
class Slider: public AbstractWidget {
public:

	Slider(int id, int x, int y, int width, int height, std::function<void(int)> observer, int layer = 0);

	virtual ~Slider() {};

	float getValue();

	void draw() override;

	void update(float timePassed) override;

private:

	std::function<void(int)> m_observer;

	bool m_pointerInsideBorders {false};

	std::shared_ptr<graphics::Sprite> m_slider;
	std::shared_ptr<graphics::Sprite> m_knob;

	float m_knobPosition;
	float m_knobWidth;

	bool m_grabbed {false};

};

} /* namespace widget */

#endif /* SRC_GUI_WIDGET_SLIDER_H_ */
