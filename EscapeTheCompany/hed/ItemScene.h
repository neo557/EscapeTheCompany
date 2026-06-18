#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Scene.h"
#include "Player.h"
#include "ItemManager.h"

struct ItemIcon {
    sf::Sprite sprite;
    int itemId;
    sf::Vector2f pos;
};

class ItemScene : public Scene {
public:
    ItemScene(sf::RenderWindow* window, Player* player, ItemManager* manager);

    void init();
    void onEnter() override;
    void onExit() override;
    void handleEvent(const sf::Event& event) override;
    void refreshIcons();
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

private:
    // 参照
    sf::RenderWindow* windowRef;
    sf::Text countText;
    Player* playerRef;
    ItemManager* itemManagerRef;

    // フォント
    sf::Font font;

    // アイコン一覧
    std::vector<ItemIcon> icons;
    std::vector<std::unique_ptr<sf::Texture>> textures;

    // 選択状態
    int selectedIndex = 0;
    int selectedItemId = -1;
    float scrollOffset = 0.f;

    // 詳細表示
    sf::Text nameText;
    sf::Text effectText;
    sf::Text flavorText;
    sf::Sprite largeSprite; 

    // 選択枠
    sf::RectangleShape highlight;
};
