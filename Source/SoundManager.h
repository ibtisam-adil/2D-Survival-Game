#pragma once

#include <string>
#include <unordered_map>
#include "raylib.h"

class SoundManager {
public:
    SoundManager();
    ~SoundManager();

    void loadSound(const std::string& key, const std::string& filePath);

    void playSound(const std::string& key);

    void unloadAllSounds();

private:
    std::unordered_map<std::string, Sound> sounds;
};

