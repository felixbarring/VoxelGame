
#include "globalResources.h"

#include <thread>

namespace util
{
namespace globalResources
{
	ThreadPool g_threadPool{1};
	ThreadPool g_threadPool2{std::thread::hardware_concurrency()};
}
}



