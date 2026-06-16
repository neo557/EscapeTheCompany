#include "ItemScene.h"
#include "SceneManager.h"
#include "SaveData.h"
#include "GameScene.h"
#include "GameScene2.h"
#include "ItemManager.h"


ItemScene::ItemScene(sf::RenderWindow* window, Player* player, ItemManager* itemManager) : windowRef(window), playerRef(player), itemManagerRef(itemManager) {
	window->clear(sf::Color(30, 30, 30));
}

void ItemScene::onEnter() {

}

void ItemScene::onExit() {

}

void ItemScene::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Enter) {

        int id = selectedItemId; // カーソルで選んだアイテム
        itemManagerRef->useItem(id, playerRef);
    }

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
		SaveData save;
		save.saveGame(playerRef);
		SceneManager::instance().popScene();
	}

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            if (selectedIndex > 0) selectedIndex--;
        }
        if (event.key.code == sf::Keyboard::Down) {
            if (selectedIndex < (int)itemList.size() - 1)selectedIndex++;
        }
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
        if (selectedItemId != -1) {
            itemManagerRef->useItem(selectedItemId, playerRef);
        }
    }
}

void ItemScene::update(float dt) {
	
}

void ItemScene::draw(sf::RenderWindow& window) {
    sf::Text t;
    font.loadFromFile("Fonts/KH-Dot-Kagurazaka-16.ttf");
    t.setFont(font);
    t.setFillColor(sf::Color::White);
    itemList.clear();
    float y = 100;
    int index = 0;

    for (auto& [id, count] : itemManagerRef->inventory) {
        if (count <= 0) continue;

        const ItemData* data = itemManagerRef->getItemData(id);
        if (!data) continue;

        sf::Text t;
        t.setFont(font);
        t.setFillColor(sf::Color::White);
        t.setString(data->name + L" x" + std::to_wstring(count));
        t.setPosition(100, y);
        window.draw(t);

        y += 40;
    }

    if (!itemList.empty()) {
        selectedItemId = itemList[selectedIndex];
    }
}