#include "Cat.h"

Location Cat::getPosition() const
{
	return m_cat;
}

void Cat::setPosition(const Location& newPos)
{
	m_cat = newPos;
}

