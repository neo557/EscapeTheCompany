#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "Player.h"
#include "ItemManager.h"
#include "SaveData.h"

class ItemScene : public Scene {
public :
	ItemScene(sf::RenderWindow* window, Player* player, ItemManager* manager);

	void onEnter() override;
	void onExit() override;
	void handleEvent(const sf::Event& event) override;
	void update(float dt) override;
	void draw(sf::RenderWindow& window) override;

private :
	sf::RenderWindow* windowRef;
	Player* playerRef;
	ItemManager* itemManagerRef;
	EnemyManager* enemyManager;
	sf::Font font;

	int selectedIndex = 0;
	int selectedItemId = -1;
	std::vector<int> itemList;
};