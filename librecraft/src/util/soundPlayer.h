#ifndef SRC_UTIL_SOUNDPLAYER_H_
#define SRC_UTIL_SOUNDPLAYER_H_

#include <map>
#include <vector>
#include <memory>

#include <SFML/Audio.hpp>

namespace util {

/**
 * @brief Class for managing all music and sounds.
 */
class SoundPlayer {
public:

  SoundPlayer() = default;

  ~SoundPlayer() = default;

  SoundPlayer(SoundPlayer const&) = delete;

  SoundPlayer(SoundPlayer &&) = delete;

  void operator=(SoundPlayer const&) = delete;

  void operator=(SoundPlayer &&) = delete;



  void update(double time);

  /**
   * @brief Plays the sound specified by the argument.
   *
   * @param soundPath The path to the sound file.
   */
  void playSound(const std::string &soundPath);

  /**
   * @brief Plays the music specified by the argument.
   *
   * @param musicPath
   */
  void playMusic(const std::string &musicPath);

  /**
   * @brief Stops the music if any is currently playing.
   */
  void stopMusic();

  void setMasterVolume(double value);

  double getMasterVolume();

  void setSoundVolume(double value);

  double getSoundVolume();

  void setMusicVolume(double value);

  double getMusicVolume();

private:

  enum class ChangeMusicVolume {
    INCREASE, DECREASE
  };

  /**
   * @brief Changes the volume of the currently playing music.
   *
   * @param value Specifies if the volume should increase or decreas.
   */
  void graduallyChangeMusicVolume(ChangeMusicVolume value);

  std::map<std::string, sf::SoundBuffer> m_buffers;
  std::vector<std::shared_ptr<sf::Sound>> m_playingSounds;
  std::shared_ptr<sf::Music> m_playingMusic;

  // TODO Load these from config files...
  double m_masterVolume{1.0};
  double m_soundVolume{100.0};
  double m_musicVolume{100.0};
};

} /* namespace util */

#endif /* SRC_UTIL_SOUNDPLAYER_H_ */
