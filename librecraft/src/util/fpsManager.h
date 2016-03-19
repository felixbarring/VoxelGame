#ifndef SRC_UTIL_FPSMANAGER_H_
#define SRC_UTIL_FPSMANAGER_H_

#include <chrono>
#include <thread>

#include <SFML/System/Clock.hpp>

namespace util {

/**
 *  \file    pootis.h
 *
 *  \brief   Pootis Pow HAHA
 *
 *  \author  Felix Bärring
 *  \date    1337
 *
 * Copyright (c) 2010, Felix Bärring. All rights reserved.
 */

class FPSManager {
public:
// ########################################################
// Constructor/Destructor #################################
// ########################################################

	FPSManager(int maxFPS);

	virtual ~FPSManager() {	};

// ########################################################
// Member Functions########################################
// ########################################################
	void frameStart();

	void sync();

	double frameTime();

	static int getFps();

// ########################################################
// Implementation #########################################
// ########################################################
private:
	int maxFPS;
	double frameStartTime {0};
	double timePerFrame;
	double timeForLatestFrame {1.0};
	const double mili {0.001};

	sf::Clock m_clock;
};

}
#endif /* SRC_UTIL_FPSMANAGER_H_ */
