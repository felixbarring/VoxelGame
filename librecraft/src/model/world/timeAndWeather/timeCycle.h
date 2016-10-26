
#ifndef SRC_MODEL_WORLD_TIMEANDWEATHER_TIMECYCLE_H_
#define SRC_MODEL_WORLD_TIMEANDWEATHER_TIMECYCLE_H_

class TimeCycle {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

// ########################################################
// Member Functions########################################
// ########################################################

    void update(float timePassed);

    void setTime(float time);

    void stopCycle();

    void resumeCycle();

    /**
     *Returns the sunlight strength that was computed in the update function.
     *
     * @return The sun strength in the range of (0, 1) where 0 means no light and 1 means max sun light.
     */
    float getSunStrenght();

// ########################################################
// Implementation #########################################
// ########################################################

private:

    bool m_paused{false};
    float s_dayLength{300};
    float m_time;

    float m_sunStrength{};
};

#endif /* SRC_MODEL_WORLD_TIMEANDWEATHER_TIMECYCLE_H_ */
