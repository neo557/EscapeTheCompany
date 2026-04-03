#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Camera.h"
#include "TileMap.h"

class GameWindow
{
public:
    sf::RenderWindow window;
    Player player;
    Camera camera;
    TileMap* tilemap;
    
	GameWindow();
	void run();
	void handleEvents();
	void update(float dt);
	void draw();
};