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
    const float fixedDt = 1.0f / 60.0f;

    SceneManager::instance().initGame(&window);

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        if (dt > 0.25f) dt = 0.25f;

        // イベント処理はここだけ
        handleEvents();

        accumulator += dt;

        while (accumulator >= fixedDt) {
            SceneManager::instance().update(fixedDt);
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

        SceneManager::instance().handleEvent(event);
    }
}

void GameWindow::update(float dt) {
    SceneManager::instance().update(dt);
}

void GameWindow::draw() {
	window.setView(window.getDefaultView());
    window.clear();
    SceneManager::instance().draw(window);
    window.display();
}