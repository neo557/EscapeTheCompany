#pragma once
#include <SFML/Graphics.hpp>

class Player; // 前方宣言

class GimmickBase {
public:
	sf::FloatRect area;
	bool active = true;

	virtual void onTrigger(Player* player, float dt) = 0;
	virtual ~GimmickBase(){
		/// デストラクタ

	}
};