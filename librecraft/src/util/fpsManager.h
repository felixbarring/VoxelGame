#ifndef SRC_UTIL_FPSMANAGER_H_
#define SRC_UTIL_FPSMANAGER_H_

#include <chrono>
#include <ctime>
#include <thread>

namespace util {

/**
 *  @brief This class provides functionality to limit and monitor the frames
 *         per second (FPS).
 */
class FPSManager {
public:
  /**
   * @brief Constructs an instance of the FPSManager class.
   *
   * @param maxFPS The fps limit.
   */
  explicit FPSManager(double maxFPS);

  /**
   * @brief Call this function at the start of each frame.
   *
   * Needed so that this class can check at what time the frame started and use
   * it to calculate the fps and how determine the amount of time the sync
   * function should but the thread to sleep.
   */
  void frameStart();

  /**
   * @brief Syncs the thread if the frame completed faster than the fps
   *        dictates.
   *
   * If the frame was faster than the fps limit set in the constructor then
   * this function will make the thread sleep for the remaining time of the time
   * that the frame should take. This requires that the frameStart function was
   * called at the start of the frame.
   */
  void sync();

  /**
   * This is computed when calling the sync function. Requires that frameStart
   * has been called, followed by sync.
   *
   * @return The amount of time the frame took.
   */
  double frameTime();

  /**
   * This is computed when calling the sync function. Requires that frameStart
   * has been called, followed by sync.
   *
   * @return The fps for the last frame.
   */
  int getFps();

private:
  std::chrono::time_point<std::chrono::system_clock> m_frameStartTime;

  double m_timePerFrame{};
  const double m_mili{0.001};

  int m_currentFPS{0};
  double m_timeForLatestFrame{0};
};
}
#endif /* SRC_UTIL_FPSMANAGER_H_ */
