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
        std::getline(ss, springStr, ',');
        if (springStr == "Fire") data.springType = SpringType::Fire;
        else if (springStr == "Ice") data.springType = SpringType::Ice;
        else if (springStr == "Electric") data.springType = SpringType::Electric;
        else data.springType = SpringType::None;

        // aiType
        std::getline(ss, data.aiType, ',');

        //Exp
        std::getline(ss,token, ','); data.Exp = std::stoi(token);

        //BattleSprite
		std::getline(ss, data.battleSprite, ',');

        playerDatabase[data.id] = data;
    }
    printf("[DEBUG] Loaded entries: %zu\n", playerDatabase.size());

}

void PlayerStatusManager::onHandle(sf::Event event,
    const std::vector<SpringType>& allowed)
{
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::R)
    {
        int next = (int)currentSpring;

        for (int i = 0; i < 5; i++) {
            next = (next + 1) % 5;

            if (springunlocked[next] &&
                std::find(allowed.begin(), allowed.end(), (SpringType)next) != allowed.end())
            {
                currentSpring = (SpringType)next;
                break;
            }
        }
    }
}

void PlayerStatusManager::addExp(int amount) {
    Exp += amount;

    while (Exp >= nextExp) {
        Exp -= nextExp;
        levelUp();
    }
}

void PlayerStatusManager::levelUp() {
    level++;

    //経験値傾斜
    nextExp = static_cast<int>(nextExp * (1.0f + level * 0.3f));

    int hpUp = rand() % 4 + 2;
    int attackUp = rand() % 4 + 2;
	int defenceUp = rand() % 4 + 2;
	int speedUp = rand() % 4 + 2;
    //ステ上昇
    maxHp += hpUp;
    attack += attackUp;
    defence += defenceUp;
    speed += speedUp;

	lasthpUp = hpUp;
	lastattackUp = attackUp;
	lastdefenceUp = defenceUp;
	lastspeedUp = speedUp;

    hp = maxHp;
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

    // 戦闘用テクスチャ読み込み
    if (!data->battleSprite.empty()) {
        battleTexture.loadFromFile(data->battleSprite);
        battleSprite.setTexture(battleTexture);

        // サイズ調整
        sf::Vector2u texSize = battleTexture.getSize();
        float scaleX = data->battleSize.x / texSize.x;
        float scaleY = data->battleSize.y / texSize.y;
        battleSprite.setScale(scaleX, scaleY);
    }

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

void PlayerStatusManager::loadBattleAppearance(const std::string& path, sf::Vector2f size) {
	battleTexture.loadFromFile(path);
	battleSprite.setTexture(battleTexture);

    //サイズ調整
	sf::Vector2u texSize = battleTexture.getSize();
	float scaleX = size.x / texSize.x;
	float scaleY = size.y / texSize.y;
    battleSprite.setScale(scaleX, scaleY);

    battleSize = size;
}

void PlayerStatusManager::applyDamage(int dmg) {
    hp -= dmg;
    if (hp < 0) hp = 0;
}

int PlayerStatusManager::calcDamage(const Enemy& enemy) {
    float base = attack + level * 1.8f;
	float reduction = enemy.defence * (level / 2.0f);
    float springMul = getSpringMultiplier(enemy.springType);
	int dmg = static_cast<int>((base - reduction) * springMul);
	return std::max(dmg, 1);
}

float PlayerStatusManager::getHpRatio() const {
    if (maxHp == 0) return 0.f;
    return (float)hp / (float)maxHp;
}

float PlayerStatusManager::getSpringMultiplier(SpringType enemySpring) const {
    SpringType p = currentSpring;
	if (p == SpringType::Fire && enemySpring == SpringType::Ice) return 1.5f;
    if (p == SpringType::Ice && enemySpring == SpringType::Electric) return 1.5f;
	if (p == SpringType::Electric && enemySpring == SpringType::Fire) return 1.5f;
    return 1.0f; // デフォルト
}

bool PlayerStatusManager::isDiscovered(SpringType type) const {
    return springunlocked[(int)type];
}

void PlayerStatusManager::discoverSpring(SpringType type) {
	if (type != SpringType::None) {
		springunlocked[(int)type] = true;
	}
}