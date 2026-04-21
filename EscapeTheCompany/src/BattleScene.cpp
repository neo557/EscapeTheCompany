#include <SFML/Graphics.hpp>
#include "GameScene.h"
#include "SceneManager.h"
#include "BattleScene.h"

BattleScene::BattleScene(const sf::RectangleShape& enemyShape) {
	//enemy見た目
	enemySprite = enemyShape; //見た目コピー
	enemySprite.setPosition(600, 300); //敵の位置設定

	//背景
	background.setSize(sf::Vector2f(800, 600));
	background.setFillColor(sf::Color(20, 20, 20));

	//Hpバー
	playerHpBar.setSize(sf::Vector2f(200, 20));
	playerHpBar.setFillColor(sf::Color::Green);
	playerHpBar.setPosition(50, 50);

	enemyHpBar.setSize(sf::Vector2f(200, 20));
	enemyHpBar.setFillColor(sf::Color::Green);
	enemyHpBar.setPosition(550, 50);
	//Cmd
	font.loadFromFile("Fonts\\KH-Dot-Dougenzaka-12.ttf");

	const char* commandStrings[3] = { "Attack", "Defend", "Run" };

	for (int i = 0; i < 3; i++) {
		commands[i].setFont(font);
		commands[i].setString(commandStrings[i]);
		commands[i].setCharacterSize(24);
		commands[i].setFillColor(sf::Color::White);
		commands[i].setPosition(100, 400 + i * 40);
	}
}

void BattleScene::onEnter() {
	// バトルシーンに入ったときの処理
}

void BattleScene::onExit() {
	// バトルシーンを抜けるときの処理
}

void BattleScene::handleEvent(const sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Up) {
			selectedIndex = (selectedIndex + 2) % 3;
		}
		if (event.key.code == sf::Keyboard::Down) {
			selectedIndex = (selectedIndex + 1) % 3;
		}
		if (event.key.code == sf::Keyboard::Enter) {
			if (event.key.code == sf::Keyboard::Enter) {
				if (selectedIndex == 0) { // Attack
					enemyHp -= 10;
					
					if (enemyHp <= 0)
						SceneManager::instance().changeScene<GameScene>();
				}
				if (selectedIndex == 2) { // Run
					SceneManager::instance().changeScene<GameScene>();
				}
			}
		}
	}
}

void BattleScene::update(float dt) {}

void BattleScene::draw(sf::RenderWindow& window) {
	window.setView(window.getDefaultView()); // ビューをリセットして固定描画
	enemyHpBar.setSize(sf::Vector2f(200 * (enemyHp / 100.f), 20));
	playerHpBar.setSize(sf::Vector2f(200 * (PlayerHp / 100.f), 20));
	window.draw(background);
	window.draw(playerHpBar);
	window.draw(enemyHpBar);
	window.draw(enemySprite);

	// コマンド描画（選択中は黄色）
	for (int i = 0; i < 3; i++) {
		if (i == selectedIndex)
			commands[i].setFillColor(sf::Color::Yellow);
		else
			commands[i].setFillColor(sf::Color::White);

		window.draw(commands[i]);
	}
}