
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

    void stopCycle();

    void resumeCycle();

    float getSunStrenght();

// ########################################################
// Implementation #########################################
// ########################################################

private:

    float s_dayLength = 30;
    float m_time;

    float m_sunStrength{};

};

#endif /* SRC_MODEL_WORLD_TIMEANDWEATHER_TIMECYCLE_H_ */
