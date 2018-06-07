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

	sf::Uint8 randUint8();

	bool loadMap(int lvl);

	void loadSave();

	void saveSave();

	
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



private:
	struct DoubleVector2i
	{
		int id0, id1;
		sf::Vector2i pos1, pos0;
		sf::Color color;
	};

	DoubleVector2i dk, tp;

	struct Dane
	{
		std::vector<DoubleVector2i> DK, TP; 
		sf::Vector2i xy;
		int coin_min;
	};

	std::fstream file_f;
	std::ofstream file_o;

	int lvl_to_save, key_nr, tp_nr;

public:

	const static int width_map = 32, height_map = 32, title_size = 16;

	int lvl, layer[height_map][width_map];

	Dane dane;

};

