#pragma once
#include <SFML/Graphics.hpp>
#include "Enemy.h"

class EnemyManager {
public:
	std::vector<Enemy> enemies;

	sf::Vector2f lastEncounterPos; // プレイヤーの最後の位置を保持する変数

	void spawn(const std::string& texturePath, sf::Vector2f pos);
	void update(float dt, TileMap& map);
	void draw(sf::RenderWindow& window);
	Enemy* checkCollision(const sf::FloatRect& playerBounds);

};