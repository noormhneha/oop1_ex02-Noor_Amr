#include "Board.h"
#include <cmath>

Board::Board(int level, std::ifstream& input) : m_mapColSize(0), m_mapRowSize(0), m_cheese(0), m_level(level), m_mouseLocation{1,1}
{
	Level levelObj(input, *this); Cheese cheese(*this);
	m_cheese = cheese.getCheeseCounter();
	CreateFiguresOfCat(level);
	m_mouseLocation = SetStartPos();
	setMouse().setPosition(m_mouseLocation);
	catsPosition();
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

Location Board::getMouseLocation() const
{
	return m_mouseLocation;
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

int Board::cheeseCounter() const
{
	return m_cheese;
}

//=============================================================================

Location Board::SetStartPos()
{
	Location location{ 1,1 };
	int i = 0;
	for (const auto& line : m_map) {
		for (int j = 0; j < line.length(); j++) {
			if (line[j] == MOUSE) {
				location = { i,j };
				return location;
			}
		}
		i++;
	}
	return location;
}

// set a cell to a received char in a received cell
void Board::SetCell(Location cell, char c)
{
	m_map[cell.col][cell.row] = c;
}


void Board::printStep(const char c, const std::string& color) {
	std::cout << color << c << RESET;// << std::endl;
}

bool Board::isEaten(int level)
{
	for (int i = 0; i < level; i++)
	{
		if (m_mouse.getPosition().row == m_cat[i].getPosition().row &&
			m_mouse.getPosition().col == m_cat[i].getPosition().col)
			return true;
	}
	return false;
}



void Board::CreateFiguresOfCat(int level)
{
	for (int i = 0; i < level; i++)
		m_cat.push_back(Cat());
}

void Board::catsPosition()
{
	for (int i = 0; i < m_map.size(); ++i) {
		for (int j = 0; j < m_map[i].length(); ++j) {
			if (m_map[i][j] == CAT) {
				Location location{ i, j };
				m_catLocation.push_back(location);
			}
		}
	}

	// Assuming m_catLocation and m_cat have the same size
	for (size_t k = 0; k < m_catLocation.size(); ++k) {
		m_cat.push_back(Cat());
		m_cat[k].setPosition(m_catLocation[k]);
	}
}