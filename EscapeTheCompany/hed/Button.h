#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button {
public :
	Button(const std::string& text, const sf::Vector2f& pos, const sf::Vector2f size);

	void draw(sf::RenderWindow& window);
	bool isMouseOver(const sf::RenderWindow& window) const;
	bool isClicked(const sf::RenderWindow& window, const sf::Event& event) const;

private:
	sf::RectangleShape shape;
	sf::Text label;
	sf::Font font;

	sf::Color normalColor = sf::Color(70, 70, 70);
	sf::Color hoverColor = sf::Color(100, 100, 100);
};