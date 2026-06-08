#include <SFML/Graphics.hpp>
#include "SceneManager.h"
#include "TitleScene.h"

SceneManager::SceneManager() {
	player = new Player();
	
}

void SceneManager::initGame(sf::RenderWindow* window) {
	// プレイヤーの初期化
	player->statusManager->loadPlayerDataFromCSV("CharacterData/CharacterManager.csv");
	player->statusManager->spawn(player, 0, { 0, 500 });
	player->init(*player->statusManager);

	// 敵キャラの初期化
	enemyManager.loadEnemyDataFromCSV("CharacterData/CharacterManager.csv");
	enemyManager.spawn(1, { 1200, 500 });
	enemyManager.spawn(2, { 1500, 800 });
	printf("GameScene ctor: enemyManager spawned\n");

	printf("[DEBUG] After CSV load: spritePath = %s\n", player->statusManager->spritePath.c_str());
	printf("[DEBUG] logicSize = (%f, %f)\n",
		player->statusManager->logicSize.x,
		player->statusManager->logicSize.y);
	// 最初の GameScene をセット
	changeScene<TitleScene>(window, player, &enemyManager);
}
