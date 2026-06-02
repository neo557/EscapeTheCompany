#pragma once
#include <SFML/Graphics.hpp>
#include "SpringType.h"
#include "PlayerStatusManager.h"

class TileMap; // 前方宣言

class Player {

public:
    sf::Vector2f worldPos;
    sf::Vector2f velocity;
    sf::Sprite sprite;
    sf::FloatRect getBounds() const {
        return sf::FloatRect(
            worldPos.x + statusManager->hitboxOffset.x,
            worldPos.y + statusManager->hitboxOffset.y,
            statusManager->logicSize.x,
            statusManager->logicSize.y
        );
    }
    PlayerStatusManager* statusManager;
    TileMap* tileMap;

    bool isOnGround = false;
    bool moveLeft = false;
    bool moveRight = false;
    bool jumpPressed = false;
	bool justReturnedFromBattle = false; // バトルから戻った直後かどうかのフラグ
	bool slip = false;
    float stuntimer = 0.f;

    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

Player();
    SpringType currentSpring = SpringType::None;
	void init(const PlayerStatusManager& status);
    void handleEvent(const sf::Event& event);
    void update(float dt, TileMap& map);
    void debugDrawHitbox(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    void moveAndCollide(TileMap& map, float dt);
    void resetInput();
	sf::Vector2f getFootPosition() const;
};