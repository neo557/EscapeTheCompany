#include <SFML/Graphics.hpp>
#include "GameWindow.h"

GameWindow::GameWindow():window(sf::VideoMode(1600, 900), "Escape The Company")
{
    tilemap = new TileMap();

    printf("%d\n", tilemap->tiles[1][0][0]);
}

void GameWindow::run() {
    sf::Clock clock;
    float accumulator = 0.0f;
    const float fixedDt = 1.0f / 60.0f; // 60FPS 固定
    sceneManager.changeScene<TitleScene>(&window);
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        // dt が跳ねたときの安全策（ウィンドウ移動時など）
        if (dt > 0.25f) dt = 0.25f;

        accumulator += dt;

        // --- 固定タイムステップ update ---
        while (accumulator >= fixedDt) {
            handleEvents();
            sceneManager.update(fixedDt);
            accumulator -= fixedDt;
        }
        draw();
    }
}

void GameWindow::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        sceneManager.handleEvent(event);
    }
}

void GameWindow::update(float dt) {
    sceneManager.update(dt);
}

void GameWindow::draw() {
	window.setView(window.getDefaultView());
    window.clear();

    sceneManager.draw(window);
    window.display();
}