
#ifndef SRC_DEMO_GUIDEMO_H_
#define SRC_DEMO_GUIDEMO_H_

#include "iDemo.h"

namespace demo {

class GuiDemo : public IDemo{
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	GuiDemo();

	virtual ~GuiDemo();

// ########################################################
// Member Functions########################################
// ########################################################

	void runDemo() override;

// ########################################################
// Instance Variables #####################################
// ########################################################


};

} /* namespace demo */

#endif /* SRC_DEMO_GUIDEMO_H_ */
