#include "Cat.h"
#include "Board.h"

Cat::Cat(): m_cat{ 1,1 }, m_nextChar(ROAD){}

Location Cat::getPosition() const
{
	return m_cat;
}

void Cat::setPosition(const Location& newPos)
{
	m_cat = newPos;
}

char Cat::getNextChar() const
{
	return m_nextChar;
}

void Cat::setNextChar(const char& oldPos)
{
	m_nextChar = oldPos;
}

