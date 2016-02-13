#ifndef SRC_GUI_WIDGET_TEXTAREA_H_
#define SRC_GUI_WIDGET_TEXTAREA_H_

#include "abstractWidget.h"

namespace widget {

class TextArea: public AbstractWidget {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	TextArea(int id, int x, int y, int width, int height);

	virtual ~TextArea() {
	}
	;

// ########################################################
// Member Functions########################################
// ########################################################

	void draw() override;

	void update(float timePassed) override;

// ########################################################
// Implementation #########################################
// ########################################################

};

} /* namespace widget */

#endif /* SRC_GUI_WIDGET_TEXTAREA_H_ */
