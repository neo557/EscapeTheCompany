#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Escape The Company");
    sf::Event event;

    sf::RectangleShape player(sf::Vector2f(50, 50));
    sf::RectangleShape ground(sf::Vector2f(800, 50));
    sf::Vector2f velocity(0, 0);
	sf::Clock clock;    //deltaTimeを計算するための時計

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

    float speed = 100.0f;     // 1秒で200px動く
    float gravity = 100.0f;   // 1秒で100px落ちる
    float jumpPower = -350.0f; // 上方向に350px/s
    // 速度が小さいときは重力を弱める
    float gravityScale = 1.0f;
	float dt = clock.restart().asSeconds(); // 最初のフレームのdeltaTimeを計算

    bool isOnGround = false;

   

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // 左右移動
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            player.move(-speed * dt, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            player.move(speed * dt, 0);

        // ジャンプ
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && isOnGround)
        {
            velocity.y = jumpPower;
            isOnGround = false;
        }

        // --- 重力計算（1回だけ） ---
        float gravityScale = 1.0f;

        // 上昇中（velocity.y < 0）
        if (velocity.y < 0)
        {
            gravityScale = 0.5f;  // 上昇は軽く
        }

        // 頂点付近（速度が小さい）
        if (std::abs(velocity.y) < 30.0f)
        {
            gravityScale = 0.1f;  // さらにふわっと
        }

        // 下降中（velocity.y > 0）
        if (velocity.y > 0)
        {
            gravityScale = 1.15f;  // 落下は強く
        }

		// ジャンプの高さ調整
        if (velocity.y < 0 && !sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            velocity.y *= 0.5f;
        }

        // 重力を1回だけ適用
        velocity.y += gravity * gravityScale * dt;

        //重力の限界地設定
		float maxFallSpeed = 100.0f;
		if (velocity.y > maxFallSpeed)
            velocity.y = maxFallSpeed;

        // 移動
        player.move(0, velocity.y * dt);

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