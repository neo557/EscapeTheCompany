
#include <SFML/Graphics.hpp>
#include "SpringType.h"

Spring::Spring(SpringType t) : type(t) {
    switch (t) {
    case SpringType::None:
        damageMultiplier = 0.0f;
		break;

    case SpringType::Normal:
        damageMultiplier = 1.0f;
        break;

    case SpringType::Fire:
        damageMultiplier = 1.2f;
        ignoreFireFloor = true;
        statusChance = 0.2f; // 炎上
        break;

    case SpringType::Ice:
        damageMultiplier = 1.1f;
        ignoreIceFloor = true;
        statusChance = 0.15f; // 凍結
        break;

    case SpringType::Electric:
        damageMultiplier = 1.1f;
        ignoreElectricFloor = true;
        statusChance = 0.15f; // 麻痺
        break;
    }
}