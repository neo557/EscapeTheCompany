#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Escape The Company");
    sf::Event event;

    sf::RectangleShape player(sf::Vector2f(50, 50));
    sf::RectangleShape ground(sf::Vector2f(800, 50));
    sf::Vector2f velocity(0, 0);

    // 地面テクスチャ
    sf::Texture groundTex;
    groundTex.loadFromFile("ground.png");
    ground.setTexture(&groundTex);

    // 色は 0〜255 の範囲にする
    ground.setFillColor(sf::Color(100, 100, 255));
    ground.setPosition(0, 550);

    // 背景テクスチャ
    sf::Texture bgTex;
    bgTex.loadFromFile("background.png");
    sf::Sprite background(bgTex);

    player.setFillColor(sf::Color::White);
    player.setPosition(375, 550);

    float speed = 1.0f;
    float gravity = 0.5f;

    bool isOnGround = false;
    float jumpPower = -12.0f;

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // 左右移動
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            player.move(-speed, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            player.move(speed, 0);

        // ジャンプ
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && isOnGround)
        {
            velocity.y = jumpPower;
            isOnGround = false;
        }

        // 重力
        velocity.y += gravity;
        player.move(0, velocity.y);

        // 地面との衝突
        float groundY = ground.getPosition().y;
        if (player.getPosition().y + player.getSize().y >= groundY)
        {
            player.setPosition(player.getPosition().x, groundY - player.getSize().y);
            velocity.y = 0;
            isOnGround = true;
        }
        else
        {
            isOnGround = false;
        }

        window.clear(sf::Color::Black);
		window.draw(background);
        window.draw(ground);
        window.draw(player);
        window.display();
    }

    return 0;
}