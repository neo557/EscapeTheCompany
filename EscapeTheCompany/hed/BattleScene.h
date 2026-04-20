#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"

class BattleScene : public Scene {
public:
    BattleScene();

    void onEnter() override;
    void onExit() override;
    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

private: 
    //背景
    sf::RectangleShape background;

	//Hpバー
    sf::RectangleShape playerHpBar;
	sf::RectangleShape enemyHpBar;

    //コマンドウィンドウ
	sf::Font font;
    sf::Text commands[3];
    int selectedIndex = 0;
};