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

	int lasthpUp = 0;
	int lastattackUp = 0;
	int lastdefenceUp = 0;
	int lastspeedUp = 0;
	SpringType currentSpring = SpringType::None;
    bool springunlocked[5] = {
        true,
        true,
        false,
        false,
		false 
    }; // スプリングの解放状態

    // Spring を発見したときに呼ぶ
    void discoverSpring(SpringType type);

    // 発見済みかどうか
    bool isDiscovered(SpringType type) const;

    sf::Vector2f logicSize;
    sf::Vector2f drawSize;
    sf::Vector2f hitboxOffset;
    sf::Vector2f startPos; // 初期位置（CSVに入れるなら後で変更）
	sf::Texture battleTexture; // 戦闘用のテクスチャ
	sf::Sprite battleSprite; // 戦闘用のスプライト
    sf::Vector2f battleSize;


    std::string spritePath;
    std::string springStr;
    std::unordered_map<int, SpringType> unlockMap = {
    {1, SpringType::Fire},
    {2, SpringType::Ice},
    {3, SpringType::Electric}
    };

    float getHpRatio() const;         // HPバー用
    float getSpringMultiplier(SpringType enemySpring) const ; // スプリングの相性倍率

    void applyDamage(int dmg);        // ダメージ処理
    // CSV 読み込み
    void loadPlayerDataFromCSV(const std::string& path);
    void loadBattleAppearance(const std::string& path, sf::Vector2f size);
    void addExp(int amount);
    void levelUp();

    //ダメージ計算    
    int calcDamage(const Enemy& enemy);

	void onHandle(sf::Event event,
        const std::vector<SpringType>& allowed); // プレイヤーのステータスに関わるイベント処理

    // Player 生成
    void spawn(Player* player, int id, sf::Vector2f pos);

};