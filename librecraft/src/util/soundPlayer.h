
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

	virtual ~SoundPlayer() { };

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

// ########################################################
// Implementation #########################################
// ########################################################

	std::map<std::string, sf::SoundBuffer> m_buffers;
	std::vector<std::shared_ptr<sf::Sound>> playingSounds;
	std::vector<std::shared_ptr<sf::Music>> playingMusic;

};

} /* namespace util */

#endif /* SRC_UTIL_SOUNDPLAYER_H_ */