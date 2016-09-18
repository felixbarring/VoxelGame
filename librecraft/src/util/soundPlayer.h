
#ifndef SRC_UTIL_SOUNDPLAYER_H_
#define SRC_UTIL_SOUNDPLAYER_H_

#include <map>
#include <vector>
#include <memory>

#include <SFML/Audio.hpp>

namespace util {

class SoundPlayer {
// ########################################################
// Constructor/Destructor #################################
// ########################################################

	SoundPlayer() {};

	SoundPlayer(SoundPlayer const&) = delete;

	void operator=(SoundPlayer const&) = delete;

// ########################################################
// Member Functions########################################
// ########################################################

public:

	static SoundPlayer& getInstance() {
		static SoundPlayer INSTANCE;
		return INSTANCE;
	}

	void playSound(const std::string &soundPath);

	void playMusic(const std::string &musicPath);

	void stopMusic();

// ########################################################
// Implementation #########################################
// ########################################################

	std::map<std::string, sf::SoundBuffer> m_buffers;
	std::vector<std::shared_ptr<sf::Sound>> m_playingSounds;
	std::shared_ptr<sf::Music> m_playingMusic;

};

} /* namespace util */

#endif /* SRC_UTIL_SOUNDPLAYER_H_ */
