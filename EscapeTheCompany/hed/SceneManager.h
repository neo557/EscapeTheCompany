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

	//押しっぱなし防止
	bool isPressed = false;

	//---最後にtrueになったステージ番号---
	int lastStage = 1;

	//---戻り地点---
	sf::Vector2f returnPos;

	std::unordered_map<sf::Keyboard::Key, bool> prevkey; // キーの押下状態を管理するマップ
	std::unordered_map<sf::Keyboard::Key, bool> curkey; // 前フレームのキーの押下状態を管理するマップ

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

		updateKeyState();
		applyPending();
		if (current) current->update(dt);
		lateUpdateKeyState();
	}

	void draw(sf::RenderWindow& window) {
		
		if (current) current -> draw(window);
	}

	void updateKeyState() {
		for (int k = 0; k < sf::Keyboard::KeyCount; k++) {
			curkey[(sf::Keyboard::Key)k] = sf::Keyboard::isKeyPressed((sf::Keyboard::Key)k);
		}
	}

	bool isPressedOnce(sf::Keyboard::Key key) {
		return curkey[key] && !prevkey[key];
	}

	void lateUpdateKeyState() {
		prevkey = curkey;
	}

	void resetKeyState() {
		prevkey.clear();
		curkey.clear();
	}
};
