#pragma once
#include "raylib.h"

#pragma once
#include "raylib.h"

class Bomb {
public:
    static constexpr int MAX_HEALTH = 5; // Add a constant for max health
    static constexpr int HEALTH_BAR_HEIGHT = 5; // Add a constant for health bar height
    static constexpr float RADIUS_DIVIDER = 2.0f; // Add a constant for the radius divider

    int health = MAX_HEALTH;
    Bomb(Vector2 position, Texture2D texture);

    void randomizePosition();
    void checkCollision(float pX, float pY);
    void draw() const;

    Vector2 getPosition() const;
    float getRadius() const;
    bool getIsAlive() const;
    void setAlive(bool status);
    void takeDamage();

private:
    Vector2 position;
    bool isAlive;
    Texture2D texture;
};
