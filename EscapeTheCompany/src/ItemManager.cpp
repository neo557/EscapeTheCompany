#include "ItemManager.h"
#include "ItemData.h"
#include "Player.h"
#include <fstream>
#include <sstream>

ItemManager::ItemManager() {

}

void ItemManager::LoadItemDataFromCSV(const std::string& path) {
	std::fstream file(path);
    if (!file.is_open()) {
        printf("Failed to open %s\n", path.c_str());
        return;
    }
    else {
        printf("Success to open %s\n", path.c_str());
    }

    std::string line;
    std::getline(file, line); // ヘッダー行を読み飛ばす

    while (std::getline(file, line)) {

        if (line.empty()) continue;

        // CSVの各行を解析して ItemData を作成
        std::stringstream ss(line);
        std::string token;
        ItemData data;

        std::getline(ss, token, ','); data.id = std::stoi(token);
        std::getline(ss, data.name, ',');
        std::getline(ss, data.photo, ',');
        std::getline(ss, data.effect, ',');
        std::getline(ss, token, ','); data.value = std::stoi(token);
        std::getline(ss, token, ','); data.max = std::stoi(token);
        std::getline(ss, data.target, ',');
        std::getline(ss, data.description, ',');
        std::getline(ss, data.usable, ',');
        std::getline(ss, data.type, ',');

        itemDatabase[data.id] = data;
        inventory[data.id] = 0;
    }
}

bool ItemManager::useItem(int id, Player* player) {
    if (inventory[id] <= 0) return false;
    const ItemData& data = itemDatabase[id];

    //アイテムでの回復処理
    if (data.effect == "Heal") {
        player->statusManager->hp += data.value;
        if (player->statusManager->hp > player->statusManager->maxHp)
            player->statusManager->hp = player->statusManager->maxHp;
    }
    //バフ・攻撃などは後で追加

    inventory[id]--;
    return true;
};

void ItemManager::addItem(int id, int amount) {
    inventory[id] += amount;
    if (inventory[id] > itemDatabase[id].max) {
        inventory[id] = itemDatabase[id].max;
    }
};

const ItemData* ItemManager::getItemData(int id) const {
    auto it = itemDatabase.find(id);
    if (it != itemDatabase.end()) return &it->second;
    return nullptr;
}

int ItemManager::getItemCount(int id) const {
    auto it = inventory.find(id);
    if (it != inventory.end()) return it->second;
    return 0;
}