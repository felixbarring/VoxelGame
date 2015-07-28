
#ifndef SRC_UTIL_FPSMANAGER_H_
#define SRC_UTIL_FPSMANAGER_H_

#include <chrono>
#include <thread>

namespace util
{

class FPSManager {
public:
// ########################################################
// Constructor/Destructor #################################
// ########################################################
	FPSManager(int maxFPS);
	virtual ~FPSManager();

// ########################################################
// Member Functions########################################
// ########################################################
	void frameStart();

	void sync();

// ########################################################
// Instance Variables #####################################
// ########################################################
private:
	int maxFPS;
	double frameStartTime = 0;
	double timePerFrame;
	const double mili = 0.001;
};

}
#endif /* SRC_UTIL_FPSMANAGER_H_ */
