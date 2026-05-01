#include<SFML/Graphics.hpp>
#include "Camera.h"
#include "Player.h"

	Camera::Camera()
	{
		view.reset(sf::FloatRect(0, 0, 1600, 900));
		offset = sf::Vector2f(300, -100); // プレイヤーの少し上にカメラを配置
	}

	void Camera::follow(const sf::Vector2f& playerPos,const sf::Vector2u& windowSize, float mapW, float mapH)
	{
		sf::Vector2f camPos = playerPos + offset;

		// カメラの半分のサイズ
		float halfW = windowSize.x / 2.0f;
		float halfH = windowSize.y / 2.0f;

		// カメラをマップ内に制限
		// マップが画面より小さい場合は clamp を使わない
		if (mapW < windowSize.x) {
			camPos.x = mapW / 2.0f;
		}
		else {
			camPos.x = std::clamp(camPos.x, halfW, mapW - halfW);
		}

		if (mapH < windowSize.y) {
			camPos.y = mapH / 2.0f;
		}
		else {
			camPos.y = std::clamp(camPos.y, halfH, mapH - halfH);
		}
		view.setCenter(camPos);
	}

	void Camera::apply(sf::RenderWindow& window)
	{
		window.setView(view);
	}
