#pragma once
#include <SFML/Graphics.hpp>
#include "SpringType.h"

class TileMap; // 前方宣言

class Player {

public:
    sf::Vector2f worldPos;
    sf::Vector2f velocity;
    sf::RectangleShape sprite;
	sf::FloatRect getBounds() const;

	bool isOnGround = false;
    bool moveLeft = false;
    bool moveRight = false;
    bool jumpPressed = false;

    Player();
    SpringType currentSpring = SpringType::None;
	void handleEvent(const sf::Event& event);
    void update(float dt, TileMap& map);
    void draw(sf::RenderWindow& window);
    void moveAndCollide(TileMap& map, float dt);
};
