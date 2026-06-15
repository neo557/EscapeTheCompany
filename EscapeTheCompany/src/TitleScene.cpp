#include <SFML/Graphics.hpp>
#include <fstream>
#include "TitleScene.h"
#include "GameScene.h"
#include "GameScene2.h"
#include "SceneManager.h"
#include "SaveData.h"


TitleScene::TitleScene(sf::RenderWindow* window, Player* player, EnemyManager* mgr) {
	windowRef = window; // RenderWindowの参照を保持
	this->player = player; // Playerの参照を保持
	enemyManager = mgr; // EnemyManagerの参照を保持
	std::ifstream f("save.json");
	hasSave = f.good();
	font.loadFromFile("Fonts/KH-Dot-Dougenzaka-16.ttf");

	titleText.setFont(font);
	titleText.setString("Escape The Company");
	titleText.setCharacterSize(64);
	titleText.setFillColor(sf::Color::White);
	titleText.setPosition(100, 150);
	
	printf("font load = %p\n", &font);
	const char* commandStrings[2] = { "New Game", "Continue" };

	for (int i = 0; i < 2; i++) {
		commands[i].setFont(font);
		commands[i].setString(commandStrings[i]);
		commands[i].setCharacterSize(24);
		commands[i].setFillColor(sf::Color::White);
		commands[i].setPosition(100, 700 + i * 40);
	}

	if (!hasSave) {
		commands[1].setFillColor(sf::Color(100, 100, 100)); // セーブデータがない場合はグレーアウト
	}
	printf("[DEBUG] TitleScene: spritePath = %s\n", player->statusManager->spritePath.c_str());
}

void TitleScene::onEnter() {
	// タイトルシーンに入ったときの処理
	auto& sm = SceneManager::instance();
	sm.resetKeyState(); // キーの状態をリセットして、Enterキーの誤検知を防止	
	enterRequested = false;
}

void TitleScene::onExit() {
	// タイトルシーンを抜けるときの処理
}

void TitleScene::handleEvent(const sf::Event& event) {
	auto& sm = SceneManager::instance();
	// マウス位置を取得
	sf::Vector2i mousePos = sf::Mouse::getPosition(*windowRef);

	// --- マウスホバーで選択更新 ---
	for (int i = 0; i < 2; i++) {
		if (commands[i].getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
			selectedIndex = i;
		}
	}

	// --- マウスクリックで実行 ---
	if (event.type == sf::Event::MouseButtonPressed &&
		event.mouseButton.button == sf::Mouse::Left) {

		if (selectedIndex == 0) {
			// New Game
			enterRequested = true;
		}
		else if (selectedIndex == 1 && hasSave) {
			SaveData save;
			save.loadGame(player);

			int stage = SceneManager::instance().lastStage;

			if (stage == 1)
				enterRequested = true;
			else if (stage == 2)
				enterRequested = true;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		selectedIndex = (selectedIndex + 1) % 2;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		selectedIndex = (selectedIndex + 1) % 2;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
		enterRequested = true;
	}
}

void TitleScene::update(float dt) {
	auto& sm = SceneManager::instance();
	if (!enterRequested) return;

	enterRequested = false;

	if (sm.isPressedOnce(sf::Keyboard::Up)) {
		selectedIndex = (selectedIndex + 1) % 2;
	}
	if (sm.isPressedOnce(sf::Keyboard::Down)) {
		selectedIndex = (selectedIndex + 1) % 2;
	}
	if (sm.isPressedOnce(sf::Keyboard::Enter)) {
		enterRequested = true;
	}

	if (enterRequested) {
		enterRequested = false;
		if (selectedIndex == 0) {
			//  ステージ情報を初期化
			sm.st1 = true;
			sm.st2 = false;
			sm.st3 = false;
			sm.lastStage = 1;

			//  プレイヤー座標を初期化
			sm.player->worldPos = { 0, 500 };
			sm.returnPos = { 0, 500 };

			//  敵を初期化
			sm.enemyManager.clear();
			sm.enemyManager.loadEnemyDataFromCSV("CharacterData/CharacterManager.csv");
			sm.enemyManager.spawn(1, { 1200, 500 });
			sm.enemyManager.spawn(2, { 1500, 800 });

			// プレイヤーのステータス初期化
			sm.player->statusManager->loadPlayerDataFromCSV("CharacterData/CharacterManager.csv");
			sm.player->statusManager->spawn(sm.player, 0, { 0, 500 });
			sm.player->init(*sm.player->statusManager);
			sm.requestScene(NextSceneType::GameScene, false);

			printf("[NEWGAME] player pos after spawn: (%f, %f)\n",
				sm.player->worldPos.x, sm.player->worldPos.y);
			for (auto& e : sm.enemyManager.enemies) {
				printf("[NEWGAME] enemy id=%d pos=(%f,%f)\n",
					e->data.id, e->worldPos.x, e->worldPos.y);
			}
		}
		else if (selectedIndex == 1 && hasSave) {
			SaveData save;
			save.loadGame(player);

			int stage = SceneManager::instance().lastStage;

			if (stage == 1)
				sm.requestScene(NextSceneType::GameScene, false);
			else if (stage == 2)
				sm.requestScene(NextSceneType::GameScene2, false);
		}
	}

}

void TitleScene::draw(sf::RenderWindow& window) {
	window.draw(titleText);
	// コマンド描画（選択中は黄色）
	for (int i = 0; i < 2; i++) {
		if (i == selectedIndex)
			commands[i].setFillColor(sf::Color::Yellow);
		else
			commands[i].setFillColor(sf::Color::White);

		window.draw(commands[i]);
	}
}