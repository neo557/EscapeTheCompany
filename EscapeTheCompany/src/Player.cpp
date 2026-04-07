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
		worldPos.x += velocity.x * dt;

		// 地面との衝突
		// 足元のタイル座標
		int tileX = worldPos.x / TileMap::TILE_SIZE;
		int tileY = (worldPos.y + 50) / TileMap::TILE_SIZE;

		// 範囲チェック
		if (tileX >= 0 && tileX < TileMap::WIDTH &&
			tileY >= 0 && tileY < TileMap::HEIGHT)
		{
			if (map.tiles[tileY][tileX].type == 1) // solid tile
			{
				// タイルの上に乗せる
				worldPos.y = tileY * TileMap::TILE_SIZE - 50;
				velocity.y = 0;
				isOnGround = true;
			}
		}

	}

	void Player::draw(sf::RenderWindow& window)
	{
		// 見た目を同期
		sprite.setPosition(worldPos);
		window.draw(sprite);
	}
