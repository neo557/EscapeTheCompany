#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include "PlayerStatusManager.h"
#include "CharacterData.h"
#include "Player.h"

void PlayerStatusManager::loadPlayerDataFromCSV(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        printf("Failed to open %s\n", path.c_str());
        return;
    }

    std::string line;
	std::getline(file, line); // ヘッダー行を読み飛ばす
    while (std::getline(file, line)) {

        if (line.empty()) continue;

		// CSVの各行を解析して CharacterData を作成
        std::stringstream ss(line);
        std::string token;
        CharacterData data;

        std::getline(ss, token, ','); data.id = std::stoi(token);
        std::getline(ss, data.name, ',');
        std::getline(ss, data.texturePath, ',');

        std::getline(ss, token, ','); data.maxHp = std::stoi(token);
        std::getline(ss, token, ','); data.attack = std::stoi(token);
        std::getline(ss, token, ','); data.defence = std::stoi(token);
        std::getline(ss, token, ','); data.speed = std::stoi(token);

        // logicSize
        std::getline(ss, token, ','); data.logicSize.x = std::stof(token);
        std::getline(ss, token, ','); data.logicSize.y = std::stof(token);

        // drawSize
        std::getline(ss, token, ','); data.drawSize.x = std::stof(token);
        std::getline(ss, token, ','); data.drawSize.y = std::stof(token);

        // battleSize
        std::getline(ss, token, ','); data.battleSize.x = std::stof(token);
        std::getline(ss, token, ','); data.battleSize.y = std::stof(token);

        // hitboxOffset
        std::getline(ss, token, ','); data.hitboxOffset.x = std::stof(token);
        std::getline(ss, token, ','); data.hitboxOffset.y = std::stof(token);

        // elementType
        std::getline(ss, data.elementType, ',');

        // aiType
        std::getline(ss, data.aiType, ',');

        playerDatabase[data.id] = data;
    }
    printf("[DEBUG] Loaded entries: %zu\n", playerDatabase.size());

}

void PlayerStatusManager::spawn(Player* player, int id, sf::Vector2f pos)
{
    auto it = playerDatabase.find(id);
    if (it == playerDatabase.end()) {
        printf("PlayerData not found for id = %d\n", id);
        return;
    }

    CharacterData* data = &it->second;

    // ステータス反映
    maxHp = data->maxHp;
    hp = data->maxHp;
    attack = data->attack;
    defence = data->defence;
    speed = data->speed;

    logicSize = data->logicSize;
    drawSize = data->drawSize;
    hitboxOffset = data->hitboxOffset;

    spritePath = data->texturePath;

    // テクスチャ準備
    const std::string& path = data->texturePath;
    if (!path.empty()) {
        auto itTex = textureCache.find(path);
        if (itTex == textureCache.end()) {
            sf::Texture tex;
            tex.loadFromFile(path);
            textureCache[path] = std::move(tex);
        }
    }

    // Player に反映
    player->sprite.setTexture(textureCache[path]);
    player->worldPos = pos;

    std::cout << "[Player Spawn] Texture loaded? "
        << (player->sprite.getTexture() ? "YES" : "NO") << std::endl;

    std::cout << "[Player Spawn] Path = " << path << std::endl;
    std::cout << "[Player Spawn] Pos = " << pos.x << ", " << pos.y << std::endl;

}
void PlayerStatusManager::applyDamage(int dmg) {
    hp -= dmg;
    if (hp < 0) hp = 0;
}

float PlayerStatusManager::getHpRatio() const {
    if (maxHp == 0) return 0.f;
    return (float)hp / (float)maxHp;
}