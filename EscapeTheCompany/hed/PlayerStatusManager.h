#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include "CharacterData.h"
#include "EnemyManager.h"

class Player;

class PlayerStatusManager {
public:
    // データベース（id → CharacterData）
    std::unordered_map<int, CharacterData> playerDatabase;

    // テクスチャキャッシュ
    std::unordered_map<std::string, sf::Texture> textureCache;

    // ステータス
    int maxHp = 0;
    int hp = 0;
    int attack = 0;
    int defence = 0;
    int speed = 0;
    int Exp = 0;
    int level = 1;
    int nextExp = 10;
    int culcDamage(const Enemy& enemy);

    sf::Vector2f logicSize;
    sf::Vector2f drawSize;
    sf::Vector2f hitboxOffset;
    sf::Vector2f startPos; // 初期位置（CSVに入れるなら後で変更）

    std::string spritePath;



    float getHpRatio() const;         // HPバー用

    void applyDamage(int dmg);        // ダメージ処理
    // CSV 読み込み
    void loadPlayerDataFromCSV(const std::string& path);
    void addExp(int amount);
    void levelUp();

    // Player 生成
    void spawn(Player* player, int id, sf::Vector2f pos);

};