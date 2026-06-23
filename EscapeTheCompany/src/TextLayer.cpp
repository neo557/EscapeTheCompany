#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <Windows.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "TextLayer.h"
#include "EnemyManager.h"
#include "SceneManager.h"

std::wstring TextLayer::utf8_to_wstring(const std::string& utf8)
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

TextLayer::TextLayer(){
}

void TextLayer::init(const std::string& csvPath, sf::Font* font, EnemyManager* em) {

	enemyManager = em;

	//移動入力のfalse
	SceneManager::instance().player->resetInput();
	//font設定
	nameText.setFont(*font);
	bodyText.setFont(*font);

	nameText.setCharacterSize(28);
	bodyText.setCharacterSize(24);

	//背景
	windowBox.setSize({ 1280.f,250.f });
	windowBox.setPosition(0.f, 470.f);
	windowBox.setFillColor(sf::Color(0, 0, 0, 180));	//半透明

	//csv読み込み
	std::ifstream file(csvPath);
	if (!file.is_open()) {
		printf("Failed to open CSV : %s\n", csvPath.c_str());
		finished = true;
		return;
	}
	std::string line;
	while (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string token;
		DialogueLine dl;

		std::getline(ss, dl.id, ',');
		ss >> dl.speaker; ss.ignore();
		ss >> dl.enemyId; ss.ignore();
		std::getline(ss,token, ',');
		dl.text= this->utf8_to_wstring(token);
		std::getline(ss, dl.photo, ',');
		std::getline(ss, dl.emotion, ',');
		std::getline(ss, dl.next, ',');

		lines[dl.id] = dl;
	}

	//初期IDの設定
	if (!lines.empty()) {
		currentId = lines.begin()->first;
	}
	else {
		finished = true;
	}

	typing = true;
	visibleChars = 0;
}

void TextLayer::update(float dt) {
	if (finished) return;

	if (typing) {
		typeTimer += dt;

		if (typeTimer > 0.03f) {
			typeTimer = 0.f;
			visibleChars++;
			if (visibleChars >= lines[currentId].text.size()) {
				typing = false;
			}
		}
	}
}

void TextLayer::handleEvent(const sf::Event& event) {
	if (finished) return;

	if (event.type == sf::Event::KeyPressed &&
		(event.key.code == sf::Keyboard::Enter)) {

		if (typing) {
			//前表示
			visibleChars = lines[currentId].text.size();
			typing = false;
		}
		else {
			std::string next = lines[currentId].next;
			auto trim = [](std::string& s) {
				s.erase(std::remove_if(s.begin(), s.end(),
					[](unsigned char c) { return std::isspace(c); }), s.end());
				};

			trim(next);
			if (next == "end" || next.empty()) {
				finished = true;
			}

			else {
				currentId = next;
				typing = true;
				visibleChars = 0;
			}
		}
	}
}

void TextLayer::draw(sf::RenderWindow& window) {
	if (finished) return;
	window.draw(windowBox);
	const DialogueLine& dl = lines[currentId];

	//name
	if (dl.speaker == 0) {
		nameText.setString("Player");
		nameText.setPosition(50, 480);
	}
	else if (dl.speaker == 1) {
		// Enemy の名前を取得
		if (enemyManager && dl.enemyId >= 0) {
			auto& data = enemyManager->enemyDatabase[dl.enemyId];
			nameText.setString(data.name);
		}
		else {
			nameText.setString("Enemy");
		}
		nameText.setPosition(1000, 480);
	}
	else {
		nameText.setString("");
	}
	window.draw(nameText);

	std::wstring visible = dl.text.substr(0, visibleChars);
	bodyText.setString(visible);
	bodyText.setPosition(50,530);

	window.draw(bodyText);
}