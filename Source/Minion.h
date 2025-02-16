#pragma once
#include "raylib.h"

class Minion {
public:
    static constexpr int MINION_SPEED = 250; 
    static constexpr int HEALTH_BAR_HEIGHT = 5; 
    static constexpr int MAX_HEALTH = 2; 

    Minion(Vector2 pos, Texture2D tex);
    Minion();

    void randomizePosition();
    void randomizeDirection();
    void checkCollision(float pX, float pY);
    void move();
    void draw() const;

    Vector2 getPosition() const;
    float getRadius() const;
    bool getIsAlive() const;
    void setAlive(bool alive);
    void takeDamage(int damage);
    void reset(Vector2 newPosition);
    void setPosition(Vector2 newPosition);

    void resetHealth();

private:
    Vector2 position;
    Vector2 direction;
    bool isAlive;
    Texture2D texture;
    float timer = 0.0f;
    float randomInterval;
    int health = MAX_HEALTH; 
    int maxHealth = MAX_HEALTH; 
};