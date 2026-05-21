#pragma once
#include <SFML/Graphics.hpp>
#include "GimmickBase.h"

class DestroyableObject : public GimmickBase {
public:
	int hp = 1;

	void onTrigger(Player* player) override{
		if (!active) return;

		hp--;
		if (hp <= 0) {
			active = false;
			// ここでオブジェクトが破壊されたときのエフェクトやアイテムドロップなどの処理
		}
	}

};