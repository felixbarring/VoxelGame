#include "soundPlayer.h"

#include <ThreadPool.h>
#include <chrono>
#include <iostream>
#include <iterator>

#include "globalResources.h"

namespace util {

double kek{100};

void SoundPlayer::update(double time) {
  m_changeVolume += changeValue * time;
  if (!(m_graduayllChange && (m_changeDirection == ChangeMusicVolume::DECREASE ?
      m_changeVolume >= targetVolume :
      m_changeVolume <= targetVolume)))
  {
    m_changeVolume = targetVolume;
    m_graduayllChange = false;
  }
  m_playingMusic->setVolume(m_changeVolume);
}

void SoundPlayer::playSound(const std::string &soundPath) {
  for (auto sound = m_playingSounds.begin(); sound < m_playingSounds.end();
      ++sound)
  {
    if ((*sound)->getStatus() == sf::SoundSource::Status::Stopped)
      m_playingSounds.erase(sound);
  }

  if (m_buffers.find(soundPath) == m_buffers.end()) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(soundPath))
      std::cout << "Could not play sound :( - " << soundPath << "\n";
    m_buffers.emplace(soundPath, std::move(buffer));
  }

  auto sound = std::make_shared<sf::Sound>();
  sound->setBuffer((*m_buffers.find(soundPath)).second);
  sound->play();
  sound->setVolume(m_soundVolume * m_masterVolume);
  m_playingSounds.push_back(sound);
}

void SoundPlayer::playMusic(const std::string &musicPath) {
  auto music = std::make_shared<sf::Music>();
  if (!music->openFromFile(musicPath))
    std::cout << "Could not play music :( - " << musicPath << "\n";
  music->play();
  music->setVolume(m_musicVolume * m_masterVolume);
  m_playingMusic = music;

  graduallyChangeMusicVolume(ChangeMusicVolume::INCREASE);
}

void SoundPlayer::stopMusic() {
  if (m_playingMusic == nullptr)
    return;

  graduallyChangeMusicVolume(ChangeMusicVolume::DECREASE);
//  m_playingMusic->stop();
}

void SoundPlayer::setMasterVolume(double value)
{
  m_masterVolume = value;
}

double SoundPlayer::getMasterVolume() {
  return m_masterVolume;
}

void SoundPlayer::setSoundVolume(double value)
{
  m_soundVolume = value * kek;
}

double SoundPlayer::getSoundVolume() {
  return m_soundVolume / kek;
}

void SoundPlayer::setMusicVolume(double value)
{
  m_musicVolume = value * kek;
  if (m_playingMusic)
    m_playingMusic->setVolume(m_musicVolume * m_masterVolume);
}

double SoundPlayer::getMusicVolume() {
  return m_musicVolume / kek;
}

void SoundPlayer::graduallyChangeMusicVolume(ChangeMusicVolume value) {
  m_graduayllChange = true;
  m_changeDirection = value;

  static double change{20.0};
  if (value == ChangeMusicVolume::INCREASE) {
    changeValue = change;
    targetVolume = m_musicVolume;
    startVolume = std::max(0.0, m_changeVolume);
  } else {
    changeValue = -change;
    targetVolume = 0.0;
    startVolume = std::min(m_musicVolume, m_changeVolume);
  }
  m_changeVolume = startVolume;
}


} /* namespace util */
