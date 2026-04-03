#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "TileMap.h"

    TileMap:: TileMap() {
        // 最下段を地面にする
        for (int x = 0; x < WIDTH; x++) {
            tiles[HEIGHT - 1][x].type = 1;
            tiles[HEIGHT - 1][x].solid = true;
        }
    }

    bool TileMap::isSolidAt(float wx, float wy) {
        int tx = wx / TILE_SIZE;
        int ty = wy / TILE_SIZE;

        if (tx < 0 || tx >= WIDTH || ty < 0 || ty >= HEIGHT)
            return false;

        return tiles[ty][tx].solid;
    }

    void TileMap::draw(sf::RenderWindow& window, const sf::View& view) {
        int halfTiles = (view.getSize().x / TILE_SIZE) / 2;

        int startX = view.getCenter().x / TILE_SIZE - halfTiles;
        int endX = view.getCenter().x / TILE_SIZE + halfTiles;

        for (int y = 0; y < HEIGHT; y++) {
            for (int x = startX; x < endX; x++) {
                if (x < 0 || x >= WIDTH) continue;

                Tile& t = tiles[y][x];
                if (t.type == 0) continue;

                sf::RectangleShape rect(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                rect.setFillColor(sf::Color::Blue);
                rect.setPosition(x * TILE_SIZE, y * TILE_SIZE);
                window.draw(rect);
            }
        }
    }