#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "EnemyManager.h"
#include "TileMap.h"

std::unordered_map<std::string, sf::Texture> EnemyManager::textureCache;
void EnemyManager::spawn(int id, sf::Vector2f pos) {

    auto it = enemyDatabase.find(id);
    if (it == enemyDatabase.end()) {
        printf("EnemyData not found for id = %d\n", id);
        return;
    }

    EnemyData* data = &it->second;

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


    enemies.emplace_back(data, pos);

    enemies.back().sprite.setTexture(textureCache[path]);
    enemies.back().sprite.setPosition(pos);
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

        EnemyData data;

        std::getline(ss, cell, ','); data.id = std::stoi(cell);
		std::getline(ss, cell, ','); data.name = cell;
		std::getline(ss, cell, ','); data.texturePath = cell;
		std::getline(ss, cell, ','); data.maxHp = std::stoi(cell);
		std::getline(ss, cell, ','); data.attack = std::stoi(cell);
		std::getline(ss, cell, ','); data.defence = std::stof(cell);
		std::getline(ss, cell, ','); data.speed = std::stof(cell);
        std::getline(ss, cell, ','); data.logicSize.x = std::stof(cell);
        std::getline(ss, cell, ','); data.logicSize.y = std::stof(cell);

        std::getline(ss, cell, ','); data.drawSize.x = std::stof(cell);
        std::getline(ss, cell, ','); data.drawSize.y = std::stof(cell);

        std::getline(ss, cell, ','); data.battleSize.x = std::stof(cell);
        std::getline(ss, cell, ','); data.battleSize.y = std::stof(cell);

        std::getline(ss, cell, ','); data.hitboxOffset.x = std::stof(cell);
        std::getline(ss, cell, ','); data.hitboxOffset.y = std::stof(cell);

        std::getline(ss, cell, ','); data.elementType = cell;
        std::getline(ss, cell, ','); data.aiType = cell;

        enemyDatabase[data.id] = data;
    }
}

void EnemyManager::update(float dt, TileMap& map) {
	for (auto& enemy : enemies) {
		enemy.update(dt, map);
	}
}

void EnemyManager::draw(sf::RenderWindow& window) {
	for (auto& enemy : enemies) {
		enemy.draw(window);
	}
}

Enemy* EnemyManager::checkCollision(const sf::FloatRect& playerBounds) {
	for (auto& enemy : enemies) {
		if (playerBounds.intersects(enemy.getBounds())) {
			return &enemy;
		}
	}
	return nullptr;
}