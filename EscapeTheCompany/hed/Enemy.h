#pragma once
#include <SFML/Graphics.hpp>
#include "EnemyData.h"
#include "TileMap.h"

class Enemy {
public: 
	sf::Sprite sprite;

	const EnemyData* data; // 敵のデータへのポインタ
	sf::Vector2f worldPos;
	sf::FloatRect getBounds() const;
	sf::Vector2f velocity;
	bool onGround = false;
	int hp = 0; // 追加: ヒットポイント

	Enemy(const EnemyData* data, sf::Vector2f startPos);

	void update(float dt, TileMap& map);
	void draw(sf::RenderWindow& window);
	void moveAndCollide(TileMap& map, float dt);
};