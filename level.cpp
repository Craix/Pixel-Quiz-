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
			file_f >> dk.pos1.y;
			file_f >> dk.pos1.x;

			file_f >> dk.id1;
			file_f >> dk.pos0.y;
			file_f >> dk.pos0.x;

			dk.color = sf::Color(randUint8(), randUint8(), randUint8());
			dane.DK.push_back(dk);
		}

		file_f >> tp_nr;
		for (int i = 0; i < tp_nr; i++)
		{
			file_f >> tp.id0;
			file_f >> tp.pos1.y;
			file_f >> tp.pos1.x;

			file_f >> tp.id1;
			file_f >> tp.pos0.y;
			file_f >> tp.pos0.x;

			tp.color = sf::Color(randUint8(), randUint8(), randUint8());
			dane.TP.push_back(tp);
		}

		file_f >> dane.coin_min;

		for (int y = 0; y < height_map; y++)
		{
			for (int x = 0; x < width_map; x++)
			{
				file_f >> layer[x][y];

				if (layer[x][y] == SPAWN)
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

	if (!lvl >= lvl_to_save)
	{

		file_o.open("save.txt");

		if (!file_o.is_open())
		{
			MessageBox(NULL, "Failed to load save fille", "ERROR", NULL);
			return;
		}

		file_o << lvl_to_save << "\n";

		file_o.close();
	}

}

sf::Uint8 Level::randUint8()
{
	return rand() % 256;
}

