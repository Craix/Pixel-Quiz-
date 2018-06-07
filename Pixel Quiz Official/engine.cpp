#include "Engine.h"

Engine::Engine()
{ 
	if (!tiled_set.loadFromFile("graphic//tiled.png"))
	{
		MessageBox(NULL, "Failed to load image tiled_set.png", "ERROR", NULL);
		return;
	}

	for (int i = 0; i < level.COUNT; i++)
	{
		tiled[i].setTexture(tiled_set);
		tiled[i].setTextureRect(sf::IntRect(i * level.title_size, 0, level.title_size, level.title_size));
	}

	tiled[level.SNOW].setTextureRect(sf::IntRect(0, 16, level.title_size, level.title_size));  

	for (int i = 0; i < 4; i++)
	{
		if (!buffer[i].loadFromFile("sounds//"+ std::to_string(i) + ".wav"))
		{
			MessageBox(NULL, "Failed to load sounds effect vv", "ERROR", NULL);
			return;
		}

		sound_effect[i].setBuffer(buffer[i]);
	}


	if (!level_set.loadFromFile("graphic//level.png"))
	{
		MessageBox(NULL, "Failed to load image level_set.png", "ERROR", NULL);
		return;
	}

	comunicat_window.setTexture(level_set);
	comunicat_window.setTextureRect(sf::IntRect(32, 0, 128, 96));
	comunicat_window.setOrigin(64, 48);
	comunicat_window.setPosition(256, 288);

	menu_button.setTexture(level_set);
	menu_button.setTextureRect(sf::IntRect(0, 0, 32, 32));
	menu_button.setOrigin(32, 32);
	menu_button.setPosition(304, 320);

	restart_button.setTexture(level_set);
	restart_button.setTextureRect(sf::IntRect(0, 32, 32, 32));
	restart_button.setOrigin(32, 32);
	restart_button.setPosition(240, 320); 

}

Engine::~Engine() {};

void Engine::runEngine(sf::RenderWindow &window, sf::Sprite &Background, sf::Sprite &sound_button, int selected_lvl, sf::Music &music, bool &soundStatus, sf::Text &text)
{
	teleport_sound = false;
	water_sound = false;

	sound_status = soundStatus;

	money_animation_frame = 0;

	text.setPosition(0, 0);

	level.loadSave();

	if (!level.loadMap(selected_lvl))
	{
		MessageBox(NULL, "Faile do load map file", "ERROR", NULL);
		return;
	}

	player.coins = 0;

	player.Respawn(level.dane.xy);

	game_status = true;

	while (game_status)
	{
		time = clock.getElapsedTime();

		if (time.asSeconds() > 1)
		{
			player.animation();
			animation();
			clock.restart();
		}

		if (level.dane.coin_min == 0)
		{
			text.setString("Coins: " + std::to_string(player.coins)); 
		}
		else
		{
			text.setString("Coins: " + std::to_string(player.coins) + "/" + std::to_string(level.dane.coin_min)); 
		}

		#pragma region movment

		sf::Vector2f mouse(sf::Mouse::getPosition(window));

		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)
			{
				game_status = !game_status;
			}
			if ((sound_button.getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left))
			{
				sound_status = !sound_status;

				if (sound_status)
				{
					sound_button.setTextureRect(sf::IntRect(0, 0, 32, 32));
					music.play();
				}
				else
				{
					sound_button.setTextureRect(sf::IntRect(32, 0, 32, 32));
					music.pause();
				}
				
			}

			if (!block(player.getPosition()))
			{
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)
				{
					if (!colision(player.getPosition() + sf::Vector2i(0, -1))) 
					{
						teleport_sound = false;
						water(player.getPosition());
						player.Move(sf::Vector2i(0, -16));
					}
				}
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
				{
					if (!colision(player.getPosition() + sf::Vector2i(0, 1))) 
					{
						teleport_sound = false;
						water(player.getPosition());
						player.Move(sf::Vector2i(0, 16));
					}
				}
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left)
				{
					if (!colision(player.getPosition() + sf::Vector2i(-1, 0))) 
					{
						teleport_sound = false;
						water(player.getPosition());
						player.Move(sf::Vector2i(-16, 0));
					}
				}
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right)
				{
					if (!colision(player.getPosition() + sf::Vector2i(1, 0))) 
					{
						teleport_sound = false;
						water(player.getPosition());
						player.Move(sf::Vector2i(16, 0));
					}
				}
			}

			else
			{
				if ((menu_button.getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left))
				{
					game_status = !game_status;
				}
				if ((restart_button.getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left))
				{
					if (!level.loadMap(level.lvl + 1))
					{
						MessageBox(NULL, "Faile do load map file 2", "ERROR", NULL);
						return; 
					}

					player.coins = 0;
					player.Respawn(level.dane.xy);
				}
			}
		}

		update(player.getPosition());

		#pragma endregion

		#pragma region draw stuff

		window.clear();
		window.draw(Background);
		window.draw(text);
		window.draw(sound_button);
		
		color(window);

		for (int i = 0; i < level.height_map; i++)
		{
			for (int j = 0; j < level.width_map; j++)
			{

				if (level.layer[i][j] == level.NONE)
				{
				}
				else if (level.layer[i][j] == level.PADLOCK)
				{
					if (level.dane.coin_min <= player.coins)
					{
						level.layer[i][j] = level.FIRE; 
					}

					tiled[level.layer[i][j] - 1].setPosition(sf::Vector2f(sf::Vector2i(level.title_size * i, level.title_size * j + 64)));
					window.draw(tiled[level.layer[i][j] - 1]);
				}
				else if (level.layer[i][j] == level.WATER)
				{						
					tiled[level.layer[i][j] - 1].setPosition(sf::Vector2f(sf::Vector2i(level.title_size * i, level.title_size * j + 64)));
					window.draw(tiled[level.layer[i][j] - 1]);
				}
				else
				{
					tiled[level.layer[i][j] - 1].setPosition(sf::Vector2f(sf::Vector2i(level.title_size * i, level.title_size * j + 64)));
					window.draw(tiled[level.layer[i][j] - 1]);
				}
			}
		}

		

		player.Draw(window);

		if (block(player.getPosition()))
		{
			window.draw(comunicat_window);
			window.draw(menu_button);
			window.draw(restart_button);
		}

		window.display();

		#pragma endregion

	}

	level.saveSave();

}
bool Engine::colision(sf::Vector2i xy)
{
	return (level.layer[xy.x][xy.y] == level.ICE || level.layer[xy.x][xy.y] == level.PADLOCK
		|| level.layer[xy.x][xy.y] == level.DOOR);
}
void Engine::water(sf::Vector2i xy)
{
	if (level.layer[xy.x][xy.y] == level.SNOW)
	{
		level.layer[xy.x][xy.y] = level.WATER;
	}
}
void Engine::update(sf::Vector2i xy)
{
	#pragma region next_lvl

	if (level.layer[xy.x][xy.y] == level.FIRE)
	{
		level.lvl++;
		if (!level.loadMap(level.lvl + 1))
		{
			MessageBox(NULL, "Faile do load map file 3", "ERROR", NULL);
			return;
		}

		player.coins = 0;
		player.Respawn(level.dane.xy);
		if (sound_status)
		{
			sound_effect[fire].play();
		}	
	}

	#pragma endregion

	#pragma region coin

	if (level.layer[xy.x][xy.y] == level.MONEY)
	{
		level.layer[xy.x][xy.y] = level.SNOW;
		player.coins++;
		if (sound_status)
		{
			sound_effect[coin].play();
		}
	}

	#pragma endregion

	#pragma region door_key


	if (level.layer[xy.x][xy.y] == level.KEY)
	{
		for (size_t i = 0; i < level.dane.DK.size(); i++)
		{
			if (level.dane.DK[i].pos1 == sf::Vector2i(xy.x, xy.y))
			{
				if (level.layer[level.dane.DK[i].pos0.x][level.dane.DK[i].pos0.y] == level.DOOR)
				{
					if (sound_status)
					{
						sound_effect[coin].play();
					}
					level.layer[level.dane.DK[i].pos0.x][level.dane.DK[i].pos0.y] = level.MONEY;
					level.layer[level.dane.DK[i].pos1.x][level.dane.DK[i].pos1.y] = level.MONEY;
				}
			}
		}
	}


	#pragma endregion

	#pragma region teleport

	if (level.layer[xy.x][xy.y] == level.ACTIVE_TP) 
	{


		for (size_t i = 0; i < level.dane.TP.size(); i++)
		{	
			if (level.dane.TP[i].pos1 == sf::Vector2i(xy.x, xy.y))  
			{
				if (level.layer[level.dane.TP[i].pos0.x][level.dane.TP[i].pos0.y] == level.ACTIVE_TP || 
					level.layer[level.dane.TP[i].pos0.x][level.dane.TP[i].pos0.y] == level.BROKEN_TP)
				{

					if (!teleport_sound)
					{
						player.Respawn(sf::Vector2i(level.dane.TP[i].pos0.x * 16 + 8, level.dane.TP[i].pos0.y * 16 + 8 + 64)); 
						teleport_sound = true; 

						if (sound_status)
						{
							sound_effect[teleport].play();
						}
					}
				}
			}

			if (level.dane.TP[i].pos0 == sf::Vector2i(xy.x, xy.y))
			{
				if (level.layer[level.dane.TP[i].pos1.x][level.dane.TP[i].pos1.y] == 9 ||
					level.layer[level.dane.TP[i].pos1.x][level.dane.TP[i].pos1.y] == 10)
				{
					if (!teleport_sound)
					{
						player.Respawn(sf::Vector2i(level.dane.TP[i].pos1.x * 16 + 8, level.dane.TP[i].pos1.y * 16 + 8 + 64));
						teleport_sound = true; 


						if (sound_status)
						{
							sound_effect[teleport].play();
						}
					}

				}
			}
		}
	}

	#pragma endregion
}
bool Engine::block(sf::Vector2i xy)
{
	if (level.layer[xy.x][xy.y] == level.WATER)
	{
		if (sound_status && !water_sound)
		{
			sound_effect[waterr].play();
			water_sound = !water_sound;
		}

		return true;
	}
	else
	{
		water_sound = false;
		return false;
	}
}
void Engine::color(sf::RenderWindow &win)
{
	for (size_t i = 0; i < level.dane.DK.size(); i++)
	{
	
		color_border.setFillColor(level.dane.DK[i].color);
		color_border.setSize(sf::Vector2f(16, 16));

		color_border.setPosition(sf::Vector2f(sf::Vector2i(level.dane.DK[i].pos1.x * 16, level.dane.DK[i].pos1.y * 16 + 64)));
		win.draw(color_border);

		color_border.setPosition(sf::Vector2f(sf::Vector2i(level.dane.DK[i].pos0.x * 16, level.dane.DK[i].pos0.y * 16 + 64)));
		win.draw(color_border);
	}

	for (size_t j = 0; j < level.dane.TP.size(); j++)
	{
		color_border.setFillColor(level.dane.TP[j].color);
		color_border.setSize(sf::Vector2f(16, 16));

		color_border.setPosition(sf::Vector2f(sf::Vector2i(level.dane.TP[j].pos1.x * 16, level.dane.TP[j].pos1.y * 16 + 64)));
		win.draw(color_border);

		color_border.setPosition(sf::Vector2f(sf::Vector2i(level.dane.TP[j].pos0.x * 16, level.dane.TP[j].pos0.y * 16 + 64)));
		win.draw(color_border);
	}

}

void Engine::animation()
{
	switch (money_animation_frame)
	{
	case 0:
	{
		tiled[2].setTextureRect(sf::IntRect(0, 16, 16, 16));
		break;
	}
	case 1:
	{
		tiled[2].setTextureRect(sf::IntRect(16, 16, 16, 16));
		break;
	}
	case 2:
	{
		tiled[2].setTextureRect(sf::IntRect(32, 16, 16, 16));
		break;
	}
	case 3:
	{
		tiled[2].setTextureRect(sf::IntRect(48, 16, 16, 16));
		money_animation_frame = -1;
		break;
	}
	}

	money_animation_frame++;
}



