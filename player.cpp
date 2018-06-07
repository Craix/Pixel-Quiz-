#include "Player.h"


Player::Player()
{
	if (!player_texture.loadFromFile("graphic//player.png"))
	{
		MessageBox(NULL, "Failed to load image player.png", "ERROR", NULL);
		return;
	}

	player_animation_frame = 0;

	player_sprite.setOrigin(8, 8);
	player_sprite.setTexture(player_texture);
	animation(); 
}

Player::~Player() {};

void Player::Respawn(sf::Vector2i xy)
{
	player_sprite.setPosition(static_cast<sf::Vector2f>(xy));

}
void Player::Move(sf::Vector2i xy)
{
	player_sprite.move(static_cast<sf::Vector2f>(xy));
}
void Player::Draw(sf::RenderWindow &win)
{
	win.draw(player_sprite);
}
sf::Vector2i Player::getPosition()
{
	return sf::Vector2i(((static_cast<int>(player_sprite.getPosition().x) - 8) / 16), (((static_cast<int>(player_sprite.getPosition().y) - 64) - 8) / 16));
}
void Player::animation()
{
	if (player_animation_frame == 0)
	{
		player_sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
	}
	if (player_animation_frame == 1)
	{
		player_sprite.setTextureRect(sf::IntRect(16, 0, 16, 16));
	}
	if (player_animation_frame == 2)
	{
		player_sprite.setTextureRect(sf::IntRect(0, 16, 16, 16));
	}
	if (player_animation_frame == 3)
	{
		player_sprite.setTextureRect(sf::IntRect(16, 16, 16, 16));
		player_animation_frame = -1;
	}

	player_animation_frame++;
}
