#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "TileMap.h"

TileMap::TileMap() {
    tileset.loadFromFile("TileMap\\map_town001a.png");

    if (!tileset.loadFromFile("TileMap\\map_town001a.png")) {
        printf("FAILED TO LOAD TILESET\n");
    }
    else {
        printf("LOADED TILESET OK: %d x %d\n",
            tileset.getSize().x, tileset.getSize().y);
    }
    /**/
    for (int l = 0; l < LAYERS; l++) {
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                tiles[l][y][x] = 0;
            }
        }
    }
}

void TileMap::loadCSV(const std::string& filename, int layer) {
    std::ifstream file(filename);

  
    if (!file.is_open()) return;

    std::string line;
    int y = 0;

    while (std::getline(file, line) && y < HEIGHT) {
        std::stringstream ss(line);
        std::string cell;
        

        
        int x = 0;
        
        while (std::getline(ss, cell, ',') && x < WIDTH) {

            printf("cell='%s'\n", cell.c_str());  // ← ここに置く

            // 空セル or 空白セルなら 0
            if (cell.find_first_not_of(" \t\r\n") == std::string::npos) {
                tiles[layer][y][x] = 0;
                x++;
                continue;
            }

            try {
                tiles[layer][y][x] = std::stoi(cell);
            }
            catch (...) {
                printf("ERROR: stoi failed for cell='%s'\n", cell.c_str());
                tiles[layer][y][x] = 0; // 安全にフォールバック
            }

            x++;
        }
        
        y++;
    }
}
void TileMap::draw(sf::RenderWindow& window, const sf::View& view) {

    // 1. Background（奥）
    drawLayer(window, view, 0);

    // 2. Ground（衝突判定あり）
    drawLayer(window, view, 1);

    // 3. Object（手前）
    drawLayer(window, view, 2);

}
void TileMap::drawLayer(sf::RenderWindow& window, const sf::View& view, int layer) {
    float left = view.getCenter().x - view.getSize().x / 2;
    float right = view.getCenter().x + view.getSize().x / 2;

    int startX = std::floor(left / TILE_SIZE);
    int endX = std::ceil(right / TILE_SIZE);

    if (startX < 0) startX = 0;
    if (endX > WIDTH) endX = WIDTH;

    int tilesPerRow = tileset.getSize().x / TILE_SIZE;

    sf::Sprite sprite;
    sprite.setTexture(tileset);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = startX; x < endX; x++) {

            int id = tiles[layer][y][x];
            if (id == 0) continue;

            int tileX = (id % tilesPerRow) * TILE_SIZE;
            int tileY = (id / tilesPerRow) * TILE_SIZE;

            sprite.setTextureRect(sf::IntRect(tileX, tileY, TILE_SIZE, TILE_SIZE));
            sprite.setPosition(x * TILE_SIZE, y * TILE_SIZE);

            window.draw(sprite);
        }
    }
}




bool TileMap::isSolidAt(float wx, float wy) {
    int tx = wx / TILE_SIZE;
    int ty = wy / TILE_SIZE;

    if (tx < 0 || tx >= WIDTH || ty < 0 || ty >= HEIGHT)
        return false;

    return tiles[1][ty][tx] != 0; // Ground レイヤー
}

