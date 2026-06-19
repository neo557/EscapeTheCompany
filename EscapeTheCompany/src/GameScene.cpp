#include <SFML/Graphics.hpp>
#include "GameScene.h"
#include "PlayerStatusManager.h"
#include "SceneManager.h"
#include "BattleScene.h"
#include "GameScene2.h"
#include "SaveData.h"
#include "SpringGimmick.h"
#include "ItemManager.h"


GameScene::GameScene(sf::RenderWindow* window, Player* player, EnemyManager* mgr, bool returnedFromBattle)
: windowRef(window), player(player), enemyManager(mgr), justReturnedFromBattle(returnedFromBattle)
{
	printf("enemydatabase size: %zu\n", enemyManager->enemyDatabase.size());
	printf("[DEBUG] GameScene ctor: spritePath = %s\n", player->statusManager->spritePath.c_str());
	printf("[GS] mgr = %p, enemyManager = %p\n", mgr, enemyManager);

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

	printf("[GAMESCENE CTOR] player pos=(%f,%f)\n",
		player->worldPos.x, player->worldPos.y);
	for (auto& e : enemyManager->enemies) {
		printf("[GAMESCENE CTOR] enemy id=%d pos=(%f,%f)\n",
			e->data.id, e->worldPos.x, e->worldPos.y);
	}
}

void GameScene::onEnter() {
	auto& sm = SceneManager::instance();
	
	if (sm.resultEnemyRef) {
		sm.enemyManager.removeEnemy(sm.resultEnemyRef->data.id);
		sm.resultEnemyRef = nullptr;   // 忘れずにクリア
	}
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

	printf("[GAMESCENE ENTER] player pos=(%f,%f)\n",
		player->worldPos.x, player->worldPos.y);
}

void GameScene ::onExit() {
	// ゲームシーンを抜けるときの処理
}
	
void GameScene::handleEvent(const sf::Event& event) {
	auto& sm = SceneManager::instance();
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
		sm.requestScene(NextSceneType::PlayerStatusScene, false);
		return;
	}

	if (!sm.gs1talkdone && !justReturnedFromBattle && player->worldPos.x > 500 && !eventTriggered) {
		sm.startText("CharacterData/Synario_Talk.csv");
		sm.gs1talkdone = true;
	}
}
void GameScene::update(float dt) {
	auto& sm = SceneManager::instance();

	// 1. プレイヤー・Enemy更新
	player->update(dt, tilemap);
	enemyManager->update(dt, tilemap);

	// 2. タイルID更新（最優先）
	sf::Vector2f foot = player->getFootPosition();
	static int lastTileId = -1;
	int tileId = tilemap.getTileIdAt(foot.x, foot.y);
	player->currentTileId = tileId;
	SceneManager::instance().onSceneChangeTile(tileId);


	// 3. ギミック処理（ChangeScene含む）
	SpringGimmickType type = tilemap.getGimmickType(foot.x, foot.y);
	if (type != SpringGimmickType::None) {
		SpringGimmick gimmick(
			SpringGimmick::requiredSpringFor(type),
			type
		);
		gimmick.onTrigger(player, dt);
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
		sm.enemyManager.lastEncounterPos = collidedEnemy->worldPos;
		player->resetInput();
		sm.battleEnemyRef = collidedEnemy;
		sm.battleAllowedSprings = allowedSprings;
		sm.requestScene(NextSceneType::BattleScene, false);
	}
	justReturnedFromBattle = false;

	// 5. UI更新
	float ratio = player->statusManager->getHpRatio();
	hpFront.setSize(sf::Vector2f(200 * ratio, 20));

	// 6. カメラ追従
	camera.follow(
		player->worldPos,
		windowRef->getSize(),
		tilemap.actualWidth * TileMap::TILE_SIZE,
		tilemap.actualHeight * TileMap::TILE_SIZE
	);

	// Spring 表示更新
	SpringType cur = player->statusManager->currentSpring;

	switch (cur) {
	case SpringType::None:     springText.setString("None"); break;
	case SpringType::Normal:   springText.setString("Normal Spring"); break;
	case SpringType::Fire:     springText.setString("Fire Spring"); break;
	case SpringType::Ice:      springText.setString("Ice Spring"); break;
	case SpringType::Electric: springText.setString("Electric Spring"); break;
	}
	springText.setFont(font);
	springText.setCharacterSize(24);
	springText.setFillColor(sf::Color::White);
	springText.setPosition(30, 800);
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