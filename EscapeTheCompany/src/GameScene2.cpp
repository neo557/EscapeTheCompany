#include <SFML/Graphics.hpp>
#include "GameScene.h"
#include "GameScene2.h"
#include "SceneManager.h"
#include "BattleScene.h"

GameScene2::GameScene2(sf::RenderWindow* window, Player* player, EnemyManager* mgr, bool returnedFromBattle)
	: windowRef(window), player(player), enemyManager(mgr), justReturnedFromBattle(returnedFromBattle)
{
	tilemap.loadCSV("TileMap\\Scene2_Bg2.csv", 0);
	tilemap.loadCSV("TileMap\\Scene2_Gr2.csv", 1);
	font.loadFromFile("Fonts\\KH-Dot-Dougenzaka-16.ttf");

	// 背景（灰）
	hpBack.setSize(sf::Vector2f(200, 20));
	hpBack.setFillColor(sf::Color(100, 100, 100));
	hpBack.setPosition(50, 50);

	// 前面（緑）
	hpFront.setSize(sf::Vector2f(200, 20));
	hpFront.setFillColor(sf::Color::Green);
	hpFront.setPosition(50, 50);

	printf("enemydatabase size: %zu\n", enemyManager->enemyDatabase.size());
	printf("[DEBUG] GameScene2 ctor: spritePath = %s\n", player->statusManager->spritePath.c_str());

}

void GameScene2::onEnter() {
	if (SceneManager::instance().lastStage == 2) {
		enemyManager->removeEnemy(); // 前のシーンから敵を引き継いでいる場合は一旦クリア
		enemyManager->spawn(2, { 1500, 800 });
		enemyManager->spawn(3, { 2000, 500 });
	}
	if (justReturnedFromBattle) {
		// 戦闘開始時に保存した座標を使う
		player->worldPos = enemyManager->lastEncounterPos + sf::Vector2f(-80, 0);
	}
	player->resetInput();
}

void GameScene2::onExit() {
	// ゲームシーンを抜けるときの処理
}

void GameScene2::handleEvent(const sf::Event& event) {
	// キー入力など
	player->handleEvent(event);


}
void GameScene2::update(float dt) {


	camera.follow(
		player->worldPos,
		windowRef->getSize(),
		tilemap.actualWidth * TileMap::TILE_SIZE,
		tilemap.actualHeight * TileMap::TILE_SIZE
	);
	player->update(dt, tilemap);

	//EnemyData
	enemyManager->update(dt, tilemap);

	float ratio = player->statusManager->getHpRatio();
	hpFront.setSize(sf::Vector2f(200 * ratio, 20));

	switch (player->statusManager->currentSpring) {
	case SpringType::None: springText.setString("None"); break;
	case SpringType::Normal: springText.setString("Normal Spring"); break;
	case SpringType::Fire: springText.setString("Fire Spring"); break;
	case SpringType::Ice: springText.setString("Ice Spring"); break;
	case SpringType::Electric: springText.setString("Electric Spring"); break;
	}

	springText.setFont(font);
	springText.setCharacterSize(24);
	springText.setFillColor(sf::Color::White);
	springText.setPosition(30, 800); // 左下

	//printf("Spring Position: (%f, %f)\n", springText.getPosition().x, springText.getPosition().y);


	// --- BattleScene から戻った直後は衝突判定をスキップ ---
	if (justReturnedFromBattle) {
		justReturnedFromBattle = false;
		return;
	}

	// --- 敵との衝突判定（1 回だけ） ---
	Enemy* collidedEnemy = enemyManager->checkCollision(player->getBounds());
	if (!justReturnedFromBattle &&
		collidedEnemy != nullptr) {
		printf("Player collided at: (%f, %f)\n",
			player->worldPos.x,
			player->worldPos.y);
		printf("Enemy collided at: (%f, %f)\n",
			collidedEnemy->worldPos.x,
			collidedEnemy->worldPos.y
		);
		SceneManager::instance().enemyManager.lastEncounterPos = collidedEnemy->worldPos;
		player->resetInput();
		SceneManager::instance().changeScene<BattleScene>(player, collidedEnemy, windowRef);
	}
	justReturnedFromBattle = false;

	//---Scene2への遷移許可判定---
	sf::FloatRect stage2Gate(1800, 0, 100, 900);

	if (!SceneManager::instance().st2 && stage2Gate.intersects(player->getBounds()))
	{
		//ステージ２の開放条件	
		SceneManager::instance().st2 = true;
		SceneManager::instance().lastStage = 2;
		//Playerの座標を初期化
		player->worldPos = sf::Vector2f(100, 700);

		///戻り地座標の更新
		SceneManager::instance().returnPos = player->worldPos;

		//ステージ遷移
		SceneManager::instance().changeScene<GameScene>(windowRef, player, enemyManager, false);
		return;
	}
}

void GameScene2::drawDebugHitboxes(sf::RenderWindow& window) {
	// Player
	drawRect(window, player->getBounds(), sf::Color::Red);

	// Enemies
	for (auto& e : enemyManager->enemies) {
		drawRect(window, e.getBounds(), sf::Color::Yellow);
	}


}
void GameScene2::drawRect(sf::RenderWindow& window, const sf::FloatRect& r, sf::Color c) {
	sf::RectangleShape rect;
	rect.setPosition(r.left, r.top);
	rect.setSize({ r.width, r.height });
	rect.setFillColor(sf::Color(0, 0, 0, 0));
	rect.setOutlineColor(c);
	rect.setOutlineThickness(1.f);
	window.draw(rect);
}

void GameScene2::draw(sf::RenderWindow& window) {
	camera.apply(window);
	tilemap.draw(window, camera.view);
	player->draw(window);
	enemyManager->draw(window);//敵キャラ描画
	if (SceneManager::instance().debugHitBox) {
		drawDebugHitboxes(window);
	}

	//ここから下はカメラ追従あり
	window.setView(window.getDefaultView()); // デフォルトビューに戻す
	window.draw(hpBack);
	window.draw(hpFront);
	window.draw(springText);


}