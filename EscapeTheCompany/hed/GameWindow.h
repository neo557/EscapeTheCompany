#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Camera.h"
#include "TileMap.h"
#include "Scene.h"
#include "SceneManager.h"
#include "TitleScene.h"

class GameWindow {
public:
	sf::RenderWindow window;
	Player player;
	Camera camera;
	TileMap* tilemap;
	SceneManager sceneManager;

	GameWindow();
	void run();
	void handleEvents();
	void update(float dt);
	void draw();
};