#pragma once

#include "Player.h"
#include "Level.h"
#include "SFML\Audio.hpp"


class Engine 
{
public:
	Engine();
	~Engine(void);

	void runEngine(sf::RenderWindow &window, sf::Sprite &Background, sf::Sprite &sound_button, int selected_lvl, sf::Music &music, bool &soudStatus, sf::Text &text);
	bool colision(sf::Vector2i xy);
	void water(sf::Vector2i xy);
	void update(sf::Vector2i xy);
	void color(sf::RenderWindow &win);
	bool block(sf::Vector2i xy);
	void animation();

private:
	Player player;
	Level level;

	sf::RectangleShape color_border;
	bool teleport_sound, water_sound, game_status, sound_status; 
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
	sf::Sprite tiled[Level::COUNT], comunicat_window, menu_button, restart_button;

};

