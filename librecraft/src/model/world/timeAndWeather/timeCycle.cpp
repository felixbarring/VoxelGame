
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
    float maxLightValue{1.0 / 16.0};
    float offset{maxLightValue / 5.0};


    auto sinVal = glm::sin((2  * 3.14 / s_dayLength) * m_time);
    m_sunStrength = std::max(sinVal * maxLightValue, 1.0 / (16.0 * 15));

//    graphics::ChunkBatcher::getInstance().setSunStrenght(0.0f);
}

void TimeCycle::setTime(float time) {
    m_time = time;
}

float TimeCycle::getSunStrenght() {
    return m_sunStrength;
}
