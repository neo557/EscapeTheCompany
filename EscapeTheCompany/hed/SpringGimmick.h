#pragma once
#include <SFML/Graphics.hpp>
#include "GimmickBase.h"
#include "Player.h"
#include "SpringGimmickType.h"
#include "PlayerStatusManager.h"

class Player; // 前方宣言

class SpringGimmick : public GimmickBase {
public:
    SpringType required;   // 必要なばね
    SpringGimmickType type; // ギミックの種類（火床・氷床など）

    bool slip = false;
    float slipFactor = 0.9f;
    float stuntimer = 0.f;

    SpringGimmick(SpringType req, SpringGimmickType t)
        : required(req), type(t) {}

    void onTrigger(Player* player, float dt) override {

        // ばねが一致しないなら発動しない
        if (player->statusManager->currentSpring == required) {
            return;
        }

        // ギミックの種類ごとに処理を分ける
        switch (type) {
        case SpringGimmickType::FireFloor:
            applyFire(player, dt);
            break;

        case SpringGimmickType::IceFloor:
            applyIce(player, dt);
            break;

        case SpringGimmickType::ElectricFloor:
            applyElectric(player, dt);
            break;

        case SpringGimmickType::WaterFloor:
            applyWater(player, dt);
            break;
        }
    }
    static SpringType requiredSpringFor(SpringGimmickType type) {
        switch (type) {
        case SpringGimmickType::FireFloor:     return SpringType::Fire;
        case SpringGimmickType::IceFloor:      return SpringType::Ice;
        case SpringGimmickType::ElectricFloor: return SpringType::Electric;
        case SpringGimmickType::WaterFloor:    return SpringType::Ice; // 氷で凍らせる
        default: return SpringType::None;
        }
    }

    void applyFire(Player* player, float dt) {
        float maxHp = player->statusManager->maxHp;
        player->statusManager->hp -= maxHp * 0.05f * dt; // 毎秒5%
    }

    void applyIce(Player* player, float dt) {

        // まず現在の速度をコピー
        sf::Vector2f v = player->velocity;

        // 入力による加速
        float accel = 80.f;        // 通常より小さく
        float friction = 50.f;      // 摩擦を極端に小さく

        if (player->moveLeft)  v.x -= accel * dt;
        if (player->moveRight) v.x += accel * dt;

        // キーを離したときの減速（摩擦）
        if (!player->moveLeft && !player->moveRight) {
            if (v.x > 0) v.x -= friction * dt;
            if (v.x < 0) v.x += friction * dt;

            // 摩擦で止まるときの閾値
            if (std::abs(v.x) < 5.f) v.x = 0.f;
        }

        // 結果を Player に書き戻す
        player->velocity = v;
    }

    void applyElectric(Player* player, float dt) {
        player->stuntimer = 1.0f; // 結果だけ渡す
    }

    void applyWater(Player* player, float dt) {
        // IceSpring なら凍らせるなど
    }
};

