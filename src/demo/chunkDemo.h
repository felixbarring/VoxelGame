
#ifndef SRC_DEMO_CHUNKDEMO_H_
#define SRC_DEMO_CHUNKDEMO_H_

#include "iDemo.h"

namespace demo
{

class ChunkDemo : public IDemo {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	ChunkDemo() {};

	virtual ~ChunkDemo() {};

// ########################################################
// Member Functions########################################
// ########################################################

	void runDemo() override;

// ########################################################
// Instance Variables #####################################
// ########################################################

};

}

#endif /* SRC_DEMO_CHUNKDEMO_H_ */
