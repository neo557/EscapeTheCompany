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

    sceneManager.changeScene<TitleScene>(&window);

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        if (dt > 0.25f) dt = 0.25f;

        // イベント処理はここだけ
        handleEvents();

        accumulator += dt;

        while (accumulator >= fixedDt) {
            sceneManager.update(fixedDt);
            accumulator -= fixedDt;
        }
        sceneManager.applyPending();

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