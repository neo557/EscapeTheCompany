#include <SFML/Graphics.hpp>
#include "TitleScene.h"
#include "GameScene.h"
#include "SceneManager.h"

TitleScene::TitleScene(sf::RenderWindow* window, Player* player, EnemyManager* mgr) {
	windowRef = window; // RenderWindowの参照を保持
	playerRef = player; // Playerの参照を保持
	enemyManager = mgr; // EnemyManagerの参照を保持
	font.loadFromFile("Fonts\\KH-Dot-Dougenzaka-12.ttf");

	titleText.setFont(font);
	titleText.setString("Escape The Company");
	titleText.setCharacterSize(48);
	titleText.setFillColor(sf::Color::White);
	titleText.setPosition(100, 150);

	pressKeyText.setFont(font);
	pressKeyText.setString("Press Any Key");
	pressKeyText.setCharacterSize(24);
	pressKeyText.setFillColor(sf::Color(200, 200, 200));
	pressKeyText.setPosition(150, 300);
}

void TitleScene::onEnter() {
	// タイトルシーンに入ったときの処理
}

void TitleScene::onExit() {
	// タイトルシーンを抜けるときの処理
}

void TitleScene::handleEvent(const sf::Event& event) {
	if (event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed) {
		// GameScene へ遷移
		SceneManager::instance().changeScene<GameScene>(windowRef, playerRef, &SceneManager::instance().enemyManager, false);
	}
}

void TitleScene::update(float dt) {
	// タイトルシーンの更新処理
}

void TitleScene::draw(sf::RenderWindow& window) {
	window.draw(titleText);
	window.draw(pressKeyText);
}