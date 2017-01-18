#ifndef SRC_DEMO_TOGGLEBUTTON_H_
#define SRC_DEMO_TOGGLEBUTTON_H_

#include "button.h"

namespace widget {

class ToggleButton: public Button {
public:

    /**
     * @brief Uaed to determine the visuals of the button.
     */
    enum class Skin {
        Regular,    //!< Regular visuals representing a button with text drawn on it that can be check
        CheckBox,   //!< CheckBox visuals representing a small box that can be checked and with text to the right
        ReadioButton//!< ReadioButton visuals representing a circle that can be check and with text to the right
    };

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	ToggleButton(int id, int x, int y, int width, int height, std::function<void(int)> observer,
			const std::string &name, int layer = 0, Skin skin = Skin::Regular);

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
	bool m_useHighlight{true}; // Do not use highlights for CheckBox and RadioButton skin since they look bad.

	std::shared_ptr<graphics::Sprite> m_spriteToggled;
};

} /* namespace widget */

#endif /* SRC_DEMO_TOGGLEBUTTON_H_ */
