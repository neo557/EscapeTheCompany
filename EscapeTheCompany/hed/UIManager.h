#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

class UIManager {
public :
	sf::Texture atlas;
	std::unordered_map<std::string, sf::IntRect> rects;
	
	bool load(const std::string& atlasPath, const std::string& jsonPath);
	sf::IntRect getRect(const std::string& name) const;
};