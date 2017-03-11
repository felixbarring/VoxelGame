#ifndef SRC_UTIL_FPSMANAGER_H_
#define SRC_UTIL_FPSMANAGER_H_

#include <chrono>
#include <thread>

#include <SFML/System/Clock.hpp>

namespace util {

class FPSManager {
public:

  FPSManager(int maxFPS);

  void frameStart();

  void sync();

  static double frameTime();

  static int getFps();

private:
  int m_maxFPS{};
  double m_frameStartTime{0};
  double m_timePerFrame{};
  const double m_mili{0.001};

  double m_timer{0};
  double m_totalTimed{0};

  sf::Clock m_clock{};
};

}
#endif /* SRC_UTIL_FPSMANAGER_H_ */
