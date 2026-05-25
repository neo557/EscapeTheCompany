#include <SFML/Graphics.hpp>
#include "GameScene.h"
#include "GameScene2.h"
#include "ResultScene.h"
#include "SaveData.h"

ResultScene::ResultScene(sf::RenderWindow* window, Player* player, Enemy* enemy, EnemyManager* mgr, bool returnedFromBattle)
	: windowRef(window), player(player),enemyRef(enemy),enemyManager(mgr), statusManager(player->statusManager) {}


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

	//敵削除
	enemyManager->removeEnemy(enemyRef->data.id);
	//セーブ
	SaveData save;
	save.saveGame(player);
}

void ResultScene::update(float dt) {
	expAnim += (expTarget - expAnim) * dt * 5.0f;
}

void ResultScene::handleEvent(const sf::Event& event){

	
	if (event.type == sf::Event::KeyPressed &&
		event.key.code == sf::Keyboard::Enter) {

		int stage = SceneManager::instance().lastStage;

		if (stage == 1)
			SceneManager::instance().changeScene<GameScene>(windowRef, player, enemyManager, true);
		else if (stage == 2)
			SceneManager::instance().changeScene<GameScene2>(windowRef, player, enemyManager, true);
	}

}

void ResultScene::draw(sf::RenderWindow& window) {
	sf::Text t;
	font.loadFromFile("Fonts\\KH-Dot-Kagurazaka-16.ttf");
	t.setFont(font);
	t.setFillColor(sf::Color::White);

	t.setString("Level Up !");
	t.setPosition(100, 100);
	window.draw(t);

	t.setString("Exp +" + std::to_string(enemyRef->expValue));
	t.setPosition(300, 100);
	window.draw(t);

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
}