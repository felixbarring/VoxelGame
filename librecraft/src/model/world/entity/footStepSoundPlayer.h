
#ifndef SRC_MODEL_WORLD_ENTITY_FOOTSTEPSOUNDPLAYER_H_
#define SRC_MODEL_WORLD_ENTITY_FOOTSTEPSOUNDPLAYER_H_

#include <string>
#include <vector>

/**
 * @brief A class that is used to play foot step sounds.
 *
 * The purpose of this class is to play sound effects that represents foot
 * step in various speeds.
 */
class FootStepSoundPlayer {
public:

  /**
   * @brief Constructs an instance of FootStepSoundPlayer
   *
   * @param stepSounds The instance will use the provided vector of strings to
   *                   play the foot steps. The strings should correspond to a
   *                   sound file.
   */
  FootStepSoundPlayer(std::vector<std::string> stepSounds);

  /**
   * @brief Plays stepping sounds at a regular interval.
   *
   * The player will randomly select a sound from the sounds provided in the
   * constructor at a regular interval. There is a delay between the foot
   * steps that can be tweaked with setWalkingIntervall.
   *
   * @param time The time since the last time this function was called.
   *             Used to check if enough time has passed for the next step
   *             sound to play.
   */
  void walkingActive(double time);

  /**
   * @brief Changes the delay between each time the foot step sound will be
   *        played.
   *
   * @param value The new delay.
   */
  void setWalkingIntervall(double value);


private:

  std::vector<std::string> m_stepSounds{};

  bool m_isWalking{false};
  double m_accumulatedTime{};
  double m_footStepDelay{0.5};
};

#endif /* SRC_MODEL_WORLD_ENTITY_FOOTSTEPSOUNDPLAYER_H_ */
