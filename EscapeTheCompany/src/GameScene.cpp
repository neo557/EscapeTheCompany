#include <SFML/Graphics.hpp>
#include "GameScene.h"
#include "SceneManager.h"
#include "BattleScene.h"

GameScene::GameScene(sf::RenderWindow* window, Player* player,bool returnedFromBattle)
: windowRef(window), playerRef(player),justReturnedFromBattle(returnedFromBattle) {
	tilemap.loadCSV("TileMap\\Scene1_Gr1.csv", 1);
	font.loadFromFile("Fonts\\KH-Dot-Dougenzaka-12.ttf");

	enemySymbol.setSize(sf::Vector2f(50, 50));
	enemySymbol.setFillColor(sf::Color::Red);
	enemySymbol.setPosition(400, 590); // 敵の位置を設定

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
		playerRef->worldPos.x = enemySymbol.getPosition().x - 60;
		playerRef->worldPos.y = enemySymbol.getPosition().y ;
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
	if (!justReturnedFromBattle &&
		playerRef->getBounds().intersects(enemySymbol.getGlobalBounds())) {

		playerRef->resetInput();
		SceneManager::instance().changeScene<BattleScene>(playerRef, enemySymbol, windowRef);
	}
	justReturnedFromBattle = false;
}

void GameScene::draw(sf::RenderWindow& window) {
	camera.apply(window);
	tilemap.draw(window, camera.view);
	playerRef->draw(window);
	window.draw(enemySymbol); // 敵のシンボルを描画

	//ここから下はカメラ追従あり
	window.setView(window.getDefaultView()); // デフォルトビューに戻す
	window.draw(hpBack);
	window.draw(hpFront);
	window.draw(springText);
	
}