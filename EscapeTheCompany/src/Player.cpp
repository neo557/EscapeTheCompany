#include <SFML/Graphics.hpp>
#include "Player.h"
#include "TileMap.h"


Player::Player() {
    sf::Texture* texture = new sf::Texture();
    texture->loadFromFile("photo/G_Ball.png");
    sprite.setTexture(*texture);
    worldPos = sf::Vector2f(0, 500);
    isOnGround = false;
    logicsize = sf::Vector2f(64, 64);
    drawsize = sf::Vector2f(64, 64);
    hitboxoffset = sf::Vector2f(0, 0);
    battlesize = sf::Vector2f(512, 512);

}

void Player::update(float dt, TileMap& map)
{
    float w = logicsize.x;
    float h = logicsize.y;

    // --- 横移動 ---
    if (moveLeft)
        velocity.x = -200.0f;
    else if (moveRight)
        velocity.x = 200.0f;
    else
        velocity.x = 0.0f;

    // --- ジャンプ ---
    if (jumpPressed && isOnGround)
    {
        velocity.y = -500;
        isOnGround = false;

        if (currentSpring == SpringType::Normal)
            velocity.y = -800;
    }

    // --- 重力 ---
    velocity.y += 900 * dt;
    if (velocity.y > 800) velocity.y = 800;

    // --- 垂直移動 ---
    worldPos.y += velocity.y * dt;

    // 足元判定位置（logicSize ベース）
    float leftFoot = worldPos.x + hitboxoffset.x + 5;
    float rightFoot = worldPos.x + hitboxoffset.x + w - 5;
    float footY = worldPos.y + hitboxoffset.y + h;

    // --- 下方向（地面） ---
    if (velocity.y > 0)
    {
        if (map.isSolidAt(leftFoot, footY) ||
            map.isSolidAt(rightFoot, footY))
        {
            int tileY = footY / TileMap::TILE_SIZE;
            worldPos.y = tileY * TileMap::TILE_SIZE - h - hitboxoffset.y;
            velocity.y = 0;
            isOnGround = true;
        }
    }

    // --- 上方向（天井） ---
    if (velocity.y < 0)
    {
        float headY = worldPos.y + hitboxoffset.y;
        if (map.isSolidAt(leftFoot, headY) ||
            map.isSolidAt(rightFoot, headY))
        {
            int tileY = headY / TileMap::TILE_SIZE;
            worldPos.y = (tileY + 1) * TileMap::TILE_SIZE - hitboxoffset.y;
            velocity.y = 0;
        }
    }

    // --- 横移動 ---
    worldPos.x += velocity.x * dt;

    float head = worldPos.y + hitboxoffset.y + 5;
    float waist = worldPos.y + hitboxoffset.y + h / 2;
    float foot = worldPos.y + hitboxoffset.y + h - 5;

    // --- 右壁 ---
    if (velocity.x > 0)
    {
        float rightX = worldPos.x + hitboxoffset.x + w;
        if (map.isSolidAt(rightX, head) ||
            map.isSolidAt(rightX, waist) ||
            map.isSolidAt(rightX, foot))
        {
            int tileX = rightX / TileMap::TILE_SIZE;
            worldPos.x = tileX * TileMap::TILE_SIZE - w - hitboxoffset.x;
            velocity.x = 0;
        }
    }

    // --- 左壁 ---
    if (velocity.x < 0)
    {
        float leftX = worldPos.x + hitboxoffset.x;
        if (map.isSolidAt(leftX, head) ||
            map.isSolidAt(leftX, waist) ||
            map.isSolidAt(leftX, foot))
        {
            int tileX = leftX / TileMap::TILE_SIZE;
            worldPos.x = (tileX + 1) * TileMap::TILE_SIZE - hitboxoffset.x;
            velocity.x = 0;
        }
    }
}


void Player::resetInput() {
	moveLeft = false;
	moveRight = false;
	jumpPressed = false;
	velocity.x = 0;
}

void Player::draw(sf::RenderWindow& window)
{
    // 描画サイズに合わせてスケール
    sprite.setScale(
        drawsize.x / sprite.getTexture()->getSize().x,
        drawsize.y / sprite.getTexture()->getSize().y
    );

    // 見た目の位置は worldPos に合わせる
    sprite.setPosition(worldPos);

    window.draw(sprite);
}

sf::FloatRect Player::getBounds() const
{
	return sf::FloatRect(
		worldPos.x + hitboxoffset.x,
		worldPos.y + hitboxoffset.y,
		logicsize.x,
		logicsize.y
	);
}

void Player::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::A) moveLeft = true;
		if (event.key.code == sf::Keyboard::D) moveRight = true;
		if (event.key.code == sf::Keyboard::Space) jumpPressed = true;
	}

	if (event.type == sf::Event::KeyReleased) {
		if (event.key.code == sf::Keyboard::A) moveLeft = false;
		if (event.key.code == sf::Keyboard::D) moveRight = false;
		if (event.key.code == sf::Keyboard::Space) jumpPressed = false;
	}
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
		int next = (int)currentSpring + 1;
		if (next > (int)SpringType::Fire) next = 0;
		currentSpring = (SpringType)next;
	}
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