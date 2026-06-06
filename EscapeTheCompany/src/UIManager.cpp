#include "UIManager.h"
#include <fstream>
#include <json.hpp>
#include <iostream>

using json = nlohmann::json;

bool UIManager::load(const std::string& atlasPath, const std::string& jsonPath) {
    if (!atlas.loadFromFile(atlasPath)) {
        std::cout << "Failed to load atlas: " << atlasPath << std::endl;
        return false;
    }

    std::ifstream file(jsonPath);
    if (!file.is_open()) {
        std::cout << "Failed to open JSON: " << jsonPath << std::endl;
        return false;
    }

    json j;
    try {
        file >> j;
    }
    catch (std::exception& e) {
        std::cout << "JSON parse error: " << e.what() << std::endl;
        return false;
    }

    if (!j.contains("frames")) {
        std::cout << "JSON missing 'frames' field" << std::endl;
        return false;
    }

    for (auto& item : j["frames"]) {
        std::string name = item["filename"];
        auto frame = item["frame"];

        rects[name] = sf::IntRect(
            frame["x"], frame["y"],
            frame["w"], frame["h"]
        );
    }

    std::cout << "UIAtlas loaded successfully. Rect count = " << rects.size() << std::endl;

    return true;
}


sf::IntRect UIManager::getRect(const std::string& name) const {
    return rects.at(name);
}