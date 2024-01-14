#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class Board;

class Level {
public:
	Level(std::ifstream& lv);
	std::ifstream& whichLevel();

private:
	std::ifstream& m_level;
	Board& m_board;
};