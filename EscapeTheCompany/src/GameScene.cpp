#include <SFML/Graphics.hpp>
#include "GameScene.h"
#include "SceneManager.h"
#include "BattleScene.h"

GameScene::GameScene(sf::RenderWindow* window) {
	windowRef = window; // RenderWindowの参照を保持
	tilemap.loadCSV("TileMap\\Scene1_Gr1.csv", 1);
	font.loadFromFile("Fonts\\KH-Dot-Dougenzaka-12.ttf");

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


	switch (player.currentSpring) {
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


	// 敵との衝突判定
	if (player.getBounds().intersects(enemySymbol.getGlobalBounds())) {
		// 衝突した場合、BattleSceneに遷移
		SceneManager::instance().changeScene<BattleScene>(enemySymbol, windowRef);
	}
}

void GameScene::draw(sf::RenderWindow& window) {
	camera.apply(window);
	tilemap.draw(window, camera.view);
	player.draw(window);
	window.draw(enemySymbol); // 敵のシンボルを描画

	//ここから下はカメラ追従あり
	window.setView(window.getDefaultView()); // デフォルトビューに戻す

	window.draw(springText);
	
}