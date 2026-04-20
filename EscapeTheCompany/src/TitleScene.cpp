#include <SFML/Graphics.hpp>
#include "TitleScene.h"
#include "GameScene.h"
#include "SceneManager.h"

TitleScene::TitleScene() {
	font.loadFromFile("Fonts\\KH-Dot-Dougenzaka-12.ttf");

	titleText.setFont(font);
	titleText.setString("Escape The Company");
	titleText.setCharacterSize(48);
	titleText.setFillColor(sf::Color::White);
	titleText.setPosition(100, 150);

	pressKeyText.setFont(font);
	pressKeyText.setString("Press Enter");
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
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Enter) {
			// GameScene へ遷移
			SceneManager::instance().changeScene<GameScene>();
		}
	}
}

void TitleScene::update(float dt) {
	// タイトルシーンの更新処理
}

void TitleScene::draw(sf::RenderWindow& window) {
	window.draw(titleText);
	window.draw(pressKeyText);
}