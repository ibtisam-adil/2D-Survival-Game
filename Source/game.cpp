#include "Game.h"

constexpr int INITIAL_WINDOW_WIDTH = 600;
constexpr int INITIAL_WINDOW_HEIGHT = 800;
constexpr Color MENU_BACKGROUND_COLOR = { 40, 40, 40, 255 };
constexpr Color GAME_BACKGROUND_COLOR = { 80, 40, 40, 255 };
constexpr float PROJECTILE_SPEED = 800.0f;
constexpr float BUTTON_WIDTH = 150.0f;
constexpr float BUTTON_HEIGHT = 40.0f;
constexpr float MAIN_MENU_BUTTON_WIDTH = 200.0f;
constexpr float MAIN_MENU_BUTTON_HEIGHT = 50.0f;
constexpr int INITIAL_BOMB_COUNT = 10;
std::stack<GameState> stateStack;

Game::Game() {
	InitWindow(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, "Exercise 1");
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	InitAudioDevice();
	SetExitKey(KEY_NULL);
	SetTargetFPS(60);

	loadResources();
	pushGameState(GameState::MAIN_MENU);
}

void Game::loadResources() {
	localResourceManager.loadTexture("player", "Assets/player.png");
	localResourceManager.loadTexture("minion", "Assets/minion.png");
	localResourceManager.loadTexture("projectile", "Assets/bullet.png");
	localResourceManager.loadTexture("bomb", "Assets/bomb.png");
	localResourceManager.loadTexture("boss", "Assets/boss.png");

	soundManager.loadSound("shoot", "Assets/shoot.wav");
	soundManager.loadSound("miss", "Assets/hit_ground.wav");
	soundManager.loadSound("hit", "Assets/hit.wav");
}

void Game::cleanupResources() {
	localResourceManager.unloadAllTextures();
	soundManager.unloadAllSounds();
	CloseAudioDevice();
	CloseWindow();
}

void Game::pushGameState(GameState state) {
	stateStack.push(state);
}

void Game::popGameState() {
	if (!stateStack.empty()) {
		stateStack.pop();
	}
}

Game::~Game() {
	cleanupResources();
}

void Game::run() {
	Player player(50, 50, localResourceManager.getTexture("player"));
	Boss boss(Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f }, localResourceManager.getTexture("boss"));
	std::vector<Projectile> projectiles;
	std::vector<Minion> minions;
	std::vector<Bomb> bombs;
	for (int i = 0; i < 10; ++i) {
		Vector2 pos;
		pos.x = GetRandomValueF(0, GetScreenWidth() - 50);
		pos.y = GetRandomValueF(200, GetScreenHeight() - 50);
		bombs.emplace_back(pos, localResourceManager.getTexture("bomb"));
	}


	while (!isGameOver) {
		double currentTime = GetTime();
		spawnMinions(minions, boss, currentTime, localResourceManager);

		BeginDrawing();
		ClearBackground(GAME_BACKGROUND_COLOR);

		GameState currentState = stateStack.top();
		switch (currentState) {
		case GameState::MAIN_MENU:
			drawMainMenu(player, boss, projectiles, minions, bombs);
			break;
		case GameState::GAME:
			drawGame(player, projectiles, minions, bombs, boss);
			break;
		case GameState::GAME_OVER:
			drawGameOver(player);
			break;
		}

		EndDrawing();
	}
}


void Game::drawMainMenu(Player& player, Boss& boss, std::vector<Projectile>& projectiles,
	std::vector<Minion>& minions, std::vector<Bomb>& bombs) {
	ClearBackground(MENU_BACKGROUND_COLOR);

	Rectangle startButton = { (GetScreenWidth() - BUTTON_WIDTH) / 2.0f, GetScreenHeight() / 3.0f, BUTTON_WIDTH, BUTTON_HEIGHT };
	Rectangle quitButton = { (GetScreenWidth() - BUTTON_WIDTH) / 2.0f, GetScreenHeight() / 2.0f, BUTTON_WIDTH, BUTTON_HEIGHT };

	if (gui_button(startButton, "START")) {
		resetState(player, boss, bombs, minions, projectiles);
		pushGameState(GameState::GAME);
	}

	if (gui_button(quitButton, "QUIT")) {
		isGameOver = true;
	}
}


void Game::drawGame(Player& player, std::vector<Projectile>& projectiles, std::vector<Minion>& minions,
	std::vector<Bomb>& bombs, Boss& boss) {
	player.move();
	player.draw();

	boss.move();
	boss.draw();

	handleProjectiles(player, projectiles);
	handleMinions(player, projectiles, minions);
	handleBombs(player, bombs, projectiles);
	handleBoss(boss, projectiles);
	checkWinCondition(minions, bombs, boss);

	if (IsKeyReleased(KEY_ESCAPE)) {
		popGameState();
	}
}


void Game::drawGameOver(Player& player) {
	ClearBackground({ 50, 50, 50, 255 });
	const char* message = player.getHealth() <= 0 ? "YOU LOSE!" : "YOU WIN!";
	float fontSize = 40.0f;

	Vector2 textSize = MeasureTextEx(GetFontDefault(), message, fontSize, 2.0f);
	DrawText(message, (GetScreenWidth() - static_cast<int>(textSize.x)) / 2,
		GetScreenHeight() / 3, static_cast<int>(fontSize), RAYWHITE);

	Rectangle mainMenuButton = { (GetScreenWidth() - MAIN_MENU_BUTTON_WIDTH) / 2.0f, GetScreenHeight() / 2.0f, MAIN_MENU_BUTTON_WIDTH, MAIN_MENU_BUTTON_HEIGHT };
	if (gui_button(mainMenuButton, "MAIN MENU")) {
		while (!stateStack.empty()) stateStack.pop();
		pushGameState(GameState::MAIN_MENU);
	}
}

void Game::handleProjectiles(Player& player, std::vector<Projectile>& projectiles) {
	static bool canShoot = true;

	if (canShoot && (IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT))) {
		Vector2 direction = calculateProjectileDirection();
		static float lifeTime = 0.6f;
		if (direction.x != 0.0f || direction.y != 0.0f) {
			projectiles.emplace_back(player.getPosition(), direction, lifeTime, PROJECTILE_SPEED, localResourceManager.getTexture("projectile"));
			soundManager.playSound("shoot");
			canShoot = false;
		}
	}

	if (IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_LEFT) || IsKeyReleased(KEY_RIGHT)) {
		canShoot = true;
	}

	for (auto it = projectiles.begin(); it != projectiles.end(); ) {
		if (!it->isActive()) {
			soundManager.playSound("miss");
			it = projectiles.erase(it);
		}
		else {
			it->move();
			it->draw();
			++it;
		}
	}
}

void Game::handleMinions(Player& player, std::vector<Projectile>& projectiles, std::vector<Minion>& minions) {
	for (auto& minion : minions) {
		if (minion.getIsAlive()) {
			minion.move();
			minion.draw();

			for (auto& projectile : projectiles) {
				if (CheckCollisionCircles(projectile.getPosition(), projectile.getRadius(), minion.getPosition(), minion.getRadius())) {
					minion.takeDamage(1);
					projectile.takeDamage();
					projectile.setActive(false);
					soundManager.playSound("hit");
					break;
				}
			}

			if (CheckCollisionCircles(player.getPosition(), player.getRadius(), minion.getPosition(), minion.getRadius())) {
				minion.takeDamage(2);
				player.takeDamage();
				soundManager.playSound("hit");
				if (player.getHealth() <= 0) {
					pushGameState(GameState::GAME_OVER);
				}
				break;
			}
		}
	}
}

void Game::handleBombs(Player& player, std::vector<Bomb>& bombs, std::vector<Projectile>& projectiles) {
	for (auto it = bombs.begin(); it != bombs.end(); ) {
		if (it->getIsAlive()) {
			it->draw();

			if (CheckCollisionCircles(player.getPosition(), player.getRadius(), it->getPosition(), it->getRadius())) {
				soundManager.playSound("hit");
				player.health -= 5;
				pushGameState(GameState::GAME_OVER);
			}

			for (auto& projectile : projectiles) {
				if (CheckCollisionCircles(projectile.getPosition(), projectile.getRadius(), it->getPosition(), it->getRadius())) {
					soundManager.playSound("hit");
					projectile.takeDamage();
					projectile.setActive(false);
					it->takeDamage();

					if (it->health <= 0) {
						it->setAlive(false);
					}
					break;
				}
			}
			++it;
		}
		else {
			it = bombs.erase(it);
		}
	}
}

void Game::handleBoss(Boss& boss, std::vector<Projectile>& projectiles) {
	for (auto& projectile : projectiles) {
		if (!boss.isAlive()) return;
		if (CheckCollisionCircles(projectile.getPosition(), projectile.getRadius(), boss.getPosition(), boss.getRadius())) {
			boss.takeDamage();
			projectile.takeDamage();
			projectile.setActive(false);
			soundManager.playSound("hit");

			if (boss.getHealth() <= 0) {
				boss.setAlive(false);
			}
			break;
		}
	}
}

Vector2 Game::calculateProjectileDirection() const {
	Vector2 direction = { 0.0f, 0.0f };
	if (IsKeyDown(KEY_RIGHT)) direction.x += 1.0f;
	if (IsKeyDown(KEY_LEFT)) direction.x -= 1.0f;
	if (IsKeyDown(KEY_UP)) direction.y -= 1.0f;
	if (IsKeyDown(KEY_DOWN)) direction.y += 1.0f;

	float length = sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length != 0.0f) {
		direction.x /= length;
		direction.y /= length;
		direction.x *= PROJECTILE_SPEED;
		direction.y *= PROJECTILE_SPEED;
	}
	return direction;
}

void Game::resetState(Player& player, Boss& boss, std::vector<Bomb>& bombs,
	std::vector<Minion>& minions, std::vector<Projectile>& projectiles) {
	player.reset();
	boss.setPosition();
	boss.setAlive(true);
	boss.setHealth();
	projectiles.clear();

	float currentTime = static_cast<float>(GetTime());
	minions.clear();
	spawnMinions(minions, boss, currentTime, localResourceManager);

	bombs.clear();
	for (int i = 0; i < INITIAL_BOMB_COUNT; ++i) {
		Vector2 pos = { GetRandomValueF(0, GetScreenWidth() - 50),
					   GetRandomValueF(200, GetScreenHeight() - 50) };
		bombs.emplace_back(pos, localResourceManager.getTexture("bomb"));
		bombs.back().setAlive(true);
	}
}

void Game::spawnMinions(std::vector<Minion>& minions, Boss& boss, double currentTime, ResourceManager& resourceManager) {
	static double lastMinionSpawnTime = 0.0;
	const float minionSpawnInterval = 0.5f;

	if (boss.isAlive()) {
		if (currentTime - lastMinionSpawnTime >= minionSpawnInterval) {
			Vector2 offset = { GetRandomValueF(-50, 50), GetRandomValueF(-50, 50) };
			Vector2 spawnPos = { boss.getPosition().x + offset.x, boss.getPosition().y + offset.y };
			minions.emplace_back(spawnPos, resourceManager.getTexture("minion"));

			lastMinionSpawnTime = currentTime;
		}
	}
}

void Game::checkWinCondition(std::vector<Minion>& minions, std::vector<Bomb>& bombs, Boss& boss) {
	bool allMinionsDead = std::all_of(minions.begin(), minions.end(), [](const Minion& minion) {
		return !minion.getIsAlive();
		});

	bool allBombsDestroyed = std::all_of(bombs.begin(), bombs.end(), [](const Bomb& bomb) {
		return !bomb.getIsAlive();
		});

	if (allMinionsDead && allBombsDestroyed && !boss.isAlive()) {
		pushGameState(GameState::GAME_OVER);
	}
}

float Game::GetRandomValueF(int min, int max) {
	return static_cast<float>(GetRandomValue(min, max));
}
