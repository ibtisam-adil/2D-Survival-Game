#pragma once
#include "raylib.h"
#include <random>

constexpr int MAX_HEALTH = 40;  
constexpr int SPEED = 100;     
constexpr int HEALTH_BAR_OFFSET = 60;  

class Boss {
public:
    
    Boss(Vector2 position, Texture2D texture);
    void draw() const;
    void move(); 
    void randomizeDirection();

    Vector2 getPosition() const;
    Vector2 setPosition();
    bool isAlive() const;
    void setAlive(bool alive);
    void takeDamage();
    int getHealth() const;
    void setHealth();
    float getRadius() const;
private:
    Vector2 position;
    Vector2 direction;
    Texture2D texture;
    bool alive = true;
    int health = 40;
    float cornerWaitTimer = 0.0f;
    float cornerWaitDuration = static_cast<float>(GetRandomValue(1, 5)) / 10.0f;
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution;
};
