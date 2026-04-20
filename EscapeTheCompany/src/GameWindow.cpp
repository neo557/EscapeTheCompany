#include <SFML/Graphics.hpp>
#include "GameWindow.h"

GameWindow::GameWindow():window(sf::VideoMode(800, 600), "Escape The Company")
{
    tilemap = new TileMap();

    printf("%d\n", tilemap->tiles[1][0][0]);
}

void GameWindow::run() {
    sf::Clock clock;
    sceneManager.changeScene<TitleScene>();
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        handleEvents();
        update(dt);
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
    window.clear();

    sceneManager.draw(window);
    window.display();
}