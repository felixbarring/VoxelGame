
#ifndef SRC_DEMO_LIBNOISEDEMO_H_
#define SRC_DEMO_LIBNOISEDEMO_H_

#include "iDemo.h"

#include <iostream>

#include "../model/world/terrainGen/libnoise/module/perlin.h"

using namespace noise;
using namespace std;

namespace demo {

class LibNoiseDemo : public IDemo
{
public:

  void runDemo() override {
    module::Perlin myModule;

    for (double i{0.0}; i < 5.0; ++i) {
      for (double j{0.0}; j < 5.0; ++j) {
        double value{myModule.GetValue(0.1 * i, 0.1 * j, 0.1 * 5)};
        cout << value << " ";
      }
      cout << "\n";
    }

    cout << "\n\n\n";
    myModule.SetSeed(myModule.GetSeed() + 5);

    // Should result in different values.
    for (double i{0.0}; i < 5.0; ++i) {
      for (double j{0.0}; j < 5.0; ++j) {
        double value{myModule.GetValue(0.1 * i, 0.1 * j, 0.1 * 5)};
        cout << value << " ";
      }
      cout << "\n";
    }
  }

};

}

#endif /* SRC_DEMO_LIBNOISEDEMO_H_ */
