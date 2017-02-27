
#ifndef SRC_UTIL_PERFORMANCE_H_
#define SRC_UTIL_PERFORMANCE_H_

#include <SFML/System/Clock.hpp>
#include <map>
#include <vector>

namespace util {

class Performance {

    Performance();

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

    void printData();

private:

    std::map<int, std::string> m_map{};
    std::vector<std::tuple<sf::Clock, double>> m_accumulatedTime{};
    sf::Clock m_clock{};

};

} /* namespace util */

#endif /* SRC_UTIL_PERFORMANCE_H_ */
