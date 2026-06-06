#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class UIManager; // 前方宣言

class Button {
public:
    Button() = default;

    void init(UIManager& ui,
        const std::string& normalName,
        const std::string& hoverName,
        const std::string& text);

    void setPosition(float x, float y);
    void draw(sf::RenderWindow& window);

    bool isMouseOver(const sf::RenderWindow& window) const;
    bool isClicked(const sf::Event& event, const sf::RenderWindow& window) const;

private:
    sf::Sprite sprite;
    sf::Text label;
    sf::Font font;

    sf::IntRect normalRect;
    sf::IntRect hoverRect;
};
