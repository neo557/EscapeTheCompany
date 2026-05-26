#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "EnemyManager.h"
#include "PlayerStatusManager.h"
#include "Player.h"

/// シーン管理クラス
struct TransitionContext {
	sf::Vector2f returnPos;
	bool isBossBattle = false;
	int bossId = -1;
};

class SceneManager {
private:

	std::unique_ptr<Scene> current;
	std::unique_ptr<Scene>pending;

public:
	bool debugHitBox = false; // ヒットボックスのデバッグ表示フラグ

	//---ステージ進行フラグ---
	bool st1 = true; // ステージ1をクリアしたか
	bool st2 = false; //ステージ２
	bool st3 = false; //ステージ３

	//---最後にtrueになったステージ番号---
	int lastStage = 1;

	//---戻り地点---
	sf::Vector2f returnPos;

	//---シングルトン---
	static SceneManager& instance() {
		static SceneManager instance;
		return instance;
	}

	//---ステージフラグ更新---
	void setStage(int stageid) {
		switch (stageid) {
		case 1: st1 = true; break;
		case 2: st2 = true; break;
		case 3: st3 = true; break;
		}
		lastStage = stageid;
	}

	//---戦闘後に戻るべきステージ番号---
	int getReturnStage() const {
		return lastStage;
	}

	TransitionContext transition; // シーン遷移のコンテキスト情報を保持
	SceneManager();

	EnemyManager enemyManager; // EnemyManager のインスタンスを追加
	Player* player; // Player のインスタンスを追加
	EnemyManager* getEnemyManager() { return &enemyManager; } // EnemyManager へのアクセス関数


	
	template<typename T, typename... Args>
	void changeScene(Args&&... args) {
		pending = std::make_unique<T>(std::forward<Args>(args)...);
	}

	void applyPending() {
		if (pending) {
			if (current) current->onExit();
			current = std::move(pending);
			current->onEnter();
		}
	}
	void initGame(sf::RenderWindow* window);
	void scene2(sf::RenderWindow* window);

	void handleEvent(const sf::Event& event) {
		if (current) current->handleEvent(event);

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F3) {
			debugHitBox = !debugHitBox;
		}
	}

	void update(float dt) {
		applyPending();
		if (current) current->update(dt);
	}

	void draw(sf::RenderWindow& window) {
		
		if (current) current -> draw(window);
	}

};
