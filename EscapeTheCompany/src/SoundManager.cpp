#include "SoundManager.h"

void SoundManager::load(const std::string& name, const std::string& path) {
    sf::SoundBuffer buffer;
    if (buffer.loadFromFile(path)) {
        buffers[name] = buffer;
        sounds[name].setBuffer(buffers[name]);
        sounds[name].setVolume(volume);
    }
}

void SoundManager::play(const std::string& name) {
    auto it = sounds.find(name);
    if (it != sounds.end()) {
        it->second.play();
    }
}

void SoundManager::setVolume(float vol) {
    volume = vol;
    for (auto& s : sounds) {
        s.second.setVolume(volume);
    }
}
