#pragma once
#include <SFML/Graphics.hpp>
#include "Tile.h"

class TileMap {
    sf::Texture tileset;
public:
    static const int WIDTH = 26;
    static const int HEIGHT = 20;
    static const int TILE_SIZE = 32;
    static const int LAYERS = 3; // 0=BG, 1=Ground, 2=Object
    int tiles[LAYERS][HEIGHT][WIDTH];

	TileMap();
    bool isSolidAt(float wx, float wy);
	bool isSolidTile(int tx, int ty) const;
	void draw(sf::RenderWindow& window, const sf::View& view);
    void drawLayer(sf::RenderWindow& window, const sf::View& view, int layer);
    void loadCSV(const std::string& filename, int layer);
};