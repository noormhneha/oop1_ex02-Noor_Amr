#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <windows>

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

// ANSI escape codes for text color
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define BROWN   "\033[0;33m"
#define KEY     "\033[1;36m"

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
	void printStep(const char c, const std::string& color);


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

