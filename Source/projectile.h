#pragma once
#include "raylib.h"
#include <raymath.h>

class Projectile {
public:
    static constexpr float DEFAULT_LIFETIME = 0.6f;
    static constexpr int DEFAULT_HEALTH = 1;
    static constexpr int MAX_HEALTH = 1;
    static constexpr int BAR_HEIGHT = 5;

    float lifeTime;
    int health;
    float speed;
    bool active;
    Vector2 position;
    Vector2 velocity;
    Vector2 playerPosition;
    Texture2D texture;

    Projectile(Vector2 playerPos, Vector2 direction, float lifeTime = DEFAULT_LIFETIME,
        float speed = 300.0f, Texture2D texture = Texture2D())
        : playerPosition(playerPos), lifeTime(lifeTime), speed(speed), active(true), texture(texture),
        health(DEFAULT_HEALTH) {
        position = playerPos;
        Vector2 normalizedDirection = Vector2Normalize(direction);
        velocity = { normalizedDirection.x * speed, normalizedDirection.y * speed };
    }

    void move();
    void draw() const;
    bool isActive() const;
    void setActive(bool state);
    void takeDamage();
    float getRadius() const;
    Vector2 getPosition() const { return position; }
};
