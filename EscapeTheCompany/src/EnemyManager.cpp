#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
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

void EnemyManager::loadEnemyData() {
	// 敵のデータをロードする処理
	// ここでは例として、ハードコードされたデータを使用します。
	enemyDatabase[1] = {
	1,"G_ball","photo/Robot_1.png",30,{64,64},{32,32},{256,256},{20,0}
	};
    enemyDatabase[2] = {
        2,"G_bat","photo/Robot_640x640.png",50,{640,640},{64,64},{265,256},{215,0}
    };
    // 必要に応じて他の敵データも追加
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