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
	Board(int level, std::ifstream& input);

	
	// get and set function for the members
	std::vector<std::string>& getMap();
	std::string& getMapIndex(const size_t& Index);
	
	std::vector<Cat> getCat()const;
	std::vector<Cat>& setCat();
	Mouse getMouse() const;
	Mouse& setMouse();

	Location getMouseLocation() const;

	size_t getMapRowSize()const;
	void setMapRowSize(const size_t& size);
	size_t getMapColSize()const;
	void setMapColSize(const size_t& size);

	int cheeseCounter() const;

	//Functions useful
	Location SetStartPos();
	void SetCell(Location cell, char c);

	bool isEaten(int level);
	/*void printBoard(int level);*/
	void printStep(const char c);


private:
	std::vector<std::string> m_map;
	Mouse m_mouse;
	std::vector<Cat> m_cat;
	
	Location m_mouseLocation;

	size_t m_mapRowSize;
	size_t m_mapColSize;

	int m_cheese;

	int m_level;
	void CreateFiguresOfCat(int level);
};

