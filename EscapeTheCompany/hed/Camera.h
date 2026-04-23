#pragma once
#include <SFML/Graphics.hpp>
#include "SpringType.h"

class Camera
{
public:
	sf::View view;
	sf::Vector2f offset; // プレイヤーからのオフセット


	Camera();
	void follow(const sf::Vector2f& playerWorldPos);
	void apply(sf::RenderWindow& window);
};
