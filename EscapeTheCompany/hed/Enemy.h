#pragma once
#include <SFML/Graphics.hpp>
#include "CharacterData.h"
#include "TileMap.h"

class Enemy {
public: 
	sf::Sprite sprite;

	const CharacterData data; 
	int hp;
	int attack;
	int defence;
	int speed;
	int expValue;

	sf::Vector2f worldPos;
	sf::FloatRect getBounds() const;
	sf::Vector2f velocity;
	bool onGround = false;

	Enemy(const CharacterData* data, sf::Vector2f startPos);

	void update(float dt, TileMap& map);
	void draw(sf::RenderWindow& window);
	void moveAndCollide(TileMap& map, float dt);
};