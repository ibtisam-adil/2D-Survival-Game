#pragma once
#include <unordered_map>
#include <string>
#include "raylib.h"

class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();
    void loadTexture(const std::string& key, const std::string& filePath);
    Texture2D getTexture(const std::string& key);
    void unloadAllTextures();

private:
    std::unordered_map<std::string, Texture2D> textures;
};
