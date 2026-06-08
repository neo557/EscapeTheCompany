#include "PlayerStatusScene.h"
#include "Player.h"
#include "GameScene.h"
#include "GameScene2.h"
#include "UIManager.h"

PlayerStatusScene::PlayerStatusScene(sf::RenderWindow* window, Player* player) : windowRef(window), player(player) {
	// コンストラクタ
	statusManager = player->statusManager;

	//ボタン作成
	itemButton = new Button();
	weaponButton = new Button();
	libraryButton = new Button();
	systemButton = new Button();

}

void PlayerStatusScene::onEnter() {

	// プレイヤー画像読み込み
	playerTex.loadFromFile(player->statusManager->spritePath);
	playerSprite.setTexture(playerTex);

	// 中央に配置
	float cx = windowRef->getSize().x / 2.f - playerTex.getSize().x / 2.f;
	float cy = windowRef->getSize().y / 2.f - playerTex.getSize().y / 2.f;
	playerSprite.setPosition(cx, cy);

	// Spring アイコン読み込み
	springTexNone.loadFromFile("UI/Spring_None.png");
	springTexNormal.loadFromFile("UI/Spring_Normal.png");
	springTexFire.loadFromFile("UI/Spring_Fire.png");
	springTexIce.loadFromFile("UI/Spring_Ice.png");
	springTexElectric.loadFromFile("UI/Spring_Electric.png");

	spNone.setTexture(springTexNone);
	spNormal.setTexture(springTexNormal);
	spFire.setTexture(springTexFire);
	spIce.setTexture(springTexIce);
	spElectric.setTexture(springTexElectric);

	// Spring の配置（右カラム下）
	float sx = windowRef->getSize().x - 175.f;
	float sy = 500.f;

	spNone.setPosition(sx, sy);
	spNormal.setPosition(sx, sy - 60);
	spFire.setPosition(sx - 60, sy);
	spIce.setPosition(sx + 60, sy);
	spElectric.setPosition(sx, sy + 60);
	// シーンに入ったときの処理
	player->statusManager->hp;
	player->statusManager->maxHp;
	player->statusManager->attack;
	player->statusManager->defence;
	player->statusManager->speed;
	player->statusManager->level;
	player->statusManager->Exp;

	ui.load("UI/UIMap.png", "UI/UIMap.json");
	// ボタンの初期化
	itemButton->init(ui, "button_rectangle_depth_gloss.png", "button_rectangle_depth_flat.png", "Item");
	weaponButton->init(ui, "button_rectangle_depth_gloss.png", "button_rectangle_depth_flat.png", "Weapon");
	libraryButton->init(ui, "button_rectangle_depth_gloss.png", "button_rectangle_depth_flat.png", "Library");
	systemButton->init(ui, "button_rectangle_depth_gloss.png", "button_rectangle_depth_flat.png", "System");

	itemButton->setPosition(50, 100);
	weaponButton->setPosition(50, 180);
	libraryButton->setPosition(50, 260);
	systemButton->setPosition(50, 340);

}

void PlayerStatusScene::onExit() {
	// シーンから出るときの処理
	
}

void PlayerStatusScene::handleEvent(const sf::Event& event) {
	// イベント処理
	if (event.type == sf::Event::KeyPressed &&
		event.key.code == sf::Keyboard::Escape) {
		player->worldPos = SceneManager::instance().returnPos;
		//Escキーでシーンを抜ける処理など
		switch (SceneManager::instance().lastStage) {
		case 1: SceneManager::instance().changeScene<GameScene>(windowRef, player, &SceneManager::instance().enemyManager, false); break;
		case 2: SceneManager::instance().changeScene<GameScene2>(windowRef, player, &SceneManager::instance().enemyManager, false); break;
			//case 3: SceneManager::instance().changeScene<GameScene3>(); break;
		}
	}

	if (itemButton->isClicked(event, *windowRef)) {
		printf("Item button clicked\n");
	}
	if (weaponButton->isClicked(event, *windowRef)) {
		printf("Weapon button clicked\n");
	}
	if (libraryButton->isClicked(event, *windowRef)) {
		printf("Library button clicked\n");
	}
	if (systemButton->isClicked(event, *windowRef)) {
		printf("System button clicked\n");
	}
}

void PlayerStatusScene::update(float dt) {
	// 更新処理（必要に応じて）
}

void PlayerStatusScene::draw(sf::RenderWindow& window) {

	// ============================
	// 右カラムの背景パネル
	// ============================
	sf::RectangleShape rightPanel;
	rightPanel.setSize({ 350.f, (float)window.getSize().y });
	rightPanel.setPosition(window.getSize().x - 350.f, 0);
	rightPanel.setFillColor(sf::Color(100, 100, 100)); // 灰色
	window.draw(rightPanel);


	// ============================
	// ステータス文字の描画
	// ============================
	sf::Text text;
	font.loadFromFile("Fonts/KH-Dot-Kagurazaka-16.ttf");
	text.setFont(font);
	text.setCharacterSize(28);
	text.setFillColor(sf::Color::White);

	float baseX = window.getSize().x - 320.f;  // 右パネル内の左端
	float baseY = 80.f;

	text.setPosition(baseX, baseY);

	text.setString(
		"LV : " + std::to_string(player->statusManager->level) + "\n" +
		"EXP: " + std::to_string(player->statusManager->Exp) + " / " + std::to_string(player->statusManager->nextExp) + "\n\n" +
		"HP : " + std::to_string(player->statusManager->hp) + " / " + std::to_string(player->statusManager->maxHp) + "\n" +
		"ATK: " + std::to_string(player->statusManager->attack) + "\n" +
		"DEF: " + std::to_string(player->statusManager->defence) + "\n" +
		"SPD: " + std::to_string(player->statusManager->speed)
	);

	// ============================
    // Spring 十字 UI
	// ============================

    // 未発見の Spring は灰色に
	if (!player->statusManager->isDiscovered(SpringType::Normal))
		spNormal.setColor(sf::Color(100, 100, 100, 150));
	if (!player->statusManager->isDiscovered(SpringType::Fire))
		spFire.setColor(sf::Color(100, 100, 100, 150));
	if (!player->statusManager->isDiscovered(SpringType::Ice))
		spIce.setColor(sf::Color(100, 100, 100, 150));
	if (!player->statusManager->isDiscovered(SpringType::Electric))
		spElectric.setColor(sf::Color(100, 100, 100, 150));

	// 描画
	window.draw(spNone);
	window.draw(spNormal);
	window.draw(spFire);
	window.draw(spIce);
	window.draw(spElectric);

	// 現在装備中の Spring を光らせる
	sf::Sprite* current = nullptr;
	switch (player->statusManager->currentSpring) {
	case SpringType::None: current = &spNone; break;
	case SpringType::Normal: current = &spNormal; break;
	case SpringType::Fire: current = &spFire; break;
	case SpringType::Ice: current = &spIce; break;
	case SpringType::Electric: current = &spElectric; break;
	}

	if (current) {
		sf::RectangleShape frame({ 50, 50 });
		frame.setPosition(current->getPosition());
		frame.setFillColor(sf::Color::Transparent);
		frame.setOutlineColor(sf::Color::Yellow);
		frame.setOutlineThickness(3);
		window.draw(frame);
	}
	window.draw(text);

	// ============================
	// Button描画
	// ============================
	itemButton->draw(window);
	weaponButton->draw(window);
	libraryButton->draw(window);
	systemButton->draw(window);
}
