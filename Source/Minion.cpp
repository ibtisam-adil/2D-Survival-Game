#include "Minion.h"
#include <random>

Minion::Minion()
    : texture(), position{ 0, 0 }, direction{ 0, 0 }, isAlive(true), timer(0.0f), randomInterval(0.0f), health(MAX_HEALTH) {
}

Minion::Minion(Vector2 pos, Texture2D tex)
    : position(pos), texture(tex), isAlive(true), health(MAX_HEALTH) {
    randomizeDirection();
}

void Minion::randomizePosition() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> pos_dist(0.0f, static_cast<float>(GetScreenWidth()));
    std::uniform_real_distribution<float> y_pos_dist(0.0f, static_cast<float>(GetScreenHeight()));

    position.x = pos_dist(gen);
    position.y = y_pos_dist(gen);
}

void Minion::randomizeDirection() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dir_dist(-1.0f, 1.0f);

    direction.x = dir_dist(gen);
    direction.y = dir_dist(gen);

    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0.0f) {
        direction.x /= length;
        direction.y /= length;
    }
}

void Minion::checkCollision(float pX, float pY) {
    if (CheckCollisionPointCircle({ pX, pY }, position, getRadius())) {
        isAlive = false;
    }
}

void Minion::move() {
    if (isAlive) {
        const float screenWidth = static_cast<float>(GetScreenWidth());
        const float screenHeight = static_cast<float>(GetScreenHeight());

        position.x += static_cast<float>(MINION_SPEED) * direction.x * GetFrameTime();
        position.y += static_cast<float>(MINION_SPEED) * direction.y * GetFrameTime();

        if (position.x < 0) {
            position.x = 0;
            randomizeDirection();
        }
        if (position.x > screenWidth) {
            position.x = screenWidth;
            randomizeDirection();
        }
        if (position.y < 0) {
            position.y = 0;
            randomizeDirection();
        }
        if (position.y > screenHeight) {
            position.y = screenHeight;
            randomizeDirection();
        }
    }
}

void Minion::draw() const {
    if (isAlive) {
        const int texWidth = texture.width;
        const int texHeight = texture.height;

        DrawTexture(texture, static_cast<int>(position.x), static_cast<int>(position.y), WHITE);

        int barWidth = texWidth;
        int barHeight = HEALTH_BAR_HEIGHT; // Use constant here
        float healthRatio = static_cast<float>(health) / static_cast<float>(maxHealth);

        healthRatio = std::max(0.0f, std::min(1.0f, healthRatio));

        DrawRectangle(static_cast<int>(position.x), static_cast<int>(position.y + texHeight),
            barWidth, barHeight, GRAY);

        DrawRectangle(static_cast<int>(position.x), static_cast<int>(position.y + texHeight),
            static_cast<int>(barWidth * healthRatio), barHeight, YELLOW);
    }
}

Vector2 Minion::getPosition() const {
    return position;
}

float Minion::getRadius() const {
    return texture.width / 2.0f;
}

bool Minion::getIsAlive() const {
    return isAlive;
}

void Minion::setAlive(bool alive) {
    isAlive = alive;
}

void Minion::takeDamage(int damage) {
    if (isAlive) {
        health -= damage;
        if (health <= 0) {
            setAlive(false);
        }
    }
}

void Minion::setPosition(Vector2 newPosition) {
    position = newPosition;
}

void Minion::resetHealth() {
    health = MAX_HEALTH; // Use constant for health reset
}

void Minion::reset(Vector2 newPosition) {
    setPosition(newPosition);
    resetHealth();
    setAlive(true);
}