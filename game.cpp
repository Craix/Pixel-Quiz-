#include "Game.h"


sf::RenderWindow window(sf::VideoMode(512, 576), "Pixel Quiz");

Game::Game()
{
	state = END;

	if (!font.loadFromFile("graphic//Font.ttf"))
	{
		MessageBox(NULL, "Failed to load font.ogg", "ERROR", NULL);
		return;
	}
	if (!Background.loadFromFile("graphic//background.png"))
	{
		MessageBox(NULL, "Failed to load image background.png", "ERROR", NULL);
		return;
	}

	if (!music.openFromFile("sounds//Pixel.ogg"))
	{
		MessageBox(NULL, "Failed to load sounds Pixel.ogg", "ERROR", NULL);
		return;
	}

	if (!Sound_button.loadFromFile("graphic//sound_button.png"))
	{
		MessageBox(NULL, "Failed to load image sound_button.png", "ERROR", NULL);
		return;
	}

	sound_button.setTexture(Sound_button);
	sound_button.setTextureRect(sf::IntRect(0, 0, 32, 32));
	sound_button.setOrigin(16, 16);
	sound_button.setPosition(480, 32);

	music.play();
	music.setLoop(true);

	background.setTexture(Background);

	name.setFont(font);
	name.setCharacterSize(50);
	name.setString("Pixel & Quiz");
	name.setStyle(sf::Text::Bold);
	name.setFillColor(sf::Color::Color(255, 153, 0)); // #ff9900

	soundStatus = true;

	wybrany_lvl = 0;

	state = MENU;
}
Game::~Game()
{
}
void Game::runGame()
{
	while (state != END)
	{
		switch (state)
		{
		case GameState::MENU:
		{
			menu();
			break;
		}
		case GameState::GAME:
		{
			single();
			break;
		}
		case GameState::LVL:
		{
			lvl();
			break;
		}
		case GameState::SAVE:
		{
			save();
			break;
		}
		}
	}
}
void Game::menu()
{
	 width = window.getSize().x;
	 height = window.getSize().x;

	name.setPosition(width / 2 - name.getGlobalBounds().width / 2, 0);

	std::string str[] = { "Play","LvL", "Save & Exit" };

	for (int i = 0; i < 3; i++)
	{
		text[i].setFont(font);
		text[i].setCharacterSize(40);

		text[i].setString(str[i]);
		text[i].setPosition((float)width / 2 - text[i].getGlobalBounds().width / 2,(float)128 + i * ((height - 64) / 3));
	}

	while (state == MENU)
	{
		sf::Vector2f mouse(sf::Mouse::getPosition(window));
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed &&
				event.key.code == sf::Keyboard::Escape)
				state = END;

			else if (text[0].getGlobalBounds().contains(mouse) &&
				event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
			{
				state = GAME;
			}

			else if (text[1].getGlobalBounds().contains(mouse) &&
				event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
			{
				state = LVL;
			}

			else if (text[2].getGlobalBounds().contains(mouse) &&
				event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
			{
				state = END;
			}

			else if (sound_button.getGlobalBounds().contains(mouse) &&
				event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
			{
				soundStatus = !soundStatus;
				if (soundStatus)
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

		}

		for (int i = 0; i < 3; i++)
		{
			if (text[i].getGlobalBounds().contains(mouse))
			{
				text[i].setFillColor(sf::Color::Color(255, 153, 0));
			}
			else
			{
				text[i].setFillColor(sf::Color::White);
			}
		}

		window.clear();
		window.draw(background);
		window.draw(name);

		for (int i = 0; i<3; i++)
		{
			window.draw(text[i]);
		}

		window.draw(sound_button);
		window.display();
	}
}
void Game::single()
{
	Engine engine(window, background, font, wybrany_lvl, sound_button, music);

	state = MENU;
}
void Game::lvl()
{
#pragma region Texture_load

	sf::Texture Open;

	if (!Open.loadFromFile("graphic//lvl.png"))
	{
		MessageBox(NULL, "Failed to load image level.png", "ERROR", NULL);
		state = MENU;
	}

	sf::Text lvl_numbers;
	lvl_numbers.setFont(font);
	lvl_numbers.setCharacterSize(50);
	lvl_numbers.setStyle(sf::Text::Bold);
	lvl_numbers.setFillColor(sf::Color::Color(255, 153, 0)); // #ff9900

	level.loadSave();

#pragma endregion

	std::vector <sf::Sprite> lvl_field;
	lvl_field.resize(16);

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			lvl_field[y * 4 + x].setPosition((float)64 + 128 * x, (float) 128 + 128 * y);
			lvl_field[y * 4 + x].setTexture(Open);
		}
	}

	while (state == LVL)
	{
		sf::Vector2f mouse(sf::Mouse::getPosition(window));
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)
			{
				state = MENU;
			}

			for (int i = 0; i < level.lvl + 1; i++)
			{
				if (lvl_field[i].getGlobalBounds().contains(mouse) &&
					event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
				{

					wybrany_lvl = i + 1;
					state = GAME;
				}
			}
			
			if ((sound_button.getGlobalBounds().contains(mouse) && 
				event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left))
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
		}

		window.clear(sf::Color::Black);
		window.draw(background);
		window.draw(name);
		window.draw(sound_button);

		for (int i = 0; i < 16; i++)
		{
			lvl_field[i].setOrigin(32, 32);

			if ((level.lvl + 1)>i)
			{
				lvl_numbers.setString(std::to_string(i));
			}
			else
			{
				lvl_numbers.setString("?");
			}

			lvl_numbers.setOrigin(lvl_numbers.getLocalBounds().width / 2, lvl_numbers.getLocalBounds().top + lvl_numbers.getGlobalBounds().height / 2);

			lvl_numbers.setPosition(lvl_field[i].getPosition());

			window.draw(lvl_field[i]);
			window.draw(lvl_numbers);
		}

		window.display();
	}

}
void Game::save()
{
	level.saveSave();

	state = END;

}
