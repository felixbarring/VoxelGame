#include "soundPlayer.h"

#include <ThreadPool.h>
#include <chrono>
#include <iostream>
#include <iterator>

#include "globalResources.h"

namespace util {

double volumeScaler{ 100 };

// Just a function that clamps the values to 0 or 100.
// Used to prevent OpenAl errors.
float
validVolume(double volume) {
  if (volume > 100)
    return 100;
  if (volume < 0)
    return 0;
  return volume;
}

void
SoundPlayer::update(double time) {
  for (auto sound = m_playingSounds.begin(); sound < m_playingSounds.end();
       ++sound) {
    if ((*sound)->getStatus() == sf::SoundSource::Status::Stopped)
      m_playingSounds.erase(sound);
  }

  if (m_graduayllChange) {
    if ((m_changeDirection == ChangeMusicVolume::DECREASE
           ? m_changeVolume >= m_targetVolume
           : m_changeVolume <= m_targetVolume)) {
      m_changeVolume += m_changeValue * time;
    } else {
      m_changeVolume = m_targetVolume;
      m_graduayllChange = false;
      if (m_changeDirection == ChangeMusicVolume::DECREASE)
        m_playingMusic->stop();
    }
    m_playingMusic->setVolume(validVolume(m_changeVolume));
  }
}

void
SoundPlayer::playSound(const std::string& soundPath) {
  if (m_buffers.find(soundPath) == m_buffers.end()) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(soundPath))
      std::cout << "Could not play sound :( - " << soundPath << "\n";
    m_buffers.emplace(soundPath, std::move(buffer));
  }

  auto sound = std::make_unique<sf::Sound>();
  sound->setBuffer((*m_buffers.find(soundPath)).second);
  sound->setVolume(validVolume(m_soundVolume * m_masterVolume));
  sound->play();
  m_playingSounds.push_back(std::move(sound));
}

void
SoundPlayer::playMusic(const std::string& musicPath) {
  auto music = std::make_unique<sf::Music>();
  if (!music->openFromFile(musicPath))
    std::cout << "Could not play music :( - " << musicPath << "\n";
  m_playingMusic = move(music);

  m_playingMusic->setVolume(validVolume(m_musicVolume * m_masterVolume));
  m_playingMusic->play();

  graduallyChangeMusicVolume(ChangeMusicVolume::INCREASE);
}

void
SoundPlayer::stopMusic() {
  if (m_playingMusic == nullptr)
    return;

  graduallyChangeMusicVolume(ChangeMusicVolume::DECREASE);
}

void
SoundPlayer::setMasterVolume(double value) {
  m_masterVolume = value;
}

double
SoundPlayer::getMasterVolume() {
  return m_masterVolume;
}

void
SoundPlayer::setSoundVolume(double value) {
  m_soundVolume = value * volumeScaler;
}

double
SoundPlayer::getSoundVolume() {
  return m_soundVolume / volumeScaler;
}

void
SoundPlayer::setMusicVolume(double value) {
  m_musicVolume = value * volumeScaler;
  if (m_playingMusic)
    m_playingMusic->setVolume(validVolume(m_musicVolume * m_masterVolume));
}

double
SoundPlayer::getMusicVolume() {
  return m_musicVolume / volumeScaler;
}

void
SoundPlayer::graduallyChangeMusicVolume(ChangeMusicVolume value) {
  m_graduayllChange = true;
  m_changeDirection = value;

  static double change{ 20.0 };
  if (value == ChangeMusicVolume::INCREASE) {
    m_changeValue = change;
    m_targetVolume = m_musicVolume;
    m_startVolume = std::max(0.0, m_changeVolume);
  } else {
    m_changeValue = -change;
    m_targetVolume = 0.0;
    m_startVolume = std::min(m_musicVolume, m_changeVolume);
  }
  m_changeVolume = m_startVolume;
}

} /* namespace util */
