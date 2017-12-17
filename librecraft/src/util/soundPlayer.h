#ifndef SRC_UTIL_SOUNDPLAYER_H_
#define SRC_UTIL_SOUNDPLAYER_H_

#include <map>
#include <memory>
#include <vector>

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

  SoundPlayer(SoundPlayer&&) = delete;

  void operator=(SoundPlayer const&) = delete;

  void operator=(SoundPlayer&&) = delete;

  /**
   * @brief Updates the sound player.
   *
   * This should be called each frame at enables the sound player do handle
   * housekeeping such as gradually change sound volume and remove sounds that
   * are no longer playing.
   *
   * @param time The time that has passed for the last frame.
   */
  void update(double time);

  /**
   * @brief Plays the sound specified by the argument.
   *
   * @param soundPath The path to the sound file.
   */
  void playSound(const std::string& soundPath);

  /**
   * @brief Plays the music specified by the argument.
   *
   * @param musicPath
   */
  void playMusic(const std::string& musicPath);

  /**
   * @brief Stops the music if any is currently playing.
   */
  void stopMusic();

  /**
   * @brief Sets the master volume.
   *
   * This affects both music and sounds, since their volume is multiplied by
   * the master volume.
   *
   * @param value The new value of the master volume. Should be in the the range
   *              of [0, 1] where 0 is no volume and 1 is max volume.
   */
  void setMasterVolume(double value);

  /**
   * @return Returns the current master volume value in the the range [0, 1]
   *         where 0 is no volume and 1 is max volume.
   */
  double getMasterVolume();

  /**
   * @brief Sets the volume of the sounds.
   *
   * All sounds that are played with playSound will be affected by this setting.
   *
   * @param value The new volume of the sounds. Should be in the range of
   *              [0, 1] where 0 is no volume and 1 is max volume.
   */
  void setSoundVolume(double value);

  /**
   * @return Returns the current sound volume value in the the range [0, 1]
   *         where 0 is no volume and 1 is max volume.
   */
  double getSoundVolume();

  /**
   * @brief Sets the volume of the music.
   *
   * The music that is played will be affected by this setting.
   *
   * @param value The new volume of the music. Should be in the range of
   *              [0, 1] where 0 is no volume and 1 is max volume.
   */
  void setMusicVolume(double value);

  /**
   * @return Returns the current music volume value in the the range [0, 1]
   *         where 0 is no volume and 1 is max volume.
   */
  double getMusicVolume();

private:
  enum class ChangeMusicVolume { INCREASE, DECREASE };

  /**
   * @brief Changes the volume of the currently playing music.
   *
   * @param value Specifies if the volume should increase or decrease.
   */
  void graduallyChangeMusicVolume(ChangeMusicVolume value);

  std::map<std::string, sf::SoundBuffer> m_buffers;
  std::vector<std::unique_ptr<sf::Sound>> m_playingSounds;
  std::unique_ptr<sf::Music> m_playingMusic;

  bool m_graduayllChange{ false };
  double m_changeVolume{};
  ChangeMusicVolume m_changeDirection{ ChangeMusicVolume::INCREASE };
  double m_changeValue{};
  double m_targetVolume{};
  double m_startVolume{};

  // TODO Load these from config files...
  double m_masterVolume{ 1.0 };
  double m_soundVolume{ 100.0 };
  double m_musicVolume{ 100.0 };
};

} /* namespace util */

#endif /* SRC_UTIL_SOUNDPLAYER_H_ */
