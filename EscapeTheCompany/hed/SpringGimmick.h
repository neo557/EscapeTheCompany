#include <SFML/Graphics.hpp>
#pragma once
#include "GimmickBase.h"

class SpringGimmmick : public GimmickBase {
public:
    SpringType required;

    SpringGimmmick(SpringType req) : required(req) {}

    void onTrigger(Player* player) override {
        if (player->statusManager->currentSpring == required) {
            // 通行可能
        }
        else {
            // 通れない
        }
    }
};
