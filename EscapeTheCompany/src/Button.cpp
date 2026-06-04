#include "Button.h"

Button::Button(const std::string& text, const sf::Vector2f& pos, const sf::Vector2f size) {
	font.loadFromFile("Fonts\\KH-Dot-Kagurazaka-16.ttf");
	shape.setSize(size);
	shape.setFillColor(normalColor);
	shape.setPosition(pos);
	label.setFont(font);
	label.setString(text);
	label.setCharacterSize(24);
	label.setFillColor(sf::Color::White);
	// テキストを中央に配置
	sf::FloatRect textRect = label.getLocalBounds();
	label.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	label.setPosition(pos.x + size.x / 2.0f, pos.y + size.y / 2.0f);
}

void Button::draw(sf::RenderWindow& window) {
	if (isMouseOver(window)) {
		shape.setFillColor(hoverColor);
	}
	else {
		shape.setFillColor(normalColor);
	}
	window.draw(shape);
	window.draw(label);
}

bool Button::isMouseOver(const sf::RenderWindow& window) const {
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	sf::FloatRect buttonBounds = shape.getGlobalBounds();
	return buttonBounds.contains(static_cast<sf::Vector2f>(mousePos));
}

bool Button::isClicked(const sf::RenderWindow& window, const sf::Event& event) const {
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		return isMouseOver(window);
	}
	return false;
}