#include "Button.h"
#include "UIManager.h"

void Button::init(UIManager& ui,
    const std::string& normalName,
    const std::string& hoverName,
    const std::string& text)
{
    // フォント読み込み
    font.loadFromFile("Fonts/KH-Dot-Kagurazaka-16.ttf");

    // スプライト設定
    sprite.setTexture(ui.atlas);
    normalRect = ui.getRect(normalName);
    hoverRect = ui.getRect(hoverName);
    sprite.setTextureRect(normalRect);

    // ラベル設定
    label.setFont(font);
    label.setString(text);
    label.setCharacterSize(24);
    label.setFillColor(sf::Color::Black);
}

void Button::setPosition(float x, float y) {
    sprite.setPosition(x, y);

    // テキストをボタン中央に配置
    sf::FloatRect bounds = label.getLocalBounds();
    label.setOrigin(bounds.width / 2, bounds.height / 2);
    label.setPosition(x + normalRect.width / 2, y + normalRect.height / 2);
}

void Button::draw(sf::RenderWindow& window) {
    if (isMouseOver(window))
        sprite.setTextureRect(hoverRect);
    else
        sprite.setTextureRect(normalRect);

    window.draw(sprite);
    window.draw(label);
}

bool Button::isMouseOver(const sf::RenderWindow& window) const {
    auto mouse = sf::Mouse::getPosition(window);
    return sprite.getGlobalBounds().contains(mouse.x, mouse.y);
}

bool Button::isClicked(const sf::Event& event, const sf::RenderWindow& window) const {
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {

        auto mouse = sf::Mouse::getPosition(window);
        return sprite.getGlobalBounds().contains(mouse.x, mouse.y);
    }
    return false;
}
