
#include "timeCycle.h"

#include <iostream>

#include "../../../graphics/chunkBatcher.h"
#include "../../../graphics/chunkBatcher.h"

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
    float offset{maxLightValue / 5.0f};

    graphics::ChunkBatcher::getInstance().setSunStrenght(offset + glm::sin((2  * 3.14 / s_dayLength) * m_time) * maxLightValue);
}

void TimeCycle::setTime(float time) {
    m_time = time;
}
