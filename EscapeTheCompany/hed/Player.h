#pragma once
#include <SFML/Graphics.hpp>

class TileMap; // 前方宣言

class Player {
public:
    sf::Vector2f worldPos;
    sf::Vector2f velocity;
    sf::RectangleShape sprite;
	sf::FloatRect getBounds() const;
	bool isOnGround = false;

    Player();
    void update(float dt, TileMap& map);
    void draw(sf::RenderWindow& window);
    void moveAndCollide(TileMap& map, float dt);
};
