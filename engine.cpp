#include "Engine.h"


Engine::Engine(sf::RenderWindow &window, sf::Sprite &Background, sf::Font &font, int selected_lvl, sf::Sprite &sound_button, sf::Music &musicc) : music(musicc)
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

	win.setTexture(level_set);
	win.setTextureRect(sf::IntRect(32, 0, 128, 96));
	win.setOrigin(64, 48);
	win.setPosition(256, 288);

	menu.setTexture(level_set);
	menu.setTextureRect(sf::IntRect(0, 0, 32, 32));
	menu.setOrigin(32, 32);
	menu.setPosition(304, 320);

	restart.setTexture(level_set);
	restart.setTextureRect(sf::IntRect(0, 32, 32, 32));
	restart.setOrigin(32, 32);
	restart.setPosition(240, 320); 

	text.setCharacterSize(50);
	text.setFont(font);
	text.setStyle(sf::Text::Bold); 
	text.setFillColor(sf::Color::Color(255, 153, 0)); // #ff9900
	text.setPosition(0, 0);

	money_animation_frame = 0;

	tp = false;
	wat = false;

	level.loadSave();

	if (!level.loadMap(selected_lvl))
	{
		MessageBox(NULL, "Faile do load map file", "ERROR", NULL);
		return;
	}

	player.coins = 0;
	player.Respawn(level.dane.xy);

	runEngine(window, Background, font, sound_button);
}

Engine::~Engine() {};

void Engine::runEngine(sf::RenderWindow &window, sf::Sprite &Background, sf::Font &font, sf::Sprite &sound_button)
{
	game_status = true;

	while (game_status)
	{
		time = clock.getElapsedTime();

		if ((int)time.asSeconds() == 1)
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


				if (music.getStatus() == sf::Sound::Status::Paused)
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
					if (colision(player.getPosition() + sf::Vector2f(0, -1)) == false) 
					{
						tp = false;
						water(player.getPosition());
						player.Move(sf::Vector2f(0, -16));
					}
				}
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
				{
					if (colision(player.getPosition() + sf::Vector2f(0, 1)) == false) 
					{
						tp = false;
						water(player.getPosition());
						player.Move(sf::Vector2f(0, 16));
					}
				}
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left)
				{
					if (colision(player.getPosition() + sf::Vector2f(-1, 0)) == false) 
					{
						tp = false;
						water(player.getPosition());
						player.Move(sf::Vector2f(-16, 0));
					}
				}
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right)
				{
					if (colision(player.getPosition() + sf::Vector2f(1, 0)) == false) 
					{
						tp = false;
						water(player.getPosition());
						player.Move(sf::Vector2f(16, 0));
					}
				}
			}

			else
			{
				if ((menu.getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left))
				{
					game_status = !game_status;
				}
				if ((restart.getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left))
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

				if (level.poziom[i][j] == level.NONE)
				{
				}
				else if (level.poziom[i][j] == level.PADLOCK)
				{
					if (level.dane.coin_min <= player.coins)
					{
						level.poziom[i][j] = level.FIRE; 
					}

					tiled[level.poziom[i][j] - 1].setPosition((float)level.title_size * i, (float)level.title_size * j + 64);
					window.draw(tiled[level.poziom[i][j] - 1]);
				}
				else if (level.poziom[i][j] == level.WATER)
				{						
					tiled[level.poziom[i][j] - 1].setPosition((float)level.title_size * i, (float)level.title_size * j + 64);
					window.draw(tiled[level.poziom[i][j] - 1]);
				}
				else
				{
					tiled[level.poziom[i][j] - 1].setPosition((float)level.title_size * i, (float)level.title_size * j + 64);
					window.draw(tiled[level.poziom[i][j] - 1]);
				}
			}
		}

		

		player.Draw(window);

		if (block(player.getPosition()) == true)
		{
			window.draw(win);
			window.draw(menu);
			window.draw(restart);
		}

		window.display();

		#pragma endregion

	}

	level.saveSave();

}
bool Engine::colision(sf::Vector2f xy)
{
	if (level.poziom[(int)xy.x][(int)xy.y] == level.ICE || level.poziom[(int)xy.x][(int)xy.y] == level.PADLOCK 
		|| level.poziom[(int)xy.x][(int)xy.y] == level.DOOR)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void Engine::water(sf::Vector2f xy)
{
	if (level.poziom[(int)xy.x][(int)xy.y] == level.SNOW)
	{
		level.poziom[(int)xy.x][(int)xy.y] = level.WATER;
	}
}
void Engine::update(sf::Vector2f xy)
{
	#pragma region next_lvl

	if (level.poziom[(int)xy.x][(int)xy.y] == level.FIRE)
	{
		level.lvl++;
		if (!level.loadMap(level.lvl + 1))
		{
			MessageBox(NULL, "Faile do load map file 3", "ERROR", NULL);
			return;
		}

		player.coins = 0;
		player.Respawn(level.dane.xy);
		if (music.getStatus() == sf::Sound::Status::Playing)
		{
			sound_effect[fire].play();
		}	
	}

	#pragma endregion

	#pragma region coin

	if (level.poziom[(int)xy.x][(int)xy.y] == level.MONEY)
	{
		level.poziom[(int)xy.x][(int)xy.y] = level.SNOW;
		player.coins++;
		if (music.getStatus() == sf::Sound::Status::Playing)
		{
			sound_effect[coin].play();
		}
	}

	#pragma endregion

	#pragma region door_key


	if (level.poziom[(int)xy.x][(int)xy.y] == level.KEY)
	{
		for (size_t i = 0; i < level.dane.DK.size(); i++)
		{
			if (level.dane.DK[i].val0 == sf::Vector2f((float)xy.x, (float)xy.y))
			{
				if (level.poziom[(int)level.dane.DK[i].val1.x][(int)level.dane.DK[i].val1.y] == level.DOOR)
				{
					if (music.getStatus() == sf::Sound::Status::Playing)
					{
						sound_effect[coin].play();
					}
					level.poziom[(int)level.dane.DK[i].val1.x][(int)level.dane.DK[i].val1.y] = level.MONEY;
					level.poziom[(int)level.dane.DK[i].val0.x][(int)level.dane.DK[i].val0.y] = level.MONEY;
				}
			}
		}
	}


	#pragma endregion

	#pragma region teleport

	if (level.poziom[(int)xy.x][(int)xy.y] == level.ACTIVE_TP) 
	{


		for (size_t i = 0; i < level.dane.TP.size(); i++)
		{	
			if (level.dane.TP[i].val0 == sf::Vector2f((float)xy.x, (float)xy.y))  
			{
				if (level.poziom[(int)level.dane.TP[i].val1.x][(int)level.dane.TP[i].val1.y] == level.ACTIVE_TP || 
					level.poziom[(int)level.dane.TP[i].val1.x][(int)level.dane.TP[i].val1.y] == level.BROKEN_TP)
				{

					if (!tp)
					{
						player.Respawn(sf::Vector2f(level.dane.TP[i].val1.x * 16 + 8, level.dane.TP[i].val1.y * 16 + 8 + 64)); 
						tp = true; 

						if (music.getStatus() == sf::Sound::Status::Playing)
						{
							sound_effect[teleport].play();
						}
					}
				}
			}

			if (level.dane.TP[i].val1 == sf::Vector2f((float)xy.x, (float)xy.y))
			{
				if (level.poziom[(int)level.dane.TP[i].val0.x][(int)level.dane.TP[i].val0.y] == 9 ||
					level.poziom[(int)level.dane.TP[i].val0.x][(int)level.dane.TP[i].val0.y] == 10)
				{
					if (!tp)
					{
						player.Respawn(sf::Vector2f(level.dane.TP[i].val0.x * 16 + 8, level.dane.TP[i].val0.y * 16 + 8 + 64));
						tp = true; 


						if (music.getStatus() == sf::Sound::Status::Playing)
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
bool Engine::block(sf::Vector2f xy)
{
	if (level.poziom[(int)xy.x][(int)xy.y] == level.WATER)
	{
		if (music.getStatus() == sf::Sound::Status::Playing)
		{
			if (!wat)
			{
				sound_effect[waterr].play();
				wat = !wat;
			}
		}

		return true;
	}
	else
	{
		wat = false;
		return false;
	}
}
void Engine::color(sf::RenderWindow &win)
{
	for (size_t i = 0; i < level.dane.DK.size(); i++)
	{
	
		race.setFillColor(level.dane.DK[i].color);
		race.setSize(sf::Vector2f(16, 16));

		race.setPosition(sf::Vector2f(level.dane.DK[i].val0.x * 16, level.dane.DK[i].val0.y * 16 + 64));
		win.draw(race);

		race.setPosition(sf::Vector2f(level.dane.DK[i].val1.x * 16, level.dane.DK[i].val1.y * 16 + 64));
		win.draw(race);
	}

	for (size_t j = 0; j < level.dane.TP.size(); j++)
	{
		race.setFillColor(level.dane.TP[j].color);
		race.setSize(sf::Vector2f(16, 16));

		race.setPosition(sf::Vector2f(level.dane.TP[j].val0.x * 16, level.dane.TP[j].val0.y * 16 + 64));
		win.draw(race);

		race.setPosition(sf::Vector2f(level.dane.TP[j].val1.x * 16, level.dane.TP[j].val1.y * 16 + 64));
		win.draw(race);
	}

}

void Engine::animation()
{
	switch (money_animation_frame)
	{
	case 0:
		tiled[2].setTextureRect(sf::IntRect(0, 16, 16, 16));
		break;
	case 1:
		tiled[2].setTextureRect(sf::IntRect(16, 16, 16, 16));
		break;
	case 2:
		tiled[2].setTextureRect(sf::IntRect(32, 16, 16, 16));
		break;
	case 3:
	{
		tiled[2].setTextureRect(sf::IntRect(48, 16, 16, 16));
		money_animation_frame = 0;
		break;
	}
	}

	money_animation_frame++;
}


