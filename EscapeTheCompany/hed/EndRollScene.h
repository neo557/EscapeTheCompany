#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "SceneManager.h"
#include "TitleScene.h"

class EndRollScene : public Scene {
private:
    sf::Font font;
    sf::Text creditsText;
    sf::RenderWindow* window;
    float scrollY = 800.0f;
    float scrollSpeed = 50.0f;

public:
    EndRollScene(sf::RenderWindow* window) : window(window) {}   

    void onEnter() override {

        font.loadFromFile("Fonts/KH-Dot-Dougenzaka-16.ttf");

        creditsText.setFont(font);
        creditsText.setCharacterSize(32);
        creditsText.setFillColor(sf::Color::White);

        creditsText.setString(
            "Game Title\n"
            "\n"
            "Programming\n"
            "  Cro\n"
            "\n"
            "Graphics\n"
            "  Cro\n"
            "\n"
            "Special Thanks\n"
            "  You\n"
            "\n"
            "Thank you for playing!"
        );

        creditsText.setPosition(100, scrollY);
    }

    void update(float dt) override {
        scrollY -= scrollSpeed * dt;
        creditsText.setPosition(100, scrollY);

        if (scrollY + creditsText.getLocalBounds().height < 0) {
            auto& sm = SceneManager::instance();
            sm.changeScene<TitleScene>(window, sm.player, &sm.enemyManager);
        }
       
    }

    void handleEvent(const sf::Event& event) override {
        if (event.type == sf::Event::KeyPressed) {
            auto& sm = SceneManager::instance();
            SceneManager::instance().isPressed = false;
            sm.changeScene<TitleScene>(window, sm.player, &sm.enemyManager);
            
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            printf("EndRoll Enter is pressed\n");
        }
        else {
            printf("EndRoll Enter is NOT pressed\n");
        }
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(creditsText);
    }

	void onExit() override {
		// エンドロールシーンを抜けるときの処理
		SceneManager::instance().resetKeyState(); // キー状態をリセットしておく
	}
};

