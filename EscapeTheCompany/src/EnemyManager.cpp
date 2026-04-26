#include <SFML/Graphics.hpp>
#include "EnemyManager.h"
#include "TileMap.h"

void EnemyManager::spawn(const std::string& texturePath, sf::Vector2f pos) {
	enemies.emplace_back(texturePath, pos);
}

void EnemyManager::update(float dt, TileMap& map) {
	for (auto& enemy : enemies) {
		enemy.update(dt, map);
	}
}

void EnemyManager::draw(sf::RenderWindow& window) {
	for (auto& enemy : enemies) {
		enemy.draw(window);
	}
}

Enemy* EnemyManager::checkCollision(const sf::FloatRect& playerBounds) {
	for (auto& enemy : enemies) {
		if (playerBounds.intersects(enemy.sprite.getGlobalBounds())) {
			return &enemy;
		}
	}
	return nullptr;
}