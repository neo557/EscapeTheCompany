#include <SFML/Graphics.hpp>
#include "GameScene.h"
#include "PlayerStatusManager.h"
#include "SceneManager.h"
#include "BattleScene.h"
#include "GameScene2.h"
#include "SaveData.h"
#include "SpringGimmick.h"



GameScene::GameScene(sf::RenderWindow* window, Player* player, EnemyManager* mgr, bool returnedFromBattle)
: windowRef(window), player(player), enemyManager(mgr), justReturnedFromBattle(returnedFromBattle)
{
	tilemap.loadCSV("TileMap/Scene1_Bg1.csv", 0);
	tilemap.loadCSV("TileMap/Scene1_Gr1.csv", 1);
	font.loadFromFile("Fonts/KH-Dot-Dougenzaka-16.ttf");

	// 背景（灰）
	hpBack.setSize(sf::Vector2f(200, 20));
	hpBack.setFillColor(sf::Color(100, 100, 100));
	hpBack.setPosition(50, 50);

	// 前面（緑）
	hpFront.setSize(sf::Vector2f(200, 20));
	hpFront.setFillColor(sf::Color::Green);
	hpFront.setPosition(50, 50);

	printf("enemydatabase size: %zu\n", enemyManager->enemyDatabase.size());
	printf("[DEBUG] GameScene ctor: spritePath = %s\n", player->statusManager->spritePath.c_str());

}

void GameScene::onEnter() {
	if (justReturnedFromBattle) {
		// 戦闘開始時に保存した座標を使う
		player->worldPos = enemyManager->lastEncounterPos + sf::Vector2f(-80, 0);
	}
	allowedSprings = { SpringType::None,SpringType::Normal };
	player->resetInput();

	// デバッグ用ステータス表示
	printf("ATK=%d DEF=%d LV=%d EXP=%d nextExp=%d\n",
		player->statusManager->attack,
		player->statusManager->defence,
		player->statusManager->level,
		player->statusManager->Exp,
		player->statusManager->nextExp
	);

}

void GameScene ::onExit() {
	// ゲームシーンを抜けるときの処理
}
	
void GameScene::handleEvent(const sf::Event& event) {
	// キー入力など
	player->handleEvent(event);
	player->statusManager->onHandle(event,allowedSprings);

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab) {
		SaveData save;
		save.saveGame(player);
		printf("Game saved.\n");
	}

	//---StatusSceneへの遷移---
	if (event.type == sf::Event::KeyPressed &&
		event.key.code == sf::Keyboard::Escape) {
		SceneManager::instance().returnPos = player->worldPos; // 現在の座標を保存
		SceneManager::instance().changeScene<PlayerStatusScene>(windowRef, player);
		return;
	}
}
void GameScene::update(float dt) {


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

	//タイルIdの取得と処理	
	sf::Vector2f foot = player->getFootPosition();
	SpringGimmickType type = tilemap.getGimmickType(foot.x, foot.y);

	if (type != SpringGimmickType::None) {
		SpringGimmick gimmick(
			SpringGimmick::requiredSpringFor(type), // FireFloor → FireSpring
			type
		); 
		gimmick.onTrigger(player, dt);
	}
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
		SceneManager::instance().changeScene<BattleScene>(player, collidedEnemy, windowRef,allowedSprings);
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
		SceneManager::instance().changeScene<GameScene2>(windowRef, player, enemyManager, false);
		return;
	}


}

void GameScene::drawDebugHitboxes(sf::RenderWindow& window) {
	// Player
	drawRect(window, player->getBounds(), sf::Color::Red);

	// Enemies
	for (auto& e : enemyManager->enemies) {
		drawRect(window, e->getBounds(), sf::Color::Yellow);
	}


}
void GameScene::drawRect(sf::RenderWindow& window, const sf::FloatRect& r, sf::Color c) {
	sf::RectangleShape rect;
	rect.setPosition(r.left, r.top);
	rect.setSize({ r.width, r.height });
	rect.setFillColor(sf::Color(0, 0, 0, 0));
	rect.setOutlineColor(c);
	rect.setOutlineThickness(1.f);
	window.draw(rect);
}

void GameScene::draw(sf::RenderWindow& window) {
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