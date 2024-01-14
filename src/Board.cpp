#include "Board.h"
#include <cmath>

Board::Board(int level, std::ifstream& input) : m_mapColSize(0), m_mapRowSize(0)
{
	Level levelObj(input, *this);
	CreateFiguresOfCat(level);
	SetStartPos(level);
}

std::vector<std::string>& Board::getMap()
{
	return m_map;
}

std::string& Board::getMapIndex(const size_t& index)
{
	return m_map[index];
}



std::vector<Cat> Board::getCat() const
{
	return m_cat;
}

std::vector<Cat>& Board::setCat()
{
	return m_cat;
}

Mouse Board::getMouse() const
{
	return m_mouse;
}

Mouse& Board::setMouse()
{
	return m_mouse;
}

size_t Board::getMapRowSize() const
{
	return m_mapRowSize;
}

void Board::setMapRowSize(const size_t& size)
{
	m_mapRowSize = size;
}

size_t Board::getMapColSize() const
{
	return m_mapColSize;
}

void Board::setMapColSize(const size_t& size)
{
	m_mapColSize = size;
}

//=============================================================================

void Board::SetStartPos(int level)
{
	Location loc[4] = { { 0, 0 }, { 0, int(m_mapColSize - 1) },
					  { int(m_mapRowSize - 1), int(m_mapColSize - 1) },
					  { int(m_mapRowSize - 1), 0 } };

	for (int i = 0; i < level; i++)
		m_cat[i].setPosition(FindNearestPoint(loc[i % 4].row, loc[i % 4].col));

	Location mousePlace = FindNearestPoint(static_cast<int>(std::ceil(int(m_mapRowSize) / 2)),
		static_cast<int>(std::ceil(int(m_mapColSize) / 2)));

	m_mouse.setPosition(mousePlace);
	if (m_map[mousePlace.col][mousePlace.row] == CHEESE)
	{
		//m_cheese--;
		SetCell(mousePlace, ROAD);
	}

}

// set a cell to a received char in a received cell
void Board::SetCell(Location cell, char c)
{
	m_map[cell.col][cell.row] = c;
}

void Board::CreateFiguresOfCat(int level)
{
	for (int i = 0; i < level; i++)
		m_cat.push_back(Cat());
}


// return the distance between 2 cells in the board
int Board::Distance(Location a, const int& row, const int& col)
{
	return (abs(a.row - row) + abs(a.col - col));
}

// return if the cell is not a wall (#)
bool Board::ValidPos(const char& pos)
{
	return pos != WALL;
}

Location Board::FindNearestPoint(const int& row, const int& col)
{
	int currentDis = 0;
	int minDis = (int(m_mapColSize + m_mapRowSize)) * 3;
	Location currentPos;
	Location newPos = { row, col };
	for (int i = 0; i < m_mapColSize; i++)
		for (int j = 0; j < m_mapRowSize; j++)
		{
			currentPos = { j,i };
			currentDis = Distance(currentPos, row, col);
			if (ValidPos(m_map[i][j]) && currentDis < minDis)
			{
				minDis = currentDis;
				newPos = currentPos;
			}
		}
	return newPos;
}




