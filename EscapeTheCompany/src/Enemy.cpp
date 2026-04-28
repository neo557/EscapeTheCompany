#include "Enemy.h"
#include "EnemyManager.h"



Enemy::Enemy(const EnemyData* data, sf::Vector2f startPos) : data(data),worldPos(startPos),hp(data->maxHp) {
	
	sprite.setPosition(worldPos);

}

void Enemy::update(float dt, TileMap& map) {
	velocity.y += 900 * dt;
	worldPos += velocity * dt;

	float w = data->logicSize.x;
	float h = data->logicSize.y;

	float leftFoot = worldPos.x + 5;
	float rightFoot = worldPos.x + w - 5;
	float footY = worldPos.y + h;

	// 下方向（地面）
	if (velocity.y > 0) {
		if (map.isSolidAt(leftFoot, footY) ||
			map.isSolidAt(rightFoot, footY)) {

			int tileY = footY / TileMap::TILE_SIZE;
			worldPos.y = tileY * TileMap::TILE_SIZE - h;
			velocity.y = 0;
			onGround = true;
		}
	}

	// 上方向（天井）
	if (velocity.y < 0) {
		float headY = worldPos.y;
		if (map.isSolidAt(leftFoot, headY) ||
			map.isSolidAt(rightFoot, headY)) {

			int tileY = headY / TileMap::TILE_SIZE;
			worldPos.y = (tileY + 1) * TileMap::TILE_SIZE;
			velocity.y = 0;
		}
	}

	sprite.setPosition(worldPos);
}

sf::FloatRect Enemy::getBounds() const {
	return sf::FloatRect(
		worldPos.x + data->hitboxoffset.x,
		worldPos.y + data->hitboxoffset.y,
		data->logicSize.x,
		data->logicSize.y
	);
}
void Enemy::draw(sf::RenderWindow& window) {
	window.draw(sprite);
}


void Enemy::moveAndCollide(TileMap& map, float dt)
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
						onGround = true;
					}
				}
			}
		}
	}

	// 修正後の位置を反映
	worldPos.x = next.left;
	worldPos.y = next.top;
}