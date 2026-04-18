#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "SceneManager.h"

class TitleScene : public Scene {
public:
    TitleScene() {}

    void onEnter() override {
        // タイトル画面に入ったときの処理
    }

    void onExit() override {
        // タイトル画面を抜けるときの処理
    }

    void handleEvent(const sf::Event& event) override {
        // キー入力など
    }

    void update(float dt) override {
        // タイトル画面の更新処理
    }

    void draw(sf::RenderWindow& window) override {
        // タイトル画面の描画
        sf::Text text;
        // 必要ならフォント設定など
    }
};