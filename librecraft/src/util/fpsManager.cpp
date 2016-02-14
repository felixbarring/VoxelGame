#include "fpsManager.h"

#include <iostream>

#include <glfw3.h>
#include <chrono>
#include <thread>

namespace util {

std::chrono::milliseconds oneMilliSecond(1);

// ########################################################
// Constructor/Destructor #################################
// ########################################################

FPSManager::FPSManager(int maxFPS)
		: maxFPS {maxFPS}, timePerFrame {1.0 / maxFPS} {
}

// ########################################################
// Member Functions########################################
// ########################################################

void FPSManager::frameStart() {
	frameStartTime = glfwGetTime();
}

void FPSManager::sync() {

	// Sleep the thread if we are running fast to save computer power.
	// If we are running too fast and there is more than 1 miliseconds left until
	//  next frame then the thread will sleep for the time that remains.
	// If there is less than 1 milliseconds left, we busy wait until it is time to proceed with the next frame.
	// The busy waiting is used to increase the timing accuracy, we do not want to start the next frame to late.

	double totalFrameTime = glfwGetTime() - frameStartTime;
	while (totalFrameTime < timePerFrame) {
		double timeRemainingInMilis = (timePerFrame - totalFrameTime) / mili;
		if (timeRemainingInMilis > 2) {
			std::this_thread::sleep_for(
					(timeRemainingInMilis - 1) * oneMilliSecond);
		}
		else if (timeRemainingInMilis > 1) {
			std::this_thread::sleep_for(oneMilliSecond);
		}
		totalFrameTime = glfwGetTime() - frameStartTime;
	}

	currentFPS = 1.0 / totalFrameTime;
	timeForLatestFrame = totalFrameTime;

//	std::cout << currentFPS << "\n";

}

double FPSManager::frameTime() {
	return timeForLatestFrame;
}

int FPSManager::getFps() {
	return currentFPS;
}

}
