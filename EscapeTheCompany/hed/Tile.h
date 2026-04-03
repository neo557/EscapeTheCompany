#pragma once
#include <SFML/Graphics.hpp>
class Tile {
public:
    int type; // 0=empty, 1=floor, 2=background, 3=object
    bool solid;
    sf::Sprite sprite;
	Tile();
};
