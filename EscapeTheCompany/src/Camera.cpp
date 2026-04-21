#include<SFML/Graphics.hpp>
#include "Camera.h"
#include "Player.h"


	Camera::Camera()
	{
		view.reset(sf::FloatRect(0, 0, 1600, 900));
		offset = sf::Vector2f(0, -100); // プレイヤーの少し上にカメラを配置
	}

	void Camera::follow(const sf::Vector2f& worldPos)
	{
		view.setCenter(worldPos + offset);
	}

	void Camera::apply(sf::RenderWindow& window)
	{
		window.setView(view);
	}
