#pragma once
#include <SFML/Graphics.hpp>
#include "TileMap.h"

class Enemy {
public: 
	sf::Sprite sprite;
	static sf::Texture sharedTexture;

	sf::Vector2f worldPos;
	sf::Vector2f velocity;
	sf::FloatRect getBounds() const;
	bool onGround = false;
	Enemy(const std::string& texturePath, sf::Vector2f startPos);

	void update(float dt, TileMap& map);
	void draw(sf::RenderWindow& window);
	void moveAndCollide(TileMap& map, float dt);


};