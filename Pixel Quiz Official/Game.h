#pragma once

#include "Engine.h"

class Game
{
public:
	Game();
	~Game();
	void runGame();

	sf::RenderWindow window;

private:
	enum GameState { MENU, GAME, LVL, SAVE, END };
	GameState state;

	int width, height;

	int selected_lvl;
	sf::Text text[3];

	bool soundStatus;
	Level level;
	sf::Music music;
	sf::Text headline;
	sf::Font font;
	sf::Texture Background, Sound_button;
	sf::Sprite background, sound_button;

	void menu();
	void single();
	void lvl();
	void save();

	Engine engine;
};

