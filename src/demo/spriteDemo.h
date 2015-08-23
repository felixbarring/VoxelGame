
#ifndef SRC_DEMO_SPRITEDEMO_H_
#define SRC_DEMO_SPRITEDEMO_H_

#include "iDemo.h"

namespace demo {

class SpriteDemo : public virtual IDemo  {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	SpriteDemo();
	virtual ~SpriteDemo();

// ########################################################
// Member Functions########################################
// ########################################################

	void runDemo() override;

// ########################################################
// Instance Variables #####################################
// ########################################################

};

} /* namespace demo */

#endif /* SRC_DEMO_SPRITEDEMO_H_ */
