#include "Player.h"


Player::Player()
{
	if (!texture.loadFromFile("graphic//player.png"))
	{
		MessageBox(NULL, "Failed to load image player.png", "ERROR", NULL);
		return;
	}

	player_animation_frame = 0;

	animation();

	sprite.setOrigin(8, 8);
	sprite.setTexture(texture);
}

Player::~Player() {};

void Player::Respawn(sf::Vector2f xy)
{
	sprite.setPosition(xy);

}
void Player::Move(sf::Vector2f xy)
{
	sprite.move(xy.x, xy.y);
}
void Player::Draw(sf::RenderWindow &win)
{
	win.draw(sprite);
}
sf::Vector2f Player::getPosition()
{
	return sf::Vector2f(((sprite.getPosition().x - 8) / 16), (((sprite.getPosition().y - 64) - 8) / 16));
}
void Player::animation()
{
	if (player_animation_frame == 0)
	{
		sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
	}
	if (player_animation_frame == 1)
	{
		sprite.setTextureRect(sf::IntRect(16, 0, 16, 16));
	}
	if (player_animation_frame == 2)
	{
		sprite.setTextureRect(sf::IntRect(0, 16, 16, 16));
	}
	if (player_animation_frame == 3)
	{
		sprite.setTextureRect(sf::IntRect(16, 16, 16, 16));
		player_animation_frame = -1;
	}

	player_animation_frame++;
}
