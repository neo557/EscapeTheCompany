#pragma once
#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include "EnemyData.h"
#include <unordered_map>
#include <vector>

class EnemyManager {
public:
	std::vector<Enemy> enemies;
	std::unordered_map<int, EnemyData> enemyDatabase; // 敵のデータを名前で管理するマップ
	sf::Vector2f lastEncounterPos; // プレイヤーの最後の位置を保持する変数
	// テクスチャキャッシュ（この翻訳単位内のグローバル）
	static std::unordered_map<std::string, sf::Texture> textureCache;

	void loadEnemyDataFromCSV(const std::string& filename); // 敵のデータをロードする関数
	void spawn(int id, sf::Vector2f pos);
	void update(float dt, TileMap& map);
	void draw(sf::RenderWindow& window);
	Enemy* checkCollision(const sf::FloatRect& playerBounds);

};