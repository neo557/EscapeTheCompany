#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class EnemyData {
public:
    int id;
    std::string name;
    std::string texturePath;
    int maxHp;
    sf::Vector2f defaultSize; // 論理サイズ（64x64 など)
};