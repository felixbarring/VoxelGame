
#ifndef SRC_DEMO_CUBEMAPDEMO_H_
#define SRC_DEMO_CUBEMAPDEMO_H_

#include "iDemo.h"

namespace demo
{

class CubeMapDemo : public IDemo{
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	CubeMapDemo() {};

	virtual ~CubeMapDemo() {};

// ########################################################
// Member Functions########################################
// ########################################################

	void runDemo() override;

// ########################################################
// Instance Variables #####################################
// ########################################################

};

}

#endif /* SRC_DEMO_CUBEMAPDEMO_H_ */
