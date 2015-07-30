
#ifndef SRC_DEMO_ARRAYTEXTUREDEMO_H_
#define SRC_DEMO_ARRAYTEXTUREDEMO_H_

#include "iDemo.h"

namespace demo
{

class ArrayTextureDemo : public virtual IDemo {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################
	ArrayTextureDemo();
	virtual ~ArrayTextureDemo();

// ########################################################
// Member Functions########################################
// ########################################################

	void runDemo() override;

// ########################################################
// Instance Variables #####################################
// ########################################################

};

}

#endif /* SRC_DEMO_ARRAYTEXTUREDEMO_H_ */
