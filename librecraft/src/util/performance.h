
#ifndef SRC_UTIL_PERFORMANCE_H_
#define SRC_UTIL_PERFORMANCE_H_

#include <SFML/System/Clock.hpp>
#include <map>
#include <vector>

namespace util {

class Performance {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	Performance();

// ########################################################
// Member Functions########################################
// ########################################################

public:

    Performance getInstance() {
        static Performance INSTANCE;
        return INSTANCE;
    }

    unsigned createId(const std::string& value);

    void startTimer(unsigned id);

    void stopTimer(unsigned id);

    void clearData();

    void loggData();

// ########################################################
// Implementation #########################################
// ########################################################

private:

    std::map<std::string, int> m_map{};
    std::vector<std::tuple<sf::Clock, double>> m_accumulatedTime{};
    sf::Clock m_clock{};

};

} /* namespace util */

#endif /* SRC_UTIL_PERFORMANCE_H_ */
