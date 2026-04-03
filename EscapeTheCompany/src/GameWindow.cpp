#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Camera.h"
#include "TileMap.h"
#include "GameWindow.h"

GameWindow::GameWindow()
    : window(sf::VideoMode(800, 600), "Escape The Company")
{
    tilemap = new TileMap();
}

void GameWindow::run() {
    sf::Clock clock;

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
    }
}

void GameWindow::update(float dt) {
    player.update(dt, *tilemap);
    camera.follow(player.worldPos);
}

void GameWindow::draw() {
    window.clear();

    camera.apply(window);
    tilemap->draw(window, camera.view);
    player.draw(window);

    window.display();
}