#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>

class SoundManager {
public :
	void load(const std::string& name, const std::string& path);
	void play(const std::string& name);
	void setVolume(float vol);

private :
	std::unordered_map<std::string, sf::SoundBuffer> buffers;
	std::unordered_map<std::string, sf::Sound> sounds;
	float volume = 100.f;
};