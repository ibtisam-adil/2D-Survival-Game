#pragma once

#include <algorithm>
#include "Gui.h"
#include <vector>
#include <stack>
#include "Player.h"
#include "Projectile.h"
#include "Minion.h"
#include "Bomb.h"
#include "Boss.h"
#include "ResourceManager.h"
#include "SoundManager.h"

enum class GameState {
    MAIN_MENU,
    GAME,
    GAME_OVER
};

class Game {
public:
    Game();
    ~Game();

    void run();
    void pushGameState(GameState state);
    void popGameState();
    void spawnMinions(std::vector<Minion>& minions, Boss& boss, double currentTime, ResourceManager& resourceManager);

private:
    ResourceManager localResourceManager;
    SoundManager soundManager;

    std::stack<GameState> stateStack;

    bool isGameOver = false;

    void loadResources();
    void cleanupResources();

    void drawMainMenu(Player& player, Boss& boss, std::vector<Projectile>& projectiles,
        std::vector<Minion>& minions, std::vector<Bomb>& bombs);
    void drawGame(Player& player, std::vector<Projectile>& projectiles,
        std::vector<Minion>& minions, std::vector<Bomb>& bombs, Boss& boss);
    void drawGameOver(Player& player);
    void checkWinCondition(std::vector<Minion>& minions, std::vector<Bomb>& bombs, Boss& boss);

    void handleProjectiles(Player& player, std::vector<Projectile>& projectiles);
    void handleMinions(Player& player, std::vector<Projectile>& projectiles, std::vector<Minion>& minions);
    void handleBombs(Player& player, std::vector<Bomb>& bombs, std::vector<Projectile>& projectiles);
    void handleBoss(Boss& boss, std::vector<Projectile>& projectiles);

    void resetState(Player& player, Boss& boss, std::vector<Bomb>& bombs,
        std::vector<Minion>& minions, std::vector<Projectile>& projectiles);

    float GetRandomValueF(int min, int max);
    Vector2 calculateProjectileDirection() const;
};

