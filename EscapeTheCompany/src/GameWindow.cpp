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
	printf("Player world position: (%.2f, %.2f)\n", player.worldPos.x, player.worldPos.y);
    camera.follow(player.worldPos);
}

void GameWindow::draw() {
    window.clear();

    camera.apply(window);

    // ===== 背景ストライプ描画 =====
    sf::RectangleShape stripe(sf::Vector2f(800, 40));
    stripe.setFillColor(sf::Color(30, 30, 30)); // 濃いグレー

    // 画面の左上のワールド座標を取得
    float left = camera.view.getCenter().x - camera.view.getSize().x / 2;
    float top = camera.view.getCenter().y - camera.view.getSize().y / 2;

    // 画面の高さ分ストライプを敷き詰める
    for (int i = 0; i < 20; i++) {
        stripe.setPosition(left, top + i * 40);
        window.draw(stripe);
    }
    // ===============================

    tilemap->draw(window, camera.view);
    player.draw(window);

    window.display();
}