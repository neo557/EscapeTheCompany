#include <SFML/Graphics.hpp>
#include "GameScene.h"
#include "SceneManager.h"
#include "BattleScene.h"

GameScene::GameScene(sf::RenderWindow* window, Player* player,bool returnedFromBattle)
: windowRef(window), playerRef(player),justReturnedFromBattle(returnedFromBattle),enemy(enemy)
{
	tilemap.loadCSV("TileMap\\Scene1_Bg1.csv", 0);
	tilemap.loadCSV("TileMap\\Scene1_Gr1.csv", 1);
	font.loadFromFile("Fonts\\KH-Dot-Dougenzaka-12.ttf");

	// 敵キャラの初期化
	enemyManager.loadEnemyData();
	enemyManager.spawn(1,{1200, 850});
	enemyManager.spawn(2, { 1500, 840 });
	printf("GameScene ctor: enemyManager spawned\n");
	// 背景（灰）
	hpBack.setSize(sf::Vector2f(200, 20));
	hpBack.setFillColor(sf::Color(100, 100, 100));
	hpBack.setPosition(50, 50);

	// 前面（緑）
	hpFront.setSize(sf::Vector2f(200, 20));
	hpFront.setFillColor(sf::Color::Green);
	hpFront.setPosition(50, 50);


}

void GameScene::onEnter() {
	if (justReturnedFromBattle) {
		// 戦闘開始時に保存した座標を使う
		playerRef->worldPos = SceneManager::instance().enemyManager.lastEncounterPos + sf::Vector2f(-60, 0);

		playerRef->resetInput();
	}
}

void GameScene ::onExit() {
	// ゲームシーンを抜けるときの処理
}
	
void GameScene::handleEvent(const sf::Event& event) {
	// キー入力など
	playerRef->handleEvent(event);

	
}
void GameScene::update(float dt) {
	playerRef->update(dt, tilemap);
	camera.follow(playerRef->worldPos);

	//EnemyData

	enemyManager.update(dt, tilemap);
	printf("enemyPosition:(%f,%f)\n", enemy.worldPos.x, enemy.worldPos.y);

	float ratio = (float)playerRef->hp / playerRef->maxHp;
	hpFront.setSize(sf::Vector2f(200 * ratio, 20));

	switch (playerRef->currentSpring) {
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

	printf("Spring Position: (%f, %f)\n", springText.getPosition().x, springText.getPosition().y);


	// --- BattleScene から戻った直後は衝突判定をスキップ ---
	if (justReturnedFromBattle) {
		justReturnedFromBattle = false;
		return;
	}

	// --- 敵との衝突判定（1 回だけ） ---
	Enemy* collidedEnemy = enemyManager.checkCollision(playerRef->getBounds());
	if (!justReturnedFromBattle &&
		collidedEnemy != nullptr) {
		printf("Player collided at: (%f, %f)\n",
			playerRef->worldPos.x,
			playerRef->worldPos.y);
		printf("Enemy collided at: (%f, %f)\n",
			collidedEnemy->worldPos.x,
			collidedEnemy->worldPos.y
		);
		SceneManager::instance().enemyManager.lastEncounterPos = collidedEnemy->worldPos;
		playerRef->resetInput();
		SceneManager::instance().changeScene<BattleScene>(playerRef, collidedEnemy, windowRef);
	}
	justReturnedFromBattle = false;
}

void GameScene::draw(sf::RenderWindow& window) {
	camera.apply(window);
	tilemap.draw(window, camera.view);
	playerRef->draw(window);
	enemyManager.draw(window);//敵キャラ描画

	//ここから下はカメラ追従あり
	window.setView(window.getDefaultView()); // デフォルトビューに戻す
	window.draw(hpBack);
	window.draw(hpFront);
	window.draw(springText);
	
}