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
	Player* playerRef; // プレイヤーの情報
    sf::Font font;
    sf::Text titleText;
    sf::Text pressKeyText;
	sf::RenderWindow* windowRef; // RenderWindowの参照
	EnemyManager* enemyManager; // EnemyManager の参照
};