
#include <SFML/Graphics.hpp>
#include "GameScene.h"
#include "GameScene2.h"
#include "ResultScene.h"
#include "SceneManager.h"
#include "BattleScene.h"
#include "Player.h"
#include "SaveData.h"
#include "EndRollScene.h"
#include "ItemScene.h"

BattleScene::BattleScene(Player* player, Enemy* enemy, sf::RenderWindow* window, const std::vector<SpringType>& allowedSprings,ItemManager* itemMng, EnemyManager* enemyMng)
: player(player), enemyRef(enemy), allowedSprings(allowedSprings),itemManager(itemMng),enemyManager(enemyMng),windowRef(window) {
	printf("BattleScene ctor start\n");
	logfont.loadFromFile("Fonts/KH-Dot-Kagurazaka-16.ttf");
	//Player見た目
	playerSprite = player -> statusManager->battleSprite; //見た目コピー
	playerSprite.setPosition(100, 300); //プレイヤーの位置設定


	//enemy見た目
	enemySprite = enemy -> sprite; //見た目コピー
	enemySprite.setPosition(1200, 300); //敵の位置設定

	//背景
	background.setSize(sf::Vector2f(1600, 900));
	background.setFillColor(sf::Color(20, 20, 20));

	//Hpバー
	hpBack.setSize(sf::Vector2f(200, 20));
	hpBack.setFillColor(sf::Color(100, 100, 100));
	hpBack.setPosition(50, 50);

	hpFront.setSize(sf::Vector2f(200, 20));
	hpFront.setFillColor(sf::Color::Green);
	hpFront.setPosition(50, 50);

	//EnemyHpバー
	enemyhpBack.setSize(sf::Vector2f(200, 20));
	enemyhpBack.setFillColor(sf::Color(100, 100, 100));
	enemyhpBack.setPosition(1200, 50);

	enemyHpBar.setSize(sf::Vector2f(200, 20));
	enemyHpBar.setFillColor(sf::Color::Green);
	enemyHpBar.setPosition(1200, 50);

	// RenderWindowの参照を保持
	windowRef = window;
	printf("windowRef: %p\n", windowRef);
	//Cmd
	font.loadFromFile("Fonts/KH-Dot-Dougenzaka-16.ttf");
	printf("font load = %p\n", &font);
	const char* commandStrings[3] = { "Attack", "Item", "Run" };

	for (int i = 0; i < 3; i++) {
		commands[i].setFont(font);
		commands[i].setString(commandStrings[i]);
		commands[i].setCharacterSize(24);
		commands[i].setFillColor(sf::Color::White);
		commands[i].setPosition(100, 700 + i * 40);
	}

	if(enemy->speed < player->statusManager->speed) {
		state = BattleState::Enemyturn;
		enemyActionPending = true;
	} else {
		state = BattleState::Playerturn;
		enemyActionPending = false;
	}

	selectedIndex = 0;
	printf("BattleScene ctor end\n");

}

void BattleScene::onEnter() {
	// バトルシーンに入ったときの処理
	printf("Enemy HP=%d maxHp=%d ATK=%d DEF=%d spring=%d\n",
		enemyRef->hp,
		enemyRef->data.maxHp,
		enemyRef->attack,
		enemyRef->defence,
		(int)enemyRef->springType
	);
	printf("Player level = %d\n", player->statusManager->level);
	printf("Player attack = %d\n", player->statusManager->attack);
	printf("Player defence = %d\n", player->statusManager->defence);

	printf("enemyRef->defence = %d, enemyRef->data.defence = %d\n",
		enemyRef->defence, enemyRef->data.defence);
	printf("playerRef->defence = %d, playerRef->data.defence = %d\n",
		player->statusManager->attack, player->statusManager->defence);

}

void BattleScene::onExit() {
	// バトルシーンを抜けるときの処理
	player->justReturnedFromBattle = true;
	//キー入力をクリア
	auto& sm = SceneManager::instance();
	sm.resetKeyState();
}

void BattleScene::handleEvent(const sf::Event& event) {
	auto& sm = SceneManager::instance();
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

	// --- マウスクリックで実行 ---
	if (event.type == sf::Event::MouseButtonPressed &&
		event.mouseButton.button == sf::Mouse::Left) {

		executeCommand(selectedIndex);
	}

	player->statusManager->onHandle(event, allowedSprings);
}

void BattleScene::executeCommand(int index) {

	if (index == 0) { // Attack
		playerAttack();

	}else if(index == 1){//item
		SceneManager::instance().pushScene<ItemScene>(windowRef, player, itemManager);
	}
	else if (index == 2) { // Run
		tryRun();
	}
}


void BattleScene::playerAttack() {

	int dmg = player->statusManager->calcDamage(*enemyRef);
	enemyRef->hp -= dmg;

	addLog(L"Enemyに " + std::to_wstring(dmg) + L" ダメージ与えた!");
	printf("Enemy springType = %d\n", (int)enemyRef->springType);
	printf("Player springType = %d\n", (int)player->statusManager->currentSpring);
	printf("enemyRef pointer = %p\n", enemyRef);
	printf("enemyRef->hp = %d\n", enemyRef->hp);
	printf("enemyRef->defence = %d\n", enemyRef->defence);

	if (enemyRef->hp <= 0) {
		addLog(L"Enemyを倒した!");
		state = BattleState::Win;
	}
	else {
		state = BattleState::Enemyturn;
	}
	
}

void BattleScene::enemyAttack() {
	int dmg = enemyRef->calcDamage(*player->statusManager);
	player->statusManager->hp -= dmg;
	addLog(L"Enemyの攻撃! " + std::to_wstring(dmg) + L" ダメージ受けた!");
	printf("atack damage: %d\n", dmg);
	if (player->statusManager->hp <= 0) {
		state = BattleState::Lose;
	}
	else {
		state = BattleState::Playerturn;
	}
	printf("player HP after attack: %d\n", player->statusManager->hp);
}

void BattleScene::tryRun() {
	// 逃走成功率は50%（仮）
	if (rand() % 100 < 50) {
		state = BattleState::Win; // 逃走成功は勝利扱い
	}
	else {
		state = BattleState::Enemyturn; // 失敗は敵のターン
	}
}

void BattleScene::addLog(const std::wstring& msg) {
	BattleLog log;
	log.logText.setFont(logfont);
	log.logText.setString(msg);
	log.logText.setCharacterSize(24);
	log.logText.setFillColor(sf::Color::White);
	log.displayTime = 3.0f; // 3秒表示
	battleLogs.push_back(log);
}

void BattleScene::update(float dt) {
	auto& sm = SceneManager::instance();
	float ratio = player->statusManager->getHpRatio();
	hpFront.setSize(sf::Vector2f(200 * ratio, 20));

	if (state == BattleState::Enemyturn) {

		if (enemyActionPending) {
			enemyActionPending = false;
		}

		enemyAttack();
	}

	// 勝利時
	if (state == BattleState::Win) {

		if (drops.empty()) {
			// 1. ドロップ判定
			drops = enemyManager->rollDrops(enemyRef);

			// 2. アイテム付与
			for (int id : drops) {
				itemManager->addItem(id, 1);
			}

			// 4. リザルトへ
			sm.resultEnemyRef = enemyRef;     
			sm.resultEnemyRef = enemyRef;
			sm.resultDrops = drops;
			sm.nextReturnedFromBattle = true;
			sm.requestScene(NextSceneType::ResultScene, true);

			// ボスならエンドロール（本当は ResultScene 終了後が綺麗）
			if (enemyRef->data.id == 3) {
				sm.requestScene(NextSceneType::EndRollScene, false);
			}
		}
	}

	if (state == BattleState::Lose) {
		SaveData save;
		save.saveGame(player);
		switch (SceneManager::instance().lastStage) {
		case 1: sm.requestScene(NextSceneType::GameScene, true); break;
		case 2: sm.requestScene(NextSceneType::GameScene2, true); break;
		//case 3: SceneManager::instance().changeScene<GameScene3>(); break;
		}
	}

	// ログ更新
	float y = 600.0f; // 画面下寄りから
	for (auto& log : battleLogs) {
		log.displayTime -= dt;
		log.logText.setPosition(100.0f, y);
		y -= 30.0f; // 1行ずつ上に積む
	}

	battleLogs.erase(
		std::remove_if(battleLogs.begin(), battleLogs.end(),
			[](const BattleLog& l) { return l.displayTime <= 0.0f; }),
		battleLogs.end()
	);

	SpringType cur = player->statusManager->currentSpring;

	if (std::find(allowedSprings.begin(), allowedSprings.end(), cur)
		== allowedSprings.end())
	{
		springText.setString("Not Allowed");
	}
	else
	{
		switch (cur) {
		case SpringType::None: springText.setString("None"); break;
		case SpringType::Normal: springText.setString("Normal Spring"); break;
		case SpringType::Fire: springText.setString("Fire Spring"); break;
		case SpringType::Ice: springText.setString("Ice Spring"); break;
		case SpringType::Electric: springText.setString("Electric Spring"); break;
		}
	}
	springText.setFont(font);
	springText.setCharacterSize(24);
	springText.setFillColor(sf::Color::White);
	springText.setPosition(30,80); // 左上

	//キー入力false
	if (state == BattleState::Playerturn) {
		if (sm.isPressedOnce(sf::Keyboard::Enter)) {
			executeCommand(selectedIndex);
		}
		if (sm.isPressedOnce(sf::Keyboard::Up)) {
			selectedIndex = (selectedIndex + 2) % 3;
		}
		if (sm.isPressedOnce(sf::Keyboard::Down)) {
			selectedIndex = (selectedIndex + 1) % 3;
		}
	}
}

void BattleScene::draw(sf::RenderWindow& window) {
	window.setView(window.getDefaultView()); // ビューをリセットして固定描画
	float ratio = enemyRef->hp / (float)enemyRef->data.maxHp;
	ratio = std::max(0.0f, ratio); // 下限だけ clamp

	enemyHpBar.setSize(sf::Vector2f(200 * ratio, 20));

	// Player
	float playerRatio = player->statusManager->getHpRatio();
	hpFront.setSize(sf::Vector2f(200 * playerRatio, 20));

	window.draw(background);
	window.draw(playerSprite);
	window.draw(enemyhpBack);
	window.draw(enemyHpBar);
	window.draw(enemySprite);
	window.draw(hpBack);
	window.draw(hpFront);
	window.draw(springText);
	//ログ描写	
	for (auto& log : battleLogs) {
		window.draw(log.logText);
	}
	// コマンド描画（選択中は黄色）
	for (int i = 0; i < 3; i++) {
		if (i == selectedIndex)
			commands[i].setFillColor(sf::Color::Yellow);
		else
			commands[i].setFillColor(sf::Color::White);

		window.draw(commands[i]);
	}
}