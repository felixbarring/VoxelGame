
#ifndef SRC_DEMO_CUBEDEMO_H_
#define SRC_DEMO_CUBEDEMO_H_

#include "iDemo.h"

namespace demo
{

class CubeDemo : public virtual IDemo {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	CubeDemo();
	virtual ~CubeDemo();

// ########################################################
// Member Functions########################################
// ########################################################

	void runDemo() override;

// ########################################################
// Instance Variables #####################################
// ########################################################

};

}

#endif /* SRC_DEMO_CUBEDEMO_H_ */
