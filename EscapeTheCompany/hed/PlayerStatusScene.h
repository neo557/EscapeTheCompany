#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Scene.h"
#include "SceneManager.h"
#include "PlayerStatusManager.h"
#include "ItemManager.h"
#include "Button.h"
#include "UIManager.h"

class PlayerStatusScene : public Scene {
public:
	PlayerStatusScene(sf::RenderWindow* window, Player* player);
	void onEnter() override;
	void onExit() override;
	void handleEvent(const sf::Event& event) override;
	void update(float dt) override;
	void draw(sf::RenderWindow& window) override;

private:
	sf::Font font;
	sf::Event event;
	sf::Texture playerTex;
	sf::Sprite playerSprite;

	sf::Texture springTexNone, springTexNormal, springTexFire, springTexIce, springTexElectric;
	sf::Sprite spNone, spNormal, spFire, spIce, spElectric;

	sf::RenderWindow* windowRef;
	PlayerStatusManager* statusManager; 
	Player* player;
	ItemManager* manager;

	UIManager ui;

	Button* itemButton;
	Button* weaponButton;
	Button* libraryButton;
	Button* systemButton;
	
};