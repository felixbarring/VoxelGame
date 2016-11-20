
#include "timeCycle.h"

// ########################################################
// Member Functions########################################
// ########################################################

void TimeCycle::update(float timePassed) {
    if (m_paused)
        return;

    m_time += timePassed * m_timeSpeed;

    if (m_time > m_dayLength)
        m_time = 0;

    if (m_time > m_dawn.first && m_time < m_dawn.second) {
        m_sunStrength = (m_time - m_dawn.first) / (m_dawn.second - m_dawn.first);
        m_starStrength = 1 - m_sunStrength;
    } else if (m_time > m_day.first && m_time < m_day.second) {
        m_sunStrength = 1.0;
        m_starStrength = 0;
    } else if (m_time > m_dusk.first && m_time < m_dusk.second) {
        m_sunStrength = 1.0 - (m_time - m_dusk.first) / (m_dusk.second - m_dusk.first);
        m_starStrength = 1 - m_sunStrength;
    } else if (m_time > m_night.first && m_time < m_night.second) {
        m_sunStrength = 0.0;
        m_starStrength = 1.0;
    }
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

double TimeCycle::getSunStrenght() {
    return m_sunStrength;
}

double TimeCycle::getStarStrenght() {
    return m_starStrength;
}

void TimeCycle::setTimeSpeed(double value) {
    m_timeSpeed = value;
}
