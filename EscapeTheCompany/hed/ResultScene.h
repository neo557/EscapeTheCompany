#pragma once
#include <SFML/Graphics.hpp>
#include "PlayerStatusManager.h"
#include "Player.h"
#include "SceneManager.h"
#include "EnemyManager.h"

class ResultScene : public Scene {
public:
	ResultScene(sf::RenderWindow* window, Player* player, Enemy* enemy, EnemyManager* mgr, bool returnedFromBattle);
	void onEnter();
	void handleEvent(const sf::Event& event);
	void update(float dt);
	void onExit();
	void draw(sf::RenderWindow& window);

	int hpUp = 0;
	int attackUp = 0;
	int defenceUp = 0;
	int speedUp = 0;

	float expAnim = 0.0f;
	float expTarget = 0.0f;
	bool levelUp = false;


private:
	Player* player; // プレイヤーの情報
	PlayerStatusManager* statusManager; // プレイヤーのステータス情報
	EnemyManager* enemyManager;
	Enemy* enemyRef;    // 敵の情報
	sf::Font font;
	sf::Text resultText;
	sf::RenderWindow* windowRef; // RenderWindowの参照
};