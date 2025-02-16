#pragma once
#include "raylib.h"

class Player {
private:
    Vector2 position;
    Texture2D texture;
    static constexpr int MAX_HEALTH = 5;
    static constexpr int INITIAL_HEALTH = 5;

public:
    int health = INITIAL_HEALTH;
    static constexpr float PLAYER_SPEED = 5.0f;
    static constexpr int HEALTH_BAR_HEIGHT = 5;
    static constexpr int HEALTH_BAR_OFFSET = 15;

    Player(float startX, float startY, Texture2D playerTexture);
    void move();
    void draw() const;

    float getX() const;
    float getY() const;
    Vector2 getPosition() const;
    void setPosition(Vector2 newPosition);
    float getRadius() const;
    int getHealth() const;
    void takeDamage();
    void reset();
};
