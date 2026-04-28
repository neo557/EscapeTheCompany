#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class EnemyData {
public:
    int id;
    std::string name;
    std::string texturePath;
    int maxHp;
    sf::Vector2f defaultSize; // デフォルトサイズ（64x64 など)
	sf::Vector2f logicSize;  // 論理サイズ（64x64 など)
	sf::Vector2f battleSize; // 戦闘時のサイズ（256x256 など)

	sf::Vector2f hitboxoffset; // ヒットボックスのオフセット
};