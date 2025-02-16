#include "Player.h"
#include <algorithm>
#include <cmath>

Player::Player(float startX, float startY, Texture2D playerTexture)
    : position({ static_cast<float>(startX), static_cast<float>(startY) }), texture(playerTexture) {
}

void Player::move() {
    float moveX = 0.0f;
    float moveY = 0.0f;

    if (IsKeyDown(KEY_A)) moveX -= 1.0f;
    if (IsKeyDown(KEY_D)) moveX += 1.0f;
    if (IsKeyDown(KEY_W)) moveY -= 1.0f;
    if (IsKeyDown(KEY_S)) moveY += 1.0f;

    float magnitude = static_cast<float>(sqrt(pow(moveX, 2) + pow(moveY, 2)));
    if (magnitude != 0.0f) {
        moveX /= magnitude;
        moveY /= magnitude;
    }

    position.x += moveX * PLAYER_SPEED;
    position.y += moveY * PLAYER_SPEED;

    float screenWidth = static_cast<float>(GetScreenWidth());
    float screenHeight = static_cast<float>(GetScreenHeight());
    position.x = std::clamp(position.x, 0.0f, screenWidth - static_cast<float>(texture.width));
    position.y = std::clamp(position.y, 0.0f, screenHeight - static_cast<float>(texture.height));
}

void Player::draw() const {
    DrawTexture(texture, static_cast<int>(position.x) - texture.width / 2, static_cast<int>(position.y) - texture.height / 2, WHITE);

    int barWidth = texture.width;
    float healthRatio = static_cast<float>(health) / static_cast<float>(MAX_HEALTH);

    healthRatio = std::max(0.0f, std::min(1.0f, healthRatio));

    DrawRectangle(static_cast<int>(position.x) - HEALTH_BAR_OFFSET, static_cast<int>(position.y + texture.height),
        barWidth, HEALTH_BAR_HEIGHT, GRAY);

    DrawRectangle(static_cast<int>(position.x) - HEALTH_BAR_OFFSET, static_cast<int>(position.y + texture.height),
        static_cast<int>(barWidth * healthRatio), HEALTH_BAR_HEIGHT, BLUE);
}

float Player::getX() const {
    return position.x;
}

float Player::getY() const {
    return position.y;
}

Vector2 Player::getPosition() const {
    return position;
}

void Player::setPosition(Vector2 newPosition) {
    position = newPosition;
}

float Player::getRadius() const {
    return texture.width / 2.0f;
}

int Player::getHealth() const {
    return health;
}

void Player::takeDamage() {
    health -= 1;
    if (health < 0) {
        health = 0;
    }
}

void Player::reset() {
    health = INITIAL_HEALTH;
    setPosition({ 50.0f, 50.0f });
}
