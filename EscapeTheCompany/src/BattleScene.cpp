#include <SFML/Graphics.hpp>
#include "GameScene.h"
#include "SceneManager.h"
#include "BattleScene.h"

 BattleScene::BattleScene(const sf::RectangleShape& enemyShape, sf::RenderWindow* window) {
	printf("BattleScene ctor start\n");
	//enemy見た目
	enemySprite = enemyShape; //見た目コピー
	enemySprite.setPosition(1200, 300); //敵の位置設定

	//背景
	background.setSize(sf::Vector2f(1600, 900));
	background.setFillColor(sf::Color(20, 20, 20));

	//Hpバー
	playerHpBar.setSize(sf::Vector2f(200, 20));
	playerHpBar.setFillColor(sf::Color::Green);
	playerHpBar.setPosition(50, 50);

	enemyHpBar.setSize(sf::Vector2f(200, 20));
	enemyHpBar.setFillColor(sf::Color::Green);
	enemyHpBar.setPosition(1200, 50);

	// RenderWindowの参照を保持
	windowRef = window;
	printf("windowRef: %p\n", windowRef);
	//Cmd
	font.loadFromFile("Fonts\\KH-Dot-Dougenzaka-12.ttf");
	printf("font load = %p\n", &font);
	const char* commandStrings[3] = { "Attack", "Defend", "Run" };

	for (int i = 0; i < 3; i++) {
		commands[i].setFont(font);
		commands[i].setString(commandStrings[i]);
		commands[i].setCharacterSize(24);
		commands[i].setFillColor(sf::Color::White);
		commands[i].setPosition(100, 700 + i * 40);
	}
	printf("BattleScene ctor end\n");
}

void BattleScene::onEnter() {
	// バトルシーンに入ったときの処理
}

void BattleScene::onExit() {
	// バトルシーンを抜けるときの処理
}

void BattleScene::handleEvent(const sf::Event& event) {

	if (state != BattleState::Playerturn) return;
	// マウス位置を取得
	sf::Vector2i mousePos = sf::Mouse::getPosition(*windowRef);
	// windowRef は BattleScene に渡す RenderWindow の参照（後で説明）

	// --- マウスホバーで選択更新 ---
	for (int i = 0; i < 3; i++) {
		if (commands[i].getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
			selectedIndex = i;
		}
	}

	// --- キーボード操作 ---
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Up) {
			selectedIndex = (selectedIndex + 2) % 3;
		}
		if (event.key.code == sf::Keyboard::Down) {
			selectedIndex = (selectedIndex + 1) % 3;
		}
		if (event.key.code == sf::Keyboard::Enter) {
			executeCommand(selectedIndex);
		}
	}

	// --- マウスクリックで実行 ---
	if (event.type == sf::Event::MouseButtonPressed &&
		event.mouseButton.button == sf::Mouse::Left) {

		executeCommand(selectedIndex);
	}
}
void BattleScene::executeCommand(int index) {
	if (index == 0) { // Attack
		enemyHp -= 10;
		if (enemyHp <= 0)
			SceneManager::instance().changeScene<GameScene>(windowRef);
		// 敵のターンに移行
		state = BattleState::Enemyturn;
	}
	else if (index == 2) { // Run
		SceneManager::instance().changeScene<GameScene>(windowRef);
	}
}
void BattleScene::update(float dt) {
	if (state == BattleState::Enemyturn) {
		// 敵のターン処理（仮）
		PlayerHp -= 8;
		if (PlayerHp <= 0)
			state = BattleState::Lose;
		else {
			state = BattleState::Playerturn;
		}
	}

	if (state == BattleState::Win)
	{
		SceneManager::instance().changeScene<GameScene>(windowRef);
		// 勝利処理（仮）
	}

	if (state == BattleState::Lose) {
		// ゲームオーバー処理（仮）
		SceneManager::instance().changeScene<GameScene>(windowRef);
	}

	
}

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