#pragma once

#include "SFML\Graphics.hpp"
#include <Windows.h>

class Player
{
private:
	sf::Texture texture;
	sf::Sprite sprite;
	int player_animation_frame;

public:
	Player();
	~Player();
	int coins;
	void Respawn(sf::Vector2f xy); 
	void Move(sf::Vector2f xy); 
	void Draw(sf::RenderWindow &win);
	sf::Vector2f getPosition();
	void animation();
};
