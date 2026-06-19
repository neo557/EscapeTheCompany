#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <string>
#include "EnemyManager.h"

struct DialogueLine {
    std::string id;
    int speaker = 0;
    int enemyId = -1;
    std::wstring text;
    std::string photo;
    std::string emotion;
    std::string next;
};

class TextLayer {
public:
    TextLayer();
    void init(const std::string& csvPath, sf::Font* font, EnemyManager* em);
    void update(float dt);
    void handleEvent(const sf::Event& event);
    void draw(sf::RenderWindow& window);
    std::wstring utf8_to_wstring(const std::string& utf8);
    bool isFinished() const { return finished; }

private:
    std::unordered_map < std::string, DialogueLine > lines;
    std::string currentId;
    int currentLine = 0;
    sf::RectangleShape windowBox;
    sf::Text nameText;
    sf::Text bodyText;
    EnemyManager* enemyManager = nullptr;

    bool typing = false;
    float typeTimer = 0.f;
    int visibleChars = 0;

    bool finished = false;
};
