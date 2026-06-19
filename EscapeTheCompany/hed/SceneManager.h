#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>
#include <vector>
#include "Scene.h"
#include "EnemyManager.h"
#include "ItemManager.h"
#include "PlayerStatusManager.h"
#include "Player.h"
#include "TextLayer.h"

enum class NextSceneType {
    None,
    Title,
    GameScene,
    GameScene2,
    ResultScene,
    ItemScene,
    PlayerStatusScene,
    BattleScene,
    EndRollScene,
};

struct TransitionContext {
    sf::Vector2f returnPos;
    bool isBossBattle = false;
    int bossId = -1;
};

class SceneManager {
private:
    SceneManager();
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator = (const SceneManager&) = delete;

public:
    // --- デバッグ・進行フラグ ---
    bool debugHitBox = false;
    bool st1 = true;
    bool st2 = false;
    bool st3 = false;
    bool isPressed = false;
    bool gs1talkdone = false;
    int  lastStage = 1;

    Enemy* battleEnemyRef = nullptr;
    EnemyManager* em;
    std::vector<SpringType> battleAllowedSprings;
    sf::Vector2f returnPos;

    std::unordered_map<sf::Keyboard::Key, bool> prevkey;
    std::unordered_map<sf::Keyboard::Key, bool> curkey;
    std::vector<std::unique_ptr<Scene>> scenes;

    //テキスト描画用
    std::unique_ptr<TextLayer> textLayer;
    bool textActive = false;
    void startText(const std::string& csvPath,sf::Font* font) {
        textLayer = std::make_unique<TextLayer>();
        textLayer->init(csvPath,font,em);   // ← CSV 読み込み
        textActive = true;
    }


    static SceneManager& instance() {
        static SceneManager instance;
        return instance;
    }

    void setStage(int stageid) {
        switch (stageid) {
        case 1: st1 = true; break;
        case 2: st2 = true; break;
        case 3: st3 = true; break;
        }
        lastStage = stageid;
    }

    int getReturnStage() const {
        return lastStage;
    }

    TransitionContext transition;

    EnemyManager enemyManager;
    Player* player;
    ItemManager itemManager;
    sf::Font uifont;
    // 追加：Window 参照と遷移予約
    sf::RenderWindow* windowRef = nullptr;
    NextSceneType nextScene = NextSceneType::None;
    bool nextReturnedFromBattle = false;

    Enemy* resultEnemyRef = nullptr;
    std::vector<int> resultDrops;
    bool resultReturnedFromBattle = false;

    EnemyManager* getEnemyManager() { return &enemyManager; }

    template<typename T, typename... Args>
    void changeScene(Args&&... args) {
        if (!scenes.empty()) {
            scenes.back()->onExit();
            scenes.pop_back();
        }
        pushScene<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    void pushScene(Args&&... args) {
        scenes.push_back(std::make_unique<T>(std::forward<Args>(args)...));
        scenes.back()->init();
        scenes.back()->onEnter();
    }

    //  遷移予約用ヘルパ
    void requestScene(NextSceneType type, bool returnedFromBattle = false) {
        nextScene = type;
        nextReturnedFromBattle = returnedFromBattle;
    }

    void initGame(sf::RenderWindow* window);
    void scene2(sf::RenderWindow* window);

    void handleEvent(const sf::Event& event) {

        if (textActive && textLayer) {
            textLayer->handleEvent(event);
            return;
        }
        if (!scenes.empty()) scenes.back()->handleEvent(event);

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F3) {
            debugHitBox = !debugHitBox;
        }
    }

    void update(float dt);

    void draw(sf::RenderWindow& window) {
        if (!scenes.empty()) scenes.back()->draw(window);

        if (textActive && textLayer) textLayer->draw(window);
    }

    void updateKeyState() {
        for (int k = 0; k < sf::Keyboard::KeyCount; k++) {
            curkey[(sf::Keyboard::Key)k] = sf::Keyboard::isKeyPressed((sf::Keyboard::Key)k);
        }
    }

    bool isPressedOnce(sf::Keyboard::Key key) {
        return curkey[key] && !prevkey[key];
    }

    void lateUpdateKeyState() {
        prevkey = curkey;
    }

    void resetKeyState() {
        prevkey.clear();
        curkey.clear();
    }

    void startText(const std::string& csvPath);


    void popScene();
    void onSceneChangeTile(int tileId);
};
