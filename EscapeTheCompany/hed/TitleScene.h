#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "Player.h"

class TitleScene : public Scene {
public:
    TitleScene(sf::RenderWindow* window, Player* player);

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
};