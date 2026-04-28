#pragma once
#include <SFML/Graphics.hpp>
#include "SpringType.h"

class TileMap; // 前方宣言

class Player {

public:
    sf::Vector2f worldPos;
    sf::Vector2f velocity;
    sf::Sprite sprite;
    sf::Vector2f logicsize;
	sf::Vector2f drawsize;
	sf::Vector2f hitboxoffset;
	sf::Vector2f battlesize;
    sf::FloatRect getBounds() const;

    int maxHp = 100;
    int hp = 100;

    bool isOnGround = false;
    bool moveLeft = false;
    bool moveRight = false;
    bool jumpPressed = false;

	bool justReturnedFromBattle = false; // バトルから戻った直後かどうかのフラグ

    Player();
    SpringType currentSpring = SpringType::None;
    void handleEvent(const sf::Event& event);
    void update(float dt, TileMap& map);
    void draw(sf::RenderWindow& window);
    void moveAndCollide(TileMap& map, float dt);
    void resetInput();
};