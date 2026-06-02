#include "PlayerPhysicsManager.h"
#include "Player.h"
#include "FloorType.h"

void PlayerPhysicsManager::applyInput(Player* player, float dt) {
	if (stuntimer > 0.f) return;

	float targetspeed = 0.f;

	if (player->moveLeft) targetspeed = -movespeed;
	if (player->moveRight) targetspeed = movespeed;

	float accel = slip ? 400.f : 1200.f;

	velocity.x += (targetspeed - velocity.x) * accel * dt;
}

void PlayerPhysicsManager::applyGravity(float dt) {
	velocity.y += gravity * dt;
	if (velocity.y > 800) velocity.y = 800;
}
