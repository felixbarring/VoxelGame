
#include "timeCycle.h"

// ########################################################
// Member Functions########################################
// ########################################################

void TimeCycle::update(float timePassed) {
    m_time = (m_time + timePassed) % s_dayLength;
}

void TimeCycle::setTime(float time) {

}
