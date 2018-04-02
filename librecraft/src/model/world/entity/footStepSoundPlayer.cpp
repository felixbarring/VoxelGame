
#include "footStepSoundPlayer.h"

#include <random>

#include "../../../util/soundPlayer.h"

using std::mt19937;
using std::random_device;
using std::uniform_int_distribution;

FootStepSoundPlayer::FootStepSoundPlayer(util::SoundPlayer& player,
                                         std::vector<std::string> stepSounds,
                                         double footStepDelay)
  : m_stepSounds{move(stepSounds)}
  , m_soundPlayer{player}
  , m_footStepDelay{footStepDelay} {
}

void
FootStepSoundPlayer::walkingActive(double time) {
  if ((m_accumulatedTime += time) <= m_footStepDelay)
    return;

  m_accumulatedTime -= m_footStepDelay;
  playSteppingSound();
}

void
FootStepSoundPlayer::playSteppingSound() {
  static int last{-1};
  static random_device randomDevice;
  static mt19937 randomNumber(randomDevice());
  static uniform_int_distribution<int> uni(0, m_stepSounds.size() - 1);

  int value{};
  do {
    value = uni(randomNumber);
  } while (value == last);
  last = value;

  m_soundPlayer.playSound(m_stepSounds[value]);
}

void
FootStepSoundPlayer::setWalkingIntervall(double value) {
  m_footStepDelay = value;
}
