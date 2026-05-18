#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "PlayerStatusManager.h"
#include "Player.h"

enum class BattleState {
    Playerturn, Enemyturn, Win, Lose
};
class BattleScene : public Scene {
public:
    BattleScene(Player* player, Enemy* enemy, sf::RenderWindow* window); //敵の見た目反映

    struct BattleLog {
        sf::Text logText;
		float displayTime; // ログの表示時間
    };

	std::vector<BattleLog> battleLogs; // 戦闘ログのリスト

    void onEnter() override;
    void onExit() override;
    void handleEvent(const sf::Event& event) override;
	void executeCommand(int index); // コマンド実行関数
	void playerAttack(); // プレイヤーの攻撃関数
	void enemyAttack(); // 敵の攻撃関数
	void tryRun(); // 逃走の試み関数
	void addLog(const std::wstring& msg); // 戦闘ログ追加関数
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

private:
    BattleState state = BattleState::Playerturn;
	PlayerStatusManager statusManager; // プレイヤーのステータスマネージャーへのポインタ
    Player* player;  // プレイヤーの情報
    Enemy* enemyRef;    // 敵の情報

    // RenderWindowの参照を保持
    sf::RenderWindow* windowRef;
    //背景
    sf::RectangleShape background;
    sf::Text springText;

    //Hpバー
    sf::RectangleShape hpBack;
    sf::RectangleShape hpFront;
    sf::RectangleShape enemyHpBar;

    //Playerの見た目
	sf::Sprite playerSprite; //描画するプレイヤーの情報

    //enemyの見た目
    sf::Sprite enemySprite;//描画する敵の情報


    bool enemyActionPending = false; // 敵の行動が保留中かどうかを示すフラグ
    int enemyHp = 100; // 敵のHP(仮)
    //コマンドウィンドウ
    sf::Font font;
    sf::Font logfont;
    sf::Text commands[3];
    int selectedIndex = 0;
};