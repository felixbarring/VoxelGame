#ifndef SRC_UTIL_SOUNDPLAYER_H_
#define SRC_UTIL_SOUNDPLAYER_H_

#include <map>
#include <vector>
#include <memory>

#include <SFML/Audio.hpp>

namespace util {

/**
 * @brief Singleton class for managing all music and sounds.
 */
class SoundPlayer {

  SoundPlayer() = default;

  ~SoundPlayer() = default;

  SoundPlayer(SoundPlayer const&) = delete;

  SoundPlayer(SoundPlayer &&) = delete;

  void operator=(SoundPlayer const&) = delete;

  void operator=(SoundPlayer &&) = delete;

public:

  /**
   * @return The single instance of this class.
   */
  static SoundPlayer& getInstance() {
    static SoundPlayer INSTANCE;
    return INSTANCE;
  }

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

  void setSoundVolume(double value);

  void setMusicVolume(double value);

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
  double m_soundVolume{2.0};
  double m_musicVolume{2.0};
};

} /* namespace util */

#endif /* SRC_UTIL_SOUNDPLAYER_H_ */
