
#ifndef SRC_DEMO_PLAYERCOLLISIONDEMO_H_
#define SRC_DEMO_PLAYERCOLLISIONDEMO_H_

#include "iDemo.h"

namespace demo {

class PlayerCollisionDemo : IDemo {
public:

	// ########################################################
	// Constructor/Destructor #################################
	// ########################################################

	PlayerCollisionDemo();
	virtual ~PlayerCollisionDemo();

	// ########################################################
	// Member Functions########################################
	// ########################################################

	void runDemo() override;

	// ########################################################
	// Instance Variables #####################################
	// ########################################################

};

} /* namespace demo */

#endif /* SRC_DEMO_PLAYERCOLLISIONDEMO_H_ */
