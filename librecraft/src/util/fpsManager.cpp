
#include "fpsManager.h"

#include <iostream>

#include <chrono>
#include <thread>

namespace util {

std::chrono::milliseconds oneMilliSecond(1);

FPSManager::FPSManager(int maxFPS)
    : m_maxFPS {maxFPS}, m_timePerFrame {1.0 / maxFPS} {
}

void FPSManager::frameStart() {
    m_clock.restart();
}

int currentFPS{0};
double timeForLatestFrame{0};

void FPSManager::sync() {

    // Sleep the thread if we are running fast to save computer power.
    // If we are running too fast and there is more than 1 miliseconds
    // left until next frame then the thread will sleep for the time
    // that remains. If there is less than 1 milliseconds left, we busy
    // wait until it is time to proceed with the next frame.
    // The busy waiting is used to increase the timing accuracy, we do not
    // want to start the next frame to late.
    double totalFrameTime = m_clock.getElapsedTime().asSeconds();
    while (totalFrameTime < m_timePerFrame) {
        double timeRemainingInMilis = (m_timePerFrame - totalFrameTime) / m_mili;
        if (timeRemainingInMilis > 2) {
            std::this_thread::sleep_for((timeRemainingInMilis - 1) * oneMilliSecond);
        }
        else if (timeRemainingInMilis > 1) {
            std::this_thread::sleep_for(oneMilliSecond);
        }
        totalFrameTime = m_clock.getElapsedTime().asSeconds() - m_frameStartTime;
    }

    currentFPS = 1.0 / totalFrameTime;
    timeForLatestFrame = totalFrameTime;

}

double FPSManager::frameTime() {
    return timeForLatestFrame;
}

int FPSManager::getFps() {
    return currentFPS;
}

}
