#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "CharacterData.h"
#include "EnemyManager.h"
#include "TileMap.h"

std::unordered_map<std::string, sf::Texture> EnemyManager::textureCache;


void EnemyManager::spawn(int id, sf::Vector2f pos) {

    auto it = enemyDatabase.find(id);
    if (it == enemyDatabase.end()) {
        printf("CharacterData not found for id = %d\n", id);
        return;
    }

    CharacterData* data = &it->second;

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

    // Enemy を作る前にテクスチャを決める
    sf::Texture& tex = textureCache[path];

    // Enemy を生成
    Enemy* e = new Enemy(data, pos, id);

    // テクスチャをセット
    e->sprite.setTexture(tex);

    // 位置をセット（テクスチャ設定後に行う）
    e->sprite.setPosition(pos);

    enemies.emplace_back(e);
}

void EnemyManager::loadEnemyDataFromCSV(const std::string& filename) {
	// CSVファイルから敵データを読み込む処理をここに実装
	std::ifstream file(filename);
	if (!file.is_open()) {
        printf("Failed to open EnemyData CSV file: %s\n", filename.c_str());
        return;
    }

    std::string line;
	bool isHeader = true;

    while (std::getline(file, line)) {
        if (isHeader) {
            isHeader = false;
            continue;
        }

        std::stringstream ss(line);
        std::string cell;

        CharacterData data;

        std::getline(ss, cell, ','); data.id = std::stoi(cell);
		std::getline(ss, cell, ','); data.name = cell;
		std::getline(ss, cell, ','); data.texturePath = cell;
		std::getline(ss, cell, ','); data.maxHp = std::stoi(cell);
		std::getline(ss, cell, ','); data.attack = std::stoi(cell);
		std::getline(ss, cell, ','); data.defence = std::stoi(cell);
		std::getline(ss, cell, ','); data.speed = std::stoi(cell);
        std::getline(ss, cell, ','); data.logicSize.x = std::stoi(cell);
        std::getline(ss, cell, ','); data.logicSize.y = std::stoi(cell);

        std::getline(ss, cell, ','); data.drawSize.x = std::stoi(cell);
        std::getline(ss, cell, ','); data.drawSize.y = std::stoi(cell);

        std::getline(ss, cell, ','); data.battleSize.x = std::stoi(cell);
        std::getline(ss, cell, ','); data.battleSize.y = std::stoi(cell);

        std::getline(ss, cell, ','); data.hitboxOffset.x = std::stoi(cell);
        std::getline(ss, cell, ','); data.hitboxOffset.y = std::stoi(cell);

		std::string springStr;
        std::getline(ss, cell, ','); springStr = cell;
        if (springStr == "Fire") data.springType = SpringType::Fire;
        else if (springStr == "Ice") data.springType = SpringType::Ice;
        else if (springStr == "Electric") data.springType = SpringType::Electric;
        else data.springType = SpringType::None;
        std::getline(ss, cell, ','); data.aiType = cell;
        std::getline(ss, cell, ','); data.Exp = std::stoi(cell);
        std::getline(ss, cell, ','); data.isBoss = (cell == "1");

        enemyDatabase[data.id] = data;
    }
}

void EnemyManager::update(float dt, TileMap& map) {
	for (auto& enemy : enemies) {
		enemy->update(dt, map);
	}
}

void EnemyManager::draw(sf::RenderWindow& window) {
	for (auto& enemy : enemies) {
		enemy->draw(window);
	}
}

Enemy* EnemyManager::checkCollision(const sf::FloatRect& playerBounds) {
	for (auto& enemy : enemies) {
		if (playerBounds.intersects(enemy->getBounds())) {
			return enemy;
		}
	}
	return nullptr;
}

void EnemyManager::removeEnemy(int id) {
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [id](Enemy* e) {
                return e->data.id == id; // delete しない
            }),
        enemies.end()
    );
}
void EnemyManager::clear() {
    for (auto e : enemies) delete e;
    enemies.clear();
}

std::vector<int> EnemyManager::rollDrops(Enemy* enemy) {
    std::vector<int> result;

    if (enemy->drops.empty()) return result;

    for (auto& drop : enemy->drops) {
        int r = rand() % 100;

        if (r < drop.probability) {
            result.push_back(drop.itemId);
        }
    }
    return result;
}

EnemyManager::~EnemyManager() {
    printf("[EM] ~EnemyManager this = %p\n", this);
    for (auto e : enemies) {
        delete e;
    }
    enemies.clear();

    enemyDatabase.clear();
}