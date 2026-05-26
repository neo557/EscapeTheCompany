#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "SpringType.h" 

class CharacterData {
public:
    int id = 0;
    std::string name;
    std::string texturePath;
    std::string springStr;
	SpringType springType = SpringType::None;
    std::string aiType;
	std::string battleSprite;
    int maxHp = 0;
    int defence = 0;
    int speed = 0;
    int attack = 0;
    int Exp = 0;
	bool isBoss = false;
    sf::Vector2f drawSize = { 0, 0 };
    sf::Vector2f defaultSize = { 0, 0}; // デフォルトサイズ（64x64 など)
	sf::Vector2f logicSize = { 0, 0 };  // 論理サイズ（64x64 など)
	sf::Vector2f battleSize = { 0, 0 }; // 戦闘時のサイズ（256x256 など)

	sf::Vector2f hitboxOffset = { 0, 0 }; // ヒットボックスのオフセット
};