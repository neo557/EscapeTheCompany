#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <json.hpp>
#include "Player.h"
#include "SceneManager.h"
using json = nlohmann::json;

struct Savedata {
    int stage;
    float playerX;
    float playerY;

    int hp;
    int maxHp;
    int attack;
    int defence;
    int speed;
    int level;
    int exp;

    int currentSpring;
    bool springUnlocked[5];
};

class SaveData {
public:
	void saveGame(Player* player) {
		Savedata data;

		data.stage = SceneManager::instance().lastStage;
		data.playerX = player->worldPos.x;
		data.playerY = player->worldPos.y;

		auto& st = *player->statusManager;
		data.hp = st.hp;
		data.maxHp = st.maxHp;
		data.attack = st.attack;
		data.defence = st.defence;
		data.speed = st.speed;
		data.level = st.level;
		data.exp = st.Exp;

		data.currentSpring = (int)st.currentSpring;
		for (int i = 0; i < 5; i++) {
			data.springUnlocked[i] = st.springunlocked[i];
		}

		json j = {
			{"stage", data.stage },
			{ "playerX", data.playerX },
			{ "playerY", data.playerY },
			{ "hp", data.hp },
			{ "maxHp", data.maxHp },
			{ "attack", data.attack },
			{ "defence", data.defence },
			{ "speed", data.speed },
			{ "level", data.level },
			{ "exp", data.exp },
			{ "currentSpring", data.currentSpring },
			{ "springUnlocked", std::vector<bool>(data.springUnlocked, data.springUnlocked + 5) }
		};

		std::ofstream file("save.json");
		file << j.dump(4); // インデント付きで保存
	}

	bool loadGame(Player* player) {
		std::ifstream file("save.json");
		if (!file.is_open()) return false;

		json j;
		file >> j;

		Savedata data;
		data.stage = j["stage"];
		data.playerX = j["playerX"];
		data.playerY = j["playerY"];
		data.hp = j["hp"];
		data.maxHp = j["maxHp"];
		data.attack = j["attack"];
		data.defence = j["defence"];
		data.speed = j["speed"];
		data.level = j["level"];
		data.exp = j["exp"];
		data.currentSpring = j["currentSpring"];
		for (int i = 0; i < 5; i++) {
			data.springUnlocked[i] = j["springUnlocked"][i];
		}

		//---呼び出し---
		SceneManager::instance().lastStage = data.stage;
		player->worldPos = { data.playerX,data.playerY };

		auto& st = *player->statusManager;
		st.hp = data.hp;
		st.maxHp = data.maxHp;
		st.attack = data.attack;
		st.defence = data.defence;
		st.speed = data.speed;
		st.level = data.level;
		st.Exp = data.exp;
		st.currentSpring = (SpringType)data.currentSpring;
		for (int i = 0; i < 5; i++) {
			st.springunlocked[i] = data.springUnlocked[i];
		}

		return true;
	}
};