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
        // 画面の左端と右端（ピクセル座標）
        float left = view.getCenter().x - view.getSize().x / 2;
        float right = view.getCenter().x + view.getSize().x / 2;

        // 左端と右端をタイル番号に変換
        int startX = std::floor(left / TILE_SIZE);
        int endX = std::ceil(right / TILE_SIZE);

        // 範囲チェック
        if (startX < 0) startX = 0;
        if (endX > WIDTH) endX = WIDTH;

        // タイル描画
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = startX; x < endX; x++) {
                Tile& t = tiles[y][x];
                if (t.type == 0) continue;

                sf::RectangleShape rect(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                rect.setFillColor(sf::Color::Blue);
                rect.setPosition(x * TILE_SIZE, y * TILE_SIZE); //  ワールド座標そのまま
                window.draw(rect);
            }
        }
    }