#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"

enum class BattleState {
    Playerturn, Enemyturn, Win, Lose
};
class BattleScene : public Scene {
public:
    BattleScene(const sf::RectangleShape& enemyShape, sf::RenderWindow* window); //敵の見た目反映

    void onEnter() override;
    void onExit() override;
    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
	void executeCommand(int index); // コマンド実行関数

private: 
    BattleState state = BattleState::Playerturn;
	// RenderWindowの参照を保持
    sf::RenderWindow* windowRef;
    //背景
    sf::RectangleShape background;

	//Hpバー
    sf::RectangleShape playerHpBar;
	sf::RectangleShape enemyHpBar;

	//enemyの見た目
	sf::RectangleShape enemySprite;//描画する敵の情報
	int enemyHp = 100; // 敵のHP(仮)
	int PlayerHp = 100; // プレイヤーのHP(仮)

    //コマンドウィンドウ
	sf::Font font;
    sf::Text commands[3];
    int selectedIndex = 0;
};