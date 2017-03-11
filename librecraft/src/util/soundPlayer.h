#ifndef SRC_UTIL_SOUNDPLAYER_H_
#define SRC_UTIL_SOUNDPLAYER_H_

#include <map>
#include <vector>
#include <memory>

#include <SFML/Audio.hpp>

namespace util {

class SoundPlayer {

  SoundPlayer() = default;

  SoundPlayer(SoundPlayer const&) = delete;

  void operator=(SoundPlayer const&) = delete;

public:

  static SoundPlayer& getInstance() {
    static SoundPlayer INSTANCE;
    return INSTANCE;
  }

  void playSound(const std::string &soundPath);

  void playMusic(const std::string &musicPath);

  void stopMusic();

private:

  enum class ChangeMusicVolume {
    INCREASE, DECREASE
  };

  void graduallyChangeMusicVolume(ChangeMusicVolume value);

  std::map<std::string, sf::SoundBuffer> m_buffers;
  std::vector<std::shared_ptr<sf::Sound>> m_playingSounds;
  std::shared_ptr<sf::Music> m_playingMusic;

  // TODO Load these from confi files...
  double m_soundVolume{2.0};
  double m_musicVolume{2.0};
};

} /* namespace util */

#endif /* SRC_UTIL_SOUNDPLAYER_H_ */
