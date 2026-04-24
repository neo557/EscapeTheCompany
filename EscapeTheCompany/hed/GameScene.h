#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "TileMap.h"
#include "Player.h"
#include "Camera.h"


class GameScene : public Scene {
public:
	GameScene(sf::RenderWindow* window, Player* player, bool returnedFromBattle = false);

	void onEnter() override;
	void onExit() override;
	void handleEvent(const sf::Event& event) override;
	void update(float dt) override;
	void draw(sf::RenderWindow& window) override;

private:
	Player* playerRef; // プレイヤーの情報
	Camera camera;
	TileMap tilemap;
	sf::Font font;
	sf::Text springText;

	sf::RectangleShape enemySymbol; // 敵のシンボル
	sf::RectangleShape hpBack;
	sf::RectangleShape hpFront;
	sf::RenderWindow* windowRef; // RenderWindowの参照

	bool justReturnedFromBattle = false;
};