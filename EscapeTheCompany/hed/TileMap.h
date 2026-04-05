#pragma once
#include <SFML/Graphics.hpp>
#include "Tile.h"

class TileMap {
public:
    static const int WIDTH = 200;
    static const int HEIGHT = 20;
    static const int TILE_SIZE = 100;
    Tile tiles[HEIGHT][WIDTH];

	TileMap();
    bool isSolidAt(float wx, float wy);
	void draw(sf::RenderWindow& window, const sf::View& view);
};