#include<SFML/Graphics.hpp>
#include "Camera.h"


	Camera::Camera()
	{
		view.reset(sf::FloatRect(0, 0, 800, 600));
		offset = sf::Vector2f(0, -100); // プレイヤーの少し上にカメラを配置
	}

	void Camera::follow(const sf::Vector2f& playerWorldPos)
	{
		view.setCenter(playerWorldPos + offset);
	}

	void Camera::apply(sf::RenderWindow& window)
	{
		window.setView(view);
	}
