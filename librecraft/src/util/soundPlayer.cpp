#include "soundPlayer.h"

#include <ThreadPool.h>
#include <chrono>
#include <iostream>
#include <iterator>

#include "globalResources.h"

namespace util {

void SoundPlayer::playSound(const std::string &soundPath) {
  for (auto sound = m_playingSounds.begin(); sound < m_playingSounds.end();
      ++sound) {
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
  globalResources::g_threadPool.enqueue([this] {
    graduallyChangeMusicVolume(ChangeMusicVolume::INCREASE);
  });
}

void SoundPlayer::stopMusic() {
  if (m_playingMusic == nullptr)
    return;

  globalResources::g_threadPool.enqueue([this] {
    graduallyChangeMusicVolume(ChangeMusicVolume::DECREASE);
    m_playingMusic->stop();
  });
}

void SoundPlayer::setMasterVolume(double value)
{
  m_masterVolume = value;
}

void SoundPlayer::setSoundVolume(double value)
{
  m_soundVolume = value;
}

void SoundPlayer::setMusicVolume(double value)
{
  m_musicVolume = value;
  if (m_playingMusic)
    m_playingMusic->setVolume(m_musicVolume * m_masterVolume);
}

void SoundPlayer::graduallyChangeMusicVolume(ChangeMusicVolume value) {
  return;

  // This causes error when exiting the game!

  static std::chrono::milliseconds oneMilliSecond(1);

  double changeValue{0.001};
  double targetVolume{m_musicVolume};
  double startVolume{0.0};

  if (value == ChangeMusicVolume::DECREASE) {
    changeValue = -changeValue;
    targetVolume = 0.0;
    startVolume = m_musicVolume;
  }

  for (double volume{startVolume};
      value == ChangeMusicVolume::DECREASE ?
          volume >= targetVolume :
          volume <= targetVolume; volume += changeValue)
  {
    m_playingMusic->setVolume(volume);
    std::this_thread::sleep_for(oneMilliSecond);
  }
}


} /* namespace util */
