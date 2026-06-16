#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include <Windows.h>
#include "ItemData.h"
#include "Player.h"
#include "ItemManager.h"

class ItemManager {
private:
	
public :

	std::unordered_map<int, ItemData> itemDatabase;
	std::unordered_map<int, int> inventory;
	std::wstring utf8_to_wstring(const std::string& utf8);

	ItemManager();
	void LoadItemDataFromCSV(const std::string& csvPath);
	int getItemCount(int id) const;
	const ItemData* getItemData(int id) const;
	bool useItem(int id, Player* player);
	void addItem(int Id, int amount);
};