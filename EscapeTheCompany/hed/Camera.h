#pragma once
#include <SFML/Graphics.hpp>
#include "SpringType.h"
#include "TileMap.h"

class Camera
{
public:
	sf::View view;
	sf::Vector2f offset; // プレイヤーからのオフセット


	Camera();
	void follow(const sf::Vector2f& playerPos,const sf::Vector2u& windowPos, float mapW, float mapH);
	void apply(sf::RenderWindow& window);
};
