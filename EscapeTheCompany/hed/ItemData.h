#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class ItemData {
public :
	int id = 0;
	std::wstring name;
	std::string photo;
	std::string effect;
	int value = 0;
	int max = 0;
	std::string target;
	std::wstring description;
	std::string usable;
	std::string type;
};