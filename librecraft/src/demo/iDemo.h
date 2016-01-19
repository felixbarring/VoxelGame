
#ifndef SRC_DEMO_IDEMO_H_
#define SRC_DEMO_IDEMO_H_

namespace demo
{

class IDemo {
public:

	virtual ~IDemo() {}

	virtual void runDemo() = 0;

};

}

#endif /* SRC_DEMO_IDEMO_H_ */
