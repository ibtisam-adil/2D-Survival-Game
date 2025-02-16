#include "ResourceManager.h"
#include <iostream>
#include <stdexcept>
#include <string>

ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager() {
    unloadAllTextures();
}

void ResourceManager::loadTexture(const std::string& key, const std::string& filePath) {
    if (textures.find(key) != textures.end()) {
        return;
    }

    Texture2D texture = LoadTexture(filePath.c_str());
    if (texture.id == 0) { 
        return;
    }

    textures[key] = texture;
}

Texture2D ResourceManager::getTexture(const std::string& key) {
    auto it = textures.find(key);
    if (it != textures.end()) {
        return it->second;
    }
    else {
        return Texture2D{};
    }
}

void ResourceManager::unloadAllTextures() {
    for (auto& pair : textures) {
        if (pair.second.id != 0) {
            UnloadTexture(pair.second);
        }
    }
    textures.clear();
}
