
#ifndef SRC_GUI_WIDGET_CHECKBUTTON_H_
#define SRC_GUI_WIDGET_CHECKBUTTON_H_

#include "abstractWidget.h"

namespace widget {

class CheckButton: public AbstractWidget {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

    CheckButton();

    virtual ~CheckButton();

// ########################################################
// Member Functions########################################
// ########################################################

    void draw() override;

    void update(float timePassed) override;

// ########################################################
// Implementation #########################################
// ########################################################

private:

};

} /* namespace widget */

#endif /* SRC_GUI_WIDGET_CHECKBUTTON_H_ */
