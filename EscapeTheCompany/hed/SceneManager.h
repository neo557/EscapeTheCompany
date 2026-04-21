#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"

class SceneManager {
private:
	std::unique_ptr<Scene> current;
	std::unique_ptr<Scene>pending;

public:
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

	void handleEvent(const sf::Event& event) {
		if (current) current->handleEvent(event);
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
