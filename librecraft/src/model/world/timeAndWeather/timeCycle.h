#ifndef SRC_MODEL_WORLD_TIMEANDWEATHER_TIMECYCLE_H_
#define SRC_MODEL_WORLD_TIMEANDWEATHER_TIMECYCLE_H_

#include <utility>

class TimeCycle {
public:

  /**
   * @brief Updates the TimeCycle state.
   *
   * @param timePassed The time change since the last call to this function.
   */
  void update(float timePassed);

  /**
   * @brief Sets the time of the day.
   *
   * @param time The new time value.
   */
  void setTime(float time);

  /**
   * @brief Returns the time value.
   *
   * @return The time value.
   */
  float getTime();

  /**
   * @brief Stops the state changes when update is called.
   */
  void stopCycle();

  /**
   * @brief Reverts the stopCycle function.
   */
  void resumeCycle();

  /**
   * @brief Returns the sunlight strength that was computed in the update
   *        function.
   *
   * @return The sun strength in the range of (0, 1) where 0 means no light and
   *         1 means max sun light.
   */
  double getSunStrenght();

  /**
   * @brief Returns the starlight strength that was computed in the update
   *        function
   *
   * @return The star strength in the range of (0, 1) where 0 means no light and
   *         1 means max star light.
   */
  double getStarStrenght();

  /**
   * @brief Sets the speed in which the time increases.
   *
   * By default the value is 1. Setting the value to 2 will mean that the time
   * progress twice as fast as the default.
   *
   * @param value The new time speed value.
   */
  void setTimeSpeed(double value);

private:

  std::pair<double, double> m_dawn{0.0, 100.0};
  std::pair<double, double> m_day{100.0, 300.0};
  std::pair<double, double> m_dusk{300.0, 400.0};
  std::pair<double, double> m_night{400.0, 600.0};

  std::pair<double, double> m_starIncrease{360, 400};
  std::pair<double, double> m_starDecrease{580, 600};

  double m_timeSpeed{1.0};
  const double m_dayLength{600};
  const double m_lowestSunStrength{0.1};

  bool m_paused{false};

  double m_time{0.0};
  double m_sunStrength{0.0};
  double m_starStrength{0.0};
};

#endif /* SRC_MODEL_WORLD_TIMEANDWEATHER_TIMECYCLE_H_ */
