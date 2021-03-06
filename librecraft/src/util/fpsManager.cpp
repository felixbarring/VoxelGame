#include "fpsManager.h"

#include <iostream>
#include <thread>

namespace util {

FPSManager::FPSManager(double maxFPS)
  : m_timePerFrame{1.0 / maxFPS} {
}

void
FPSManager::frameStart() {
  m_frameStartTime = std::chrono::system_clock::now();
}

void
FPSManager::sync() {
  static std::chrono::milliseconds oneMilliSecond(1);

  // Sleep the thread if we are running fast to save computer power.
  // If we are running too fast and there is more than 1 miliseconds
  // left until next frame then the thread will sleep for the time
  // that remains. If there is less than 1 milliseconds left, we busy
  // wait until it is time to proceed with the next frame.
  // The busy waiting is used to increase the timing accuracy, we do not
  // want to start the next frame to late.

  std::chrono::duration<double> tft =
    std::chrono::system_clock::now() - m_frameStartTime;
  double totalFrameTime = tft.count();

  while (totalFrameTime < m_timePerFrame) {
    double timeRemainingInMilis = (m_timePerFrame - totalFrameTime) / m_mili;
    if (timeRemainingInMilis > 2) {
      std::this_thread::sleep_for((timeRemainingInMilis - 1) * oneMilliSecond);
    } else if (timeRemainingInMilis > 1) {
      std::this_thread::sleep_for(oneMilliSecond);
    }
    tft = std::chrono::system_clock::now() - m_frameStartTime;
    totalFrameTime = tft.count();
  }

  m_frameStartTime = std::chrono::system_clock::now();
  m_currentFPS = 1.0 / totalFrameTime;
  m_timeForLatestFrame = totalFrameTime;
}

double
FPSManager::frameTime() {
  return m_timeForLatestFrame;
}

int
FPSManager::getFps() {
  return m_currentFPS;
}
}
