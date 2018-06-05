#pragma once

#include "SFML\Graphics.hpp"
#include <Windows.h>

class Player
{
private:
	sf::Texture player_texture;
	sf::Sprite player_sprite;
	int player_animation_frame;

public:
	Player();
	~Player();
	int coins;
	void Respawn(sf::Vector2i xy); 
	void Move(sf::Vector2i xy); 
	void Draw(sf::RenderWindow &win);
	sf::Vector2i getPosition();
	void animation();
};
