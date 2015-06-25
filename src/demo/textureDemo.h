
#ifndef SRC_DEMO_TEXTUREDEMO_H_
#define SRC_DEMO_TEXTUREDEMO_H_

#include "iDemo.h"

class TextureDemo : public virtual IDemo {
public:
// ########################################################
// Constructor/Destructor #################################
// ########################################################
	TextureDemo();
	virtual ~TextureDemo();

// ########################################################
// Member Functions########################################
// ########################################################
	void runDemo() override;

// ########################################################
// Instance Variables #####################################
// ########################################################

};

#endif /* SRC_DEMO_TEXTUREDEMO_H_ */
