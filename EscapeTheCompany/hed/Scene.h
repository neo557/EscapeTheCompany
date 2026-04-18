#pragma once
#include <SFML/Graphics.hpp>

class Scene
{
public:
	virtual ~Scene() {}

	virtual void onEnter() {}
	virtual void onExit() {}

	virtual void handleEvent(const sf::Event &event) = 0;
	virtual void update(float dt) = 0;
	virtual void draw(sf::RenderWindow &window) = 0;
};