#pragma once
#include <SFML/Graphics.hpp>
#include "Tile.h"

class TileMap {
public:
    static const int WIDTH = 500;
    static const int HEIGHT = 20;
    static const int TILE_SIZE = 50;
    int tiles[HEIGHT][WIDTH];

	TileMap();
    bool isSolidAt(float wx, float wy);
	bool isSolidTile(int tx, int ty) const;
	void draw(sf::RenderWindow& window, const sf::View& view);
    void loadCSV(const std::string& filename);
};