#pragma once
#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include "CharacterData.h"
#include "DropTable.h"
#include "ItemData.h"
#include <unordered_map>
#include <vector>

class ItemManager; //前方宣言

class EnemyManager {
public:
	EnemyManager(ItemManager* itemManager);
	~EnemyManager();

	EnemyManager(const EnemyManager&) = delete;
	EnemyManager& operator = (const EnemyManager&) = delete;

	std::vector<Enemy*> enemies;
	std::unordered_map<int, CharacterData> enemyDatabase; // 敵のデータを名前で管理するマップ
	std::vector<int> rollDrops(Enemy* enemy);
	std::vector<const ItemData*> getDroppedItems(Enemy* enemy);
	sf::Vector2f lastEncounterPos; // プレイヤーの最後の位置を保持する変数
	// テクスチャキャッシュ（この翻訳単位内のグローバル）
	static std::unordered_map<std::string, sf::Texture> textureCache;

	void loadEnemyDataFromCSV(const std::string& filename); // 敵のデータをロードする関数
	void spawn(int id, sf::Vector2f pos);
	void update(float dt, TileMap& map);
	void draw(sf::RenderWindow& window);
	void removeEnemy(int id);
	void clear();
	Enemy* checkCollision(const sf::FloatRect& playerBounds);

private:
	DropTable dropTable;
	ItemManager* itemManager;
};