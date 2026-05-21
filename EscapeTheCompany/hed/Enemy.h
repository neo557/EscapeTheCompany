#pragma once
#include <SFML/Graphics.hpp>
#include "CharacterData.h"
#include "TileMap.h"
#include "SpringType.h"

class PlayerStatusManager; // 前方宣言
class Enemy {
public: 
	sf::Sprite sprite;
	
	const CharacterData data; 
	int hp;
	int attack;
	int defence;
	int speed;
	int expValue;
	SpringType springType = SpringType::None;

	sf::Vector2f worldPos;
	sf::FloatRect getBounds() const;
	sf::Vector2f velocity;
	bool onGround = false;
	int calcDamage(const PlayerStatusManager& player);
	Enemy(const CharacterData* data, sf::Vector2f startPos);

	void update(float dt, TileMap& map);
	void draw(sf::RenderWindow& window);
	void moveAndCollide(TileMap& map, float dt);
	
	float getSpringMultiplier(SpringType playerSpring) const;
};