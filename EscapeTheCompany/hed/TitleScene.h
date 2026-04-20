#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"

class TitleScene : public Scene {
public:
    TitleScene();

    void onEnter() override;
    void onExit() override;
    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

private:
    sf::Font font;
    sf::Text titleText;
    sf::Text pressKeyText;
};