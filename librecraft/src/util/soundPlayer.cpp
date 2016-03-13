
#include "soundPlayer.h"

#include <iostream>
#include <iterator>

namespace util {

void SoundPlayer::playSound(const std::string &soundPath) {

	for (auto sound = playingSounds.begin(); sound < playingSounds.end(); ++sound) {
		if ((*sound)->getStatus() == sf::SoundSource::Status::Stopped) {
			playingSounds.erase(sound);
			std::cout << "Removing sound :D \n";
		}
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
	playingSounds.push_back(sound);
}

void SoundPlayer::playMusic(const std::string &musicPath) {
	auto music = std::make_shared<sf::Music>();
	if (!music->openFromFile(musicPath))
		std::cout << "Could not play music :( - " << musicPath << "\n";
	music->play();
	playingMusic.push_back(music);
}

} /* namespace util */
