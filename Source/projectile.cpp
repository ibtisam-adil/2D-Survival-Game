#include "Projectile.h"
#include <raymath.h>
#include <random>

void Projectile::move() {
    position.x += velocity.x * GetFrameTime();
    position.y += velocity.y * GetFrameTime();
    lifeTime -= GetFrameTime();
    if (lifeTime <= 0) active = false;
    if (position.x < 0 || position.x > GetScreenWidth() || position.y < 0 || position.y > GetScreenHeight()) {
        active = false;
    }
}

void Projectile::draw() const {
    DrawTextureV(texture, position, WHITE);

    int barWidth = texture.width;
    float healthRatio = static_cast<float>(health) / static_cast<float>(MAX_HEALTH);

    healthRatio = std::max(0.0f, std::min(1.0f, healthRatio));

    DrawRectangle(static_cast<int>(position.x), static_cast<int>(position.y + texture.height),
        barWidth, BAR_HEIGHT, GRAY);

    DrawRectangle(static_cast<int>(position.x), static_cast<int>(position.y + texture.height),
        static_cast<int>(barWidth * healthRatio), BAR_HEIGHT, LIGHTGRAY);
}

bool Projectile::isActive() const {
    return active;
}

void Projectile::setActive(bool state) { active = state; }

float Projectile::getRadius() const {
    return texture.width / 2.0f;
}

void Projectile::takeDamage() {
    health -= 1;
}
