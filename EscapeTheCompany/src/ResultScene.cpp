#include <SFML/Graphics.hpp>
#include "GameScene.h"
#include "GameScene2.h"
#include "ResultScene.h"
#include "SaveData.h"

ResultScene::ResultScene(sf::RenderWindow* window, Player* player, Enemy* enemy,const std::vector<int>& drops, bool returnedFromBattle)
	: windowRef(window), player(player),enemyRef(enemy),drops(drops), statusManager(player->statusManager) {}


void ResultScene::onEnter(){
	auto* st = player->statusManager;

	int id = enemyRef->data.id;
	auto it = player->statusManager->unlockMap.find(id);
	if (it != player->statusManager->unlockMap.end()) {
		SpringType unlocked = it->second;
		player->statusManager->springunlocked[(int)unlocked] = true;
	}

	player->statusManager->addExp(enemyRef->expValue);
	if (enemyRef->isBoss()) {
		player->statusManager->springunlocked[(int)SpringType::Fire] = true;
	}

	hpUp = st->lasthpUp;
	attackUp = st->lastattackUp;
	defenceUp = st->lastdefenceUp;
	speedUp = st->lastspeedUp;

	//経験値
	expTarget = (float)st->Exp / (float)st->nextExp;

	//レベルアップ確認
	levelUp = (hpUp > 0 || attackUp > 0 || defenceUp > 0 || speedUp > 0);

	
	//セーブ
	SaveData save;
	save.saveGame(player);
}

void ResultScene::update(float dt) {
	expAnim += (expTarget - expAnim) * dt * 5.0f;
}

void ResultScene::onExit() {
	// 結果シーンを抜けるときの処理

	// 3. 敵削除
	//enemyManager->removeEnemy(enemyRef->data.id);
	
}

void ResultScene::handleEvent(const sf::Event& event){
	auto& sm = SceneManager::instance();
	
	if (event.type == sf::Event::KeyPressed &&
		event.key.code == sf::Keyboard::Enter) {

		int stage = SceneManager::instance().lastStage;

		if (stage == 1)
			sm.requestScene(NextSceneType::GameScene, true);
		else if (stage == 2)
			sm.requestScene(NextSceneType::GameScene2, true);
	}

}

void ResultScene::draw(sf::RenderWindow& window) {
	sf::Text t;
	font.loadFromFile("Fonts/KH-Dot-Kagurazaka-16.ttf");
	t.setFont(font);
	t.setFillColor(sf::Color::White);
	//レベル上昇したら描画
	if (levelUp) {
		t.setString("Level Up !");
		t.setPosition(100, 100);
		window.draw(t);

		t.setString("Exp +" + std::to_string(enemyRef->expValue));
		t.setPosition(300, 100);
		window.draw(t);
	}
	else {
		t.setString("Exp +" + std::to_string(enemyRef->expValue));
		t.setPosition(100, 100);
		window.draw(t);
	}

	t.setString("Level " + std::to_string(player->statusManager->level));
	t.setPosition(100, 150);
	window.draw(t);

	t.setString("HP +" + std::to_string(hpUp));
	t.setPosition(100, 200);
	window.draw(t);

	t.setString("Attack +" + std::to_string(attackUp));
	t.setPosition(100, 250);
	window.draw(t);

	t.setString("Defence +" + std::to_string(defenceUp));
	t.setPosition(100, 300);
	window.draw(t);

	t.setString("Speed +" + std::to_string(speedUp));
	t.setPosition(100, 350);
	window.draw(t);

	t.setString("Press Enter");
	t.setPosition(500, 800);
	window.draw(t);
		
	//経験値バー
	sf::RectangleShape back({ 300,20 });
	back.setFillColor(sf::Color(80, 80, 80));
	back.setPosition(100, 400);
	window.draw(back);

	sf::RectangleShape front({ 300 * expAnim, 20 });
	front.setFillColor(sf::Color::Blue);
	front.setPosition(100, 400);
	window.draw(front);

	//取得したアイテム
	float y = 450;

	for (int id : drops) {
		const ItemData* data = SceneManager::instance().itemManager.getItemData(id);
		if (!data) continue;

		sf::Text t;
		t.setFont(font);
		t.setFillColor(sf::Color::White);
		t.setString(data->name + "x 1");
		t.setPosition(100, y);
		window.draw(t);
		y += 40;
		
	}
}