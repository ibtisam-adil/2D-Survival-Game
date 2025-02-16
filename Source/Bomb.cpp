
#include "Bomb.h"
#include <random>

Bomb::Bomb(Vector2 position, Texture2D texture) : position(position), texture(texture), isAlive(true) {}

void Bomb::randomizePosition() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> pos_dist(0.0f, static_cast<float>(GetScreenWidth()));
    std::uniform_real_distribution<float> y_pos_dist(0.0f, static_cast<float>(GetScreenHeight()));

    position.x = pos_dist(gen);
    position.y = y_pos_dist(gen);
}

void Bomb::checkCollision(float pX, float pY) {
    if (CheckCollisionPointCircle({ pX, pY }, { position.x, position.y }, getRadius())) {
        isAlive = false;
    }
}

void Bomb::draw() const {
    if (isAlive) {
        const int texWidth = texture.width;
        const int texHeight = texture.height;

        DrawTexture(texture, static_cast<int>(position.x), static_cast<int>(position.y), WHITE);

        int barWidth = texWidth;
        int barHeight = HEALTH_BAR_HEIGHT; // Use constant here
        float healthRatio = static_cast<float>(health) / static_cast<float>(MAX_HEALTH);

        healthRatio = std::max(0.0f, std::min(1.0f, healthRatio));

        DrawRectangle(static_cast<int>(position.x), static_cast<int>(position.y + texHeight),
            barWidth, barHeight, GRAY);

        DrawRectangle(static_cast<int>(position.x), static_cast<int>(position.y + texHeight),
            static_cast<int>(barWidth * healthRatio), barHeight, GOLD);
    }
}

Vector2 Bomb::getPosition() const {
    return { position.x, position.y };
}

float Bomb::getRadius() const {
    return texture.width / RADIUS_DIVIDER;
}

bool Bomb::getIsAlive() const {
    return isAlive;
}

void Bomb::setAlive(bool alive) {
    isAlive = alive;
}

void Bomb::takeDamage() {
    health -= 1;
    if (health < 0) {
        health = 0;
    }
}