
#ifndef SRC_DEMO_LIBNOISEDEMO_H_
#define SRC_DEMO_LIBNOISEDEMO_H_

#include <iostream>

#include "dependencies/libnoise/module/perlin.h"

namespace demo {

class LibNoiseDemo {
public:
  void runDemo() {

    using namespace noise;
    using namespace std;

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
