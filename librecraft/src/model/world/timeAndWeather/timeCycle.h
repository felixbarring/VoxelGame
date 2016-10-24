
#ifndef SRC_MODEL_WORLD_TIMEANDWEATHER_TIMECYCLE_H_
#define SRC_MODEL_WORLD_TIMEANDWEATHER_TIMECYCLE_H_

class TimeCycle {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

    virtual ~TimeCycle() {};

// ########################################################
// Member Functions########################################
// ########################################################

    void update(float timePassed);

    void setTime(float time);

// ########################################################
// Implementation #########################################
// ########################################################

private:
    float s_dayLength = 30;
    float m_time;
};

#endif /* SRC_MODEL_WORLD_TIMEANDWEATHER_TIMECYCLE_H_ */
