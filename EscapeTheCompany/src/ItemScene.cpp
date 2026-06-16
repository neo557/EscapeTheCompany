#include "ItemScene.h"
#include "SceneManager.h"
#include "SaveData.h"
#include "GameScene.h"
#include "GameScene2.h"
#include "ItemManager.h"


ItemScene::ItemScene(sf::RenderWindow* window, Player* player, ItemManager* itemManager) : windowRef(window), playerRef(player), itemManagerRef(itemManager) {
	window->clear(sf::Color(30, 30, 30));
}

void ItemScene::init() {
    font.loadFromFile("Fonts/KH-Dot-Kagurazaka-16.ttf");
    // アイテム一覧の生成
    const auto& db = itemManagerRef->itemDatabase;
    for (auto& [id, data] : db) {
        if (itemManagerRef->getItemCount(id) > 0) {
            ItemIcon icon;
            icon.itemId = id;

            // PNG 読み込み
            sf::Texture* tex = new sf::Texture();
            tex->loadFromFile(data.photo);
            textures.push_back(std::unique_ptr<sf::Texture>(tex));

            icon.sprite.setTexture(*tex);

            // 3:2 にリサイズ
            float w = 96;
            float h = 64;
            icon.sprite.setScale(
                w / tex->getSize().x,
                h / tex->getSize().y
            );

            icons.push_back(icon);
        }
    }

    // 選択枠
    highlight.setSize({ 96, 64 });
    highlight.setFillColor(sf::Color(255, 255, 255, 40));
    highlight.setOutlineThickness(2);
    highlight.setOutlineColor(sf::Color(255, 255, 255, 180));

    // 右側の詳細テキスト
    nameText.setFont(font);
    nameText.setCharacterSize(32);

    effectText.setFont(font);
    effectText.setCharacterSize(24);

    flavorText.setFont(font);
    flavorText.setCharacterSize(20);


    if (!icons.empty()) {
        selectedIndex = 0;
        selectedItemId = icons[0].itemId;
    }
}

void ItemScene::onEnter() {

}

void ItemScene::onExit() {

}

void ItemScene::handleEvent(const sf::Event& event) {
    // Esc で戻る
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Escape) {
        SaveData save;
        save.saveGame(playerRef);
        SceneManager::instance().popScene();
        return;
    }

    // アイテムが 0 個なら何もしない
    if (icons.empty()) {
        selectedIndex = -1;
        selectedItemId = -1;
        return;
    }

    // マウス位置
    sf::Vector2i mousePos = sf::Mouse::getPosition(*windowRef);
    sf::Vector2f mousePosF((float)mousePos.x, (float)mousePos.y);

    // --- マウスホバーで選択更新 ---
    for (int i = 0; i < (int)icons.size(); i++) {
        if (icons[i].sprite.getGlobalBounds().contains(mousePosF)) {
            selectedIndex = i;
            selectedItemId = icons[i].itemId;
        }
    }

    // カーソル移動（キーボード）
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Right) selectedIndex++;
        if (event.key.code == sf::Keyboard::Left)  selectedIndex--;
        if (event.key.code == sf::Keyboard::Up)    selectedIndex -= 5;
        if (event.key.code == sf::Keyboard::Down)  selectedIndex += 5;

        selectedIndex = std::clamp(selectedIndex, 0, (int)icons.size() - 1);
        selectedItemId = icons[selectedIndex].itemId;
    }

    // マウスホイールでスクロール
    if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.delta > 0) scrollOffset -= 40;
        if (event.mouseWheelScroll.delta < 0) scrollOffset += 40;
    }

    // Enter で使用
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Enter) {
        if (selectedItemId != -1) {
            itemManagerRef->useItem(selectedItemId, playerRef);
        }
    }

    // マウス左クリックで使用
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {
        if (selectedItemId != -1) {
            itemManagerRef->useItem(selectedItemId, playerRef);
        }
    }
}



void ItemScene::update(float dt) {
}

void ItemScene::draw(sf::RenderWindow& window) {
    /*sf::Text t;
    font.loadFromFile("Fonts/KH-Dot-Kagurazaka-16.ttf");
    t.setFont(font);
    t.setFillColor(sf::Color::White);
    float y = 100;
    int index = 0;*/


    if (icons.empty()) return;

    // 左側アイテム一覧
    float startX = 50;
    float startY = 100;
    float gapX = 120;
    float gapY = 90;
    int col = 5;

    for (int i = 0; i < (int)icons.size(); i++) {
        int cx = i % col;
        int cy = i / col;

        sf::Vector2f pos(
            startX + cx * gapX,
            startY + cy * gapY - scrollOffset
        );

        icons[i].pos = pos;
        icons[i].sprite.setPosition(pos);
        window.draw(icons[i].sprite);

        if (i == selectedIndex) {
            highlight.setPosition(pos);
            window.draw(highlight);
        }
    }

    // 右側詳細表示
    const ItemData* data = itemManagerRef->getItemData(icons[selectedIndex].itemId);
    if (!data) return;

    nameText.setString(data->name);
    nameText.setPosition(700, 100);
    window.draw(nameText);

    effectText.setString(data->effect);
    effectText.setPosition(700, 150);
    window.draw(effectText);

    flavorText.setString(data->description);
    flavorText.setPosition(700, 200);
    window.draw(flavorText);

    largeSprite = icons[selectedIndex].sprite;
    largeSprite.setScale(2.0f, 2.0f);
    largeSprite.setPosition(700, 300);
    window.draw(largeSprite);
}