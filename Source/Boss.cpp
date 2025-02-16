#include "Boss.h"
#include <random>

Boss::Boss(Vector2 position, Texture2D texture)
	: position(position), texture(texture), alive(true), health(MAX_HEALTH) {
	randomizeDirection();
}

void Boss::draw() const {
	if (isAlive()) {
		DrawTexture(texture, static_cast<int>(position.x) - texture.width / 2, static_cast<int>(position.y) - texture.height / 2, WHITE);
		int barWidth = texture.width;
		int barHeight = 5;
		float healthRatio = static_cast<float>(health) / static_cast<float>(MAX_HEALTH);

		healthRatio = std::max(0.0f, std::min(1.0f, healthRatio));

		DrawRectangle(static_cast<int>(position.x) - HEALTH_BAR_OFFSET, static_cast<int>(position.y + texture.height - HEALTH_BAR_OFFSET),
			barWidth, barHeight, GRAY);

		DrawRectangle(static_cast<int>(position.x) - HEALTH_BAR_OFFSET, static_cast<int>(position.y + texture.height - HEALTH_BAR_OFFSET),
			static_cast<int>(barWidth * healthRatio), barHeight, ORANGE);
	}
}

void Boss::move() {
	if (isAlive()) {
		if (cornerWaitTimer > 0.0f) {
			cornerWaitTimer -= GetFrameTime();
			return;
		}

		position.x += static_cast<float>(SPEED) * direction.x * GetFrameTime();
		position.y += static_cast<float>(SPEED) * direction.y * GetFrameTime();

		if (position.x < 0) {
			position.x = 0;
			cornerWaitTimer = cornerWaitDuration;
			randomizeDirection();
		}
		if (position.x > static_cast<float>(GetScreenWidth())) {
			position.x = static_cast<float>(GetScreenWidth());
			cornerWaitTimer = cornerWaitDuration;
			randomizeDirection();
		}
		if (position.y < 0) {
			position.y = 0;
			cornerWaitTimer = cornerWaitDuration;
			randomizeDirection();
		}
		if (position.y > static_cast<float>(GetScreenHeight())) {
			position.y = static_cast<float>(GetScreenHeight());
			cornerWaitTimer = cornerWaitDuration;
			randomizeDirection();
		}
	}
}

Vector2 Boss::getPosition() const {
	return position;
}

Vector2 Boss::setPosition() {
	position.x = GetScreenWidth() / 2.0f;
	position.y = GetScreenHeight() / 2.0f;;
	return position;
}

bool Boss::isAlive() const {
	return alive;
}

void Boss::setAlive(bool isAlive) {
	this->alive = isAlive;
}

void Boss::takeDamage() {
	health -= 1;
	if (health <= 0) {
		health = 0;
		alive = false;
	}
}

int Boss::getHealth() const {
	return health;
}

void Boss::setHealth() {
	health = MAX_HEALTH;
}

float Boss::getRadius() const {
	return texture.width / 2.0f;
}



void Boss::randomizeDirection() {
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