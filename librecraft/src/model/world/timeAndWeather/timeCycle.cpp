
#include "timeCycle.h"

#include <iostream>

#include "detail/func_trigonometric.hpp"

// ########################################################
// Member Functions########################################
// ########################################################

void TimeCycle::update(float timePassed) {
    if (m_paused)
        return;

    m_time += timePassed;

    if (m_time > m_dayLength)
        m_time = 0;

    static constexpr float pi{3.14};
    float magicNumber = 0.2; // bigger value makes the day longer and night shorter.
    float sinVal = magicNumber + glm::sin((2  * pi / m_dayLength) * m_time);
    if (sinVal > 1) sinVal = 1;
    m_sunStrength = std::max(sinVal, 1.0f / (15.0f));
}

void TimeCycle::setTime(float time) {
    m_time = time;
}

void TimeCycle::stopCycle() {
    m_paused = true;
}

void TimeCycle::resumeCycle() {
    m_paused = false;

}

void TimeCycle::setDayLenght(float value) {
    m_dayLength = value;
}

float TimeCycle::getSunStrenght() {
    return m_sunStrength;
}
