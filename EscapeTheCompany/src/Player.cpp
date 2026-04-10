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
			velocity.x = -50.0f; // 左に移動
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			velocity.x = 50.0f; // 右に移動
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
			if (map.tiles[tileY][tileX] != 0)  // solid tile
			{
				// タイルの上に乗せる
				worldPos.y = tileY * TileMap::TILE_SIZE - 50;
				velocity.y = 0;
				isOnGround = true;
			}
		}

		// 壁との衝突
		moveAndCollide(map, dt);

	}

	void Player::draw(sf::RenderWindow& window)
	{
		// 見た目を同期
		sprite.setPosition(worldPos);
		window.draw(sprite);
	}

	sf::FloatRect Player::getBounds() const
	{
		return sf::FloatRect(worldPos.x, worldPos.y, 50, 50);
	}


	void Player::moveAndCollide(TileMap& map, float dt)
	{
		// 移動予定位置
		sf::FloatRect next = getBounds();
		next.left += velocity.x * dt;
		next.top += velocity.y * dt;

		// タイル範囲
		int startX = next.left / TileMap::TILE_SIZE;
		int endX = (next.left + next.width) / TileMap::TILE_SIZE;
		int startY = next.top / TileMap::TILE_SIZE;
		int endY = (next.top + next.height) / TileMap::TILE_SIZE;

		// 衝突フラグ
		//isOnGround = false;

		// タイルと AABB の衝突チェック
		for (int y = startY; y <= endY; y++) {
			for (int x = startX; x <= endX; x++) {

				if (!map.isSolidAt(x * TileMap::TILE_SIZE, y * TileMap::TILE_SIZE))
					continue;

				sf::FloatRect tileRect(
					x * TileMap::TILE_SIZE,
					y * TileMap::TILE_SIZE,
					TileMap::TILE_SIZE,
					TileMap::TILE_SIZE
				);

				sf::FloatRect playerRect = next;

				if (playerRect.intersects(tileRect)) {

					float dx1 = (tileRect.left + tileRect.width) - playerRect.left;
					float dx2 = (playerRect.left + playerRect.width) - tileRect.left;
					float dy1 = (tileRect.top + tileRect.height) - playerRect.top;
					float dy2 = (playerRect.top + playerRect.height) - tileRect.top;

					float resolveX = (dx1 < dx2) ? dx1 : -dx2;
					float resolveY = (dy1 < dy2) ? dy1 : -dy2;

					if (std::abs(resolveX) < std::abs(resolveY)) {
						next.left += resolveX;
						velocity.x = 0;
					}
					else {
						next.top += resolveY;
						velocity.y = 0;

						if (resolveY < 0) {
							isOnGround = true;
						}
					}
				}
			}
		}

		// 修正後の位置を反映
		worldPos.x = next.left;
		worldPos.y = next.top;
	}