#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "Player.h"
#include "EnemyManager.h"

class TitleScene : public Scene {
public:
    TitleScene(sf::RenderWindow* window, Player* player, EnemyManager* mgr);

    void onEnter() override;
    void onExit() override;
    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

private:
	Player* player; // プレイヤーの情報
    sf::Font font;
    sf::Text titleText;
    sf::Text pressKeyText;
	sf::Text commands[2]; // コマンドテキスト（New Game, Continue）
    bool hasSave = false; // セーブデータの有無を示すフラグ
	int selectedIndex = 1; // 選択中のメニューインデックス
	sf::RenderWindow* windowRef; // RenderWindowの参照
	EnemyManager* enemyManager; // EnemyManager の参照
};