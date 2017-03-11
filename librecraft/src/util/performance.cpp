#include "performance.h"

#include <algorithm>
#include <iosfwd>
using namespace std;

namespace util {

static unsigned s_idCounter = 0;

unsigned Performance::createId(const string& value) {

  // Check if the value in the map equals the string argument value.
//    auto res = m_map.find(value);
//    if (res != m_map.end())
//        return (*res).second;

  m_map.emplace(++s_idCounter, value);
  return s_idCounter;
}

void Performance::startTimer(unsigned id) {
  get<0>(m_accumulatedTime[id]).restart();
}

void Performance::stopTimer(unsigned id) {
  auto &p = m_accumulatedTime[id];
  get<1>(p) += get<0>(p).getElapsedTime().asSeconds();
}

void Performance::clearData() {
  for_each(m_accumulatedTime.begin(), m_accumulatedTime.end(),
      [](tuple<sf::Clock, double> &t) {get<1>(t) = 0;});
}

void Performance::loggData() {
  // TODO Implement :-)
}

void Performance::printData() {
  for_each(m_accumulatedTime.begin(), m_accumulatedTime.end(),
      [] (tuple<sf::Clock, double>&)
      {
//          std::cout << m_map.find(get<1>(t)) << "Blal " << get<0>(t).getElapsedTime() << "bla bla \n";
      });
}

} /* namespace util */
