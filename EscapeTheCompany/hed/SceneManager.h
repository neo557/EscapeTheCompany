#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "EnemyManager.h"
#include "PlayerStatusManager.h"
#include "Player.h"

class SceneManager {
private:
	std::unique_ptr<Scene> current;
	std::unique_ptr<Scene>pending;

public:
	SceneManager();

	EnemyManager enemyManager; // EnemyManager のインスタンスを追加
	Player* player; // Player のインスタンスを追加
	EnemyManager* getEnemyManager() { return &enemyManager; } // EnemyManager へのアクセス関数

	bool debugHitBox = false; // ヒットボックスのデバッグ表示フラグ
	
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
	void handleEvent(const sf::Event& event) {
		if (current) current->handleEvent(event);

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F3) {
			debugHitBox = !debugHitBox;
		}
	}

	void update(float dt) {
		if (current) current->update(dt);
	}

	void draw(sf::RenderWindow& window) {
		if (current) current -> draw(window);
	}
	static SceneManager& instance() {
		static SceneManager instance;
		return instance;
	}
};
