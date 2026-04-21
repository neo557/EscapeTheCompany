#include <SFML/Graphics.hpp>
#include "GameScene.h"
#include "SceneManager.h"
#include "BattleScene.h"

GameScene::GameScene() {
	tilemap.loadCSV("TileMap\\Scene1_Gr1.csv", 1);

	enemySymbol.setSize(sf::Vector2f(50, 50));
	enemySymbol.setFillColor(sf::Color::Red);
	enemySymbol.setPosition(400, 590); // 敵の位置を設定
}

void GameScene::onEnter() {
	// ゲームシーンに入ったときの処理
}

void GameScene ::onExit() {
	// ゲームシーンを抜けるときの処理
}
	
void GameScene::handleEvent(const sf::Event& event) {
	// キー入力など
	player.handleEvent(event);
}
void GameScene::update(float dt) {
	player.update(dt, tilemap);
	camera.follow(player.worldPos);

	// 敵との衝突判定
	if (player.getBounds().intersects(enemySymbol.getGlobalBounds())) {
		// 衝突した場合、BattleSceneに遷移
		SceneManager::instance().changeScene<BattleScene>(enemySymbol);
	}
}

void GameScene::draw(sf::RenderWindow& window) {
	camera.apply(window);
	tilemap.draw(window, camera.view);
	player.draw(window);
	window.draw(enemySymbol); // 敵のシンボルを描画
}