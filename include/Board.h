#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Cat.h"
#include "Mouse.h"
#include "Level.h"
#include "Cheese.h"

const char MOUSE = '%';
const char CAT = '^';
const char DOOR = 'D';
const char WALL = '#';
const char KEY = 'F';
const char CHEESE = '*';
const char GIFT = '$';
const char ROAD = ' ';

class Board
{
public:
	Board(int level, std::ifstream input);

	
	// get and set function for the members
	std::vector<std::string>& getMap();
	std::string& getMapIndex(const size_t& Index);
	
	std::vector<Cat> getCat()const;
	std::vector<Cat>& setCat();
	Mouse getMouse() const;
	Mouse& setMouse();

	size_t getMapRowSize()const;
	void setMapRowSize(const size_t& size);
	size_t getMapColSize()const;
	void setMapColSize(const size_t& size);

	//Functions useful
	void SetStartPos(int level);
	void SetCell(Location cell, char c);


private:
	std::vector<std::string> m_map;
	Mouse m_mouse;
	std::vector<Cat> m_cat;
	
	size_t m_mapRowSize;
	size_t m_mapColSize;

	Cheese m_cheese;

	void CreateFiguresOfCat(int level);
	int Distance(Location a, const int& row, const int& col);
	bool ValidPos(const char& pos);
	Location FindNearestPoint(const int& row, const int& col);
};

