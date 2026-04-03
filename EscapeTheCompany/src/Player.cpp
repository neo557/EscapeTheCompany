#include <SFML/Graphics.hpp>
#include "Player.h"
#include "TileMap.h"


	Player::Player() {
		sprite.setSize(sf::Vector2f(50, 50));
		sprite.setFillColor(sf::Color::White);
		worldPos = sf::Vector2f(0, 500);
		isOnGround = false;
	}

	void Player::update(float dt, TileMap& map)
	{
		// 左右移動
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			velocity.x = -200.0f; // 左に移動
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			velocity.x = 200.0f; // 右に移動
		}
		else
		{
			velocity.x = 0.0f; // 停止
		}
		// ジャンプ
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && isOnGround)
		{
			velocity.y = -500;
			isOnGround = false;
		}
		// 重力
		velocity.y += 900 * dt;
		if (velocity.y > 800) velocity.y = 800;

		worldPos.y += velocity.y * dt;

		// 地面との衝突
		if (map.isSolidAt(worldPos.x, worldPos.y + 50)) {
			worldPos.y = (TileMap::HEIGHT - 1) * TileMap::TILE_SIZE - 50;
			velocity.y = 0;
			isOnGround = true;
		}

		// 見た目を同期
		sprite.setPosition(worldPos);
	}

	void Player::draw(sf::RenderWindow& window)
	{
		sprite.setPosition(worldPos);
		window.draw(sprite);
	}
