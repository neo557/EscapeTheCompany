#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "TileMap.h"
#include "Player.h"
#include "Camera.h"

class GameScene : public Scene {
public:
	GameScene(sf::RenderWindow* window);

	void onEnter() override;
	void onExit() override;
	void handleEvent(const sf::Event& event) override;
	void update(float dt) override;
	void draw(sf::RenderWindow& window) override;

private:
	Player player;
	Camera camera;
	TileMap tilemap;

	sf::RectangleShape enemySymbol; // 敵のシンボル
	sf::RenderWindow* windowRef; // RenderWindowの参照
};