
#include "timeCycle.h"

#include <iostream>

#include "../../../graphics/chunkBatcher.h"

#include "../../../graphics/chunkBatcher.h"
// ########################################################
// Member Functions########################################
// ########################################################

void TimeCycle::update(float timePassed) {
    m_time += timePassed;

    if (m_time > s_dayLength)
        m_time = 0;

    std::cout << m_time << "\n";

    graphics::ChunkBatcher::getInstance().setSunStrenght(m_time / (2 * s_dayLength));
}

void TimeCycle::setTime(float time) {
    m_time = time;
}
