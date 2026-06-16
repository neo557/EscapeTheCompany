#include <SFML/Graphics.hpp>
#include "SceneManager.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "GameScene2.h"
#include "ResultScene.h"
#include "ItemScene.h"
#include "PlayerStatusScene.h"
#include "EndRollScene.h"
#include "BattleScene.h"

SceneManager::SceneManager() {
    player = new Player();
}

void SceneManager::initGame(sf::RenderWindow* window) {
    windowRef = window; // ここで保持

    // プレイヤーの初期化
    player->statusManager->loadPlayerDataFromCSV("CharacterData/CharacterManager.csv");
    player->statusManager->spawn(player, 0, { 0, 300 });
    player->init(*player->statusManager);

    // 敵キャラの初期化
    enemyManager.loadEnemyDataFromCSV("CharacterData/CharacterManager.csv");
    enemyManager.spawn(1, { 1200, 500 });
    enemyManager.spawn(2, { 1500, 800 });

    //Itemcsvの初期化と取り込み
    itemManager.LoadItemDataFromCSV("Item/ItemData.csv");

    // 最初のシーンは TitleScene
    changeScene<TitleScene>(windowRef, player, &enemyManager);

    printf("[SM] enemyManager = %p\n", &enemyManager);
}

void SceneManager::update(float dt) {

    if (windowRef == nullptr) {
        printf("[ERROR] windowRef is null in SceneManager::update\n");
    }
    updateKeyState();

    if (!scenes.empty())
        scenes.back()->update(dt);

    lateUpdateKeyState();

    // 遷移予約があればここで実行
    if (nextScene != NextSceneType::None) {
        switch (nextScene) {
        case NextSceneType::Title:
            changeScene<TitleScene>(windowRef, player, &enemyManager);
            break;
        case NextSceneType::GameScene:
            pushScene<GameScene>(windowRef, player, &enemyManager, nextReturnedFromBattle);
            break;
        case NextSceneType::GameScene2:
            pushScene<GameScene2>(windowRef, player, &enemyManager, nextReturnedFromBattle);
            break;
        case NextSceneType::ResultScene:
            changeScene<ResultScene>(
                windowRef,
                player,
                resultEnemyRef,
                resultDrops,
                resultReturnedFromBattle
            );
            break;
        case NextSceneType::ItemScene:
            pushScene<ItemScene>(windowRef, player, &itemManager);
            break;
        case NextSceneType::PlayerStatusScene:
            pushScene<PlayerStatusScene>(windowRef, player);
            break;
        case NextSceneType::EndRollScene:
            changeScene<EndRollScene>(windowRef);
            break;
        case NextSceneType::BattleScene:
            changeScene<BattleScene>(
                player,
                battleEnemyRef,
                windowRef,
                battleAllowedSprings,
                &itemManager,
                &enemyManager
            );
            break;
        default:
            break;
        }
        nextScene = NextSceneType::None;
        nextReturnedFromBattle = false;
    }
}

void SceneManager::popScene() {
    if (!scenes.empty()) {
        scenes.back()->onExit();
        scenes.pop_back();
    }
}

void SceneManager::onSceneChangeTile(int tileId) {
    switch (tileId) {
    case 11 :
        requestScene(NextSceneType::GameScene2,false);
        break;
    default:
        break;
    }

}