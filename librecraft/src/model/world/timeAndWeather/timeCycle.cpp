
#include "timeCycle.h"

#include <iostream>

#include "detail/func_trigonometric.hpp"

// ########################################################
// Member Functions########################################
// ########################################################

void TimeCycle::update(float timePassed) {
    m_time += timePassed;

    if (m_time > s_dayLength)
        m_time = 0;

    std::cout << m_time << "\n";
    float pi{3.14};

    float sinVal = glm::sin((2  * pi / s_dayLength) * m_time);
    m_sunStrength = std::max(sinVal, 1.0f / (15.0f));
}

void TimeCycle::setTime(float time) {
    m_time = time;
}

void TimeCycle::stopCycle() {

}

void TimeCycle::resumeCycle() {

}

float TimeCycle::getSunStrenght() {
    return m_sunStrength;
}
