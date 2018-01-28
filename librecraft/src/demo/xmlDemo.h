
#ifndef SOURCE_DIRECTORY__SRC_DEMO_XMLDEMO_H_
#define SOURCE_DIRECTORY__SRC_DEMO_XMLDEMO_H_

#include "iDemo.h"
#include "../dependencies/tinyXml2/tinyxml2.h"


namespace demo {

class XmlDemo : public IDemo {
public:

  void runDemo() override {
    tinyxml2::XMLDocument doc;
  }

};

}

#endif /* SOURCE_DIRECTORY__SRC_DEMO_XMLDEMO_H_ */
