#pragma once
#include <fstream>
#include <Windows.h>
#include "SFML\Graphics.hpp"
#include <vector>

class Level
{
public:

	Level();

	~Level();

	int lose_rgb();

	bool loadMap(int lvl);

	void loadSave();

	void saveSave();

	int lvl;
	enum FieldType
	{
		NONE, // 0

		ICE, // 1
		SNOW, // 2
		MONEY, // 3
		WATER, // 4

		SPAWN, // 5
		FIRE, // 6

		KEY, // 7
		DOOR, // 8

		ACTIVE_TP, // 9
		BROKEN_TP, // 10

		PADLOCK, // 11

		COUNT,
	};

	FieldType Type;

	const static int width_map = 32, height_map = 32, title_size = 16;


private:
	struct DoubleVector2f
	{
		int id0, id1;
		sf::Vector2f val0, val1;
		sf::Color color;
	};

	DoubleVector2f dk, tp;

	struct Dane
	{
		std::vector<DoubleVector2f> DK, TP; 
		sf::Vector2f xy;
		int coin_min;
	};

	std::fstream file_f;
	std::ofstream file_o;

	int lvl_to_save, key_nr, tp_nr;

public:
	Dane dane;
	int poziom[height_map][width_map];

};

