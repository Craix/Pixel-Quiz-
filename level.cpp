#include "Level.h"

Level::Level()
{

};
Level::~Level()
{

}
bool Level::loadMap(int selected_lvl)
{

	if (selected_lvl == 0)
	{

		file_f.open("maps//" + std::to_string(lvl) + ".txt");
	}
	else 
	{

		file_f.open("maps//" + std::to_string(selected_lvl - 1) + ".txt");
		lvl = selected_lvl - 1;
	}



	if (!file_f.is_open())
	{
		return false; 
	}
	else
	{
		dane.DK.clear();
		dane.TP.clear();



		file_f >> key_nr;

		for (int i = 0; i < key_nr; i++)
		{
			file_f >> dk.id0;
			file_f >> dk.val0.y;
			file_f >> dk.val0.x;

			file_f >> dk.id1;
			file_f >> dk.val1.y;
			file_f >> dk.val1.x;

			dk.color = sf::Color(lose_rgb(), lose_rgb(), lose_rgb());
			dane.DK.push_back(dk);
		}

		file_f >> tp_nr;
		for (int i = 0; i < tp_nr; i++)
		{
			file_f >> tp.id0;
			file_f >> tp.val0.y;
			file_f >> tp.val0.x;

			file_f >> tp.id1;
			file_f >> tp.val1.y;
			file_f >> tp.val1.x;

			tp.color = sf::Color(lose_rgb(), lose_rgb(), lose_rgb());
			dane.TP.push_back(tp);
		}

		file_f >> dane.coin_min;

		for (int y = 0; y < height_map; y++)
		{
			for (int x = 0; x < width_map; x++)
			{
				file_f >> poziom[x][y];

				if (poziom[x][y] == SPAWN)
				{
					dane.xy = sf::Vector2i(8 + (x * 16),8 + (y * 16) + 64);
				}
			}
		}

		file_f.close();
	}
	
	return true; 

}

void Level::loadSave()
{
	lvl = 0;

	file_f.open("save.txt");
	
	if (!file_f.is_open())
	{
		MessageBox(NULL, "Failed to load save fille", "ERROR", NULL);
		return;
	}
	else
	{
		file_f >> lvl;
	}

	file_f.close();
}

void Level::saveSave()
{
	lvl_to_save = lvl;

	loadSave();

	if (lvl >= lvl_to_save)
	{

	}
	else
	{

		file_o.open("save.txt");

		if (!file_o.is_open())
		{
			MessageBox(NULL, "Failed to load save fille", "ERROR", NULL);
			return;
		}

		file_o << lvl_to_save << std::endl;

		file_o.close();
	}

}

int Level::lose_rgb()
{
	return rand() % 257;
}

