
#include "soundPlayer.h"

#include <iostream>
#include <iterator>

namespace util {

void SoundPlayer::playSound(const std::string &soundPath) {

	for (auto sound = m_playingSounds.begin(); sound < m_playingSounds.end(); ++sound) {
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
	m_playingSounds.push_back(sound);
}

void SoundPlayer::playMusic(const std::string &musicPath) {
	auto music = std::make_shared<sf::Music>();
	if (!music->openFromFile(musicPath))
		std::cout << "Could not play music :( - " << musicPath << "\n";
	music->play();
	m_playingMusic = music;
}

void SoundPlayer::stopMusic() {
	m_playingMusic->stop();
}


} /* namespace util */
