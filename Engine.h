#pragma once

#include "Player.h"
#include "Level.h"
#include "SFML\Audio.hpp"


class Engine 
{
public:
	Engine(sf::RenderWindow &window, sf::Sprite &Background, sf::Font &font, int selected_lvl, sf::Sprite &sound_button, sf::Music &musicc);
	~Engine(void);

	void runEngine(sf::RenderWindow &window, sf::Sprite &Background, sf::Font &font, sf::Sprite &sound_button);
	bool colision(sf::Vector2f xy);
	void water(sf::Vector2f xy);
	void update(sf::Vector2f xy);
	void color(sf::RenderWindow &win);
	bool block(sf::Vector2f xy);
	void animation();

private:
	Player player;
	Level level;

	sf::Text text;
	sf::Music &music;

	sf::RectangleShape race;

	bool tp, wat, game_status;

	sf::SoundBuffer buffer[4];
	sf::Sound sound_effect[4];

	enum sounds__effects
	{
		coin,
		fire,
		teleport,
		waterr
	};

	sf::Clock clock;
	sf::Time time;

	int money_animation_frame;

	sf::Texture tiled_set, level_set;
	sf::Sprite tiled[Level::COUNT], win, menu, restart;

};
