#include "SoundManager.h"
#include "raylib.h"

SoundManager::SoundManager() {}

SoundManager::~SoundManager() {
    unloadAllSounds();
}

void SoundManager::loadSound(const std::string& key, const std::string& filePath) {
    Sound sound = LoadSound(filePath.c_str());
    sounds[key] = sound;
}

void SoundManager::playSound(const std::string& key) {
    auto it = sounds.find(key);
    if (it != sounds.end()) {
        PlaySound(it->second);
    }
}

void SoundManager::unloadAllSounds() {
    for (auto& pair : sounds) {
        UnloadSound(pair.second);
    }
    sounds.clear();
}
