#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "Player.h"

class GimmickBase {
public:
	sf::FloatRect area;
	bool active = true;

	virtual void onTrigger(Player* player) = 0;
	virtual ~GimmickBase(){
		/// デストラクタ

	}
};