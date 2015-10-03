
#ifndef SRC_DEMO_FONTDEMO_H_
#define SRC_DEMO_FONTDEMO_H_

#include "iDemo.h"

namespace demo {

class FontDemo : public IDemo {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	FontDemo() {};

	virtual ~FontDemo() {};

// ########################################################
// Member Functions########################################
// ########################################################

	void runDemo() override;

// ########################################################
// Instance Variables #####################################
// ########################################################

};

} /* namespace demo */

#endif /* SRC_DEMO_FONTDEMO_H_ */
