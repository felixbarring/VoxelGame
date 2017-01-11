#ifndef SRC_DEMO_TOGGLEBUTTON_H_
#define SRC_DEMO_TOGGLEBUTTON_H_

#include "button.h"

namespace widget {

class ToggleButton: public Button {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	ToggleButton(int id, int x, int y, int width, int height, std::function<void(int)> observer,
			const std::string &name, int layer = 0);

	virtual ~ToggleButton() {};

// ########################################################
// Member Functions########################################
// ########################################################

	bool isToggled();

	void toggle();

	void draw() override;

	void update(float timePassed) override;

// ########################################################
// Implementation #########################################
// ########################################################

private:

	bool m_toggled{false};

	std::shared_ptr<graphics::Sprite> m_spriteToggled;
};

} /* namespace widget */

#endif /* SRC_DEMO_TOGGLEBUTTON_H_ */
