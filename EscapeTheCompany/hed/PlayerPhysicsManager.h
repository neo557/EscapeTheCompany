#pragma once
#include <SFML/Graphics.hpp>
#include "FloorType.h"

class Player; // 前方宣言
class PlayerStatusManager; // 前方宣言
class TileMap; // 前方宣言

class PlayerPhysicsManager {
public:
	sf:: Vector2f velocity;
	PlayerStatusManager* statusManager;

	float gravity = 900.0f;
	float movespeed = 200.0f;
	float jumpforce = 500;

	bool slip = false;
	float slipFactor = 0.9f;

	float stuntimer = 0.f;

	void applyInput(Player* player, float dt);
	void applyGravity(float dt);
	void applyFloorEffect(Player* player,int tileId, float dt);
	void updatePosition(Player* player, TileMap& map, float dt);
	FloorType convertTileIdToFloorType(int id);
};