#include "ItemManager.h"
#include "ItemData.h"
#include "Player.h"
#include <fstream>
#include <sstream>


ItemManager::ItemManager() {

}

std::wstring ItemManager::utf8_to_wstring(const std::string& utf8)
{
    if (utf8.empty()) return L"";

    int size_needed = MultiByteToWideChar(
        CP_UTF8, 0,
        utf8.c_str(), (int)utf8.size(),
        NULL, 0
    );

    std::wstring result(size_needed, 0);

    MultiByteToWideChar(
        CP_UTF8, 0,
        utf8.c_str(), (int)utf8.size(),
        &result[0], size_needed
    );

    return result;
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

    //BOM除去
    if (line.size() >= 3 &&
        (unsigned char)line[0] == 0xEF &&
        (unsigned char)line[1] == 0xBB &&
        (unsigned char)line[2] == 0xBF) {
        line = line.substr(3);
    }

    while (std::getline(file, line)) {

        if (line.empty()) continue;

        // CSVの各行を解析して ItemData を作成
        std::stringstream ss(line);
        std::string token;
        ItemData data;

        std::getline(ss, token, ',');
        data.id = std::stoi(token);
        std::getline(ss, token, ',');
        data.name = this->utf8_to_wstring(token);   // UTF-8 → wstring
        std::getline(ss, token, ',');
        data.photo = token;
        std::getline(ss, token, ',');
        data.effect = token;
        std::getline(ss, token, ',');
        data.value = std::stoi(token);
        std::getline(ss, token, ',');
        data.max = std::stoi(token);
        std::getline(ss, token, ',');
        data.target = token;
        std::getline(ss, token, ',');
        data.description = this->utf8_to_wstring(token); // UTF-8 → wstring

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