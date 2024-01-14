#include "Mouse.h"

Location Mouse::getPosition() const
{
	return m_mouse;
}

void Mouse::setPosition(const Location& newPos)
{
	m_mouse = newPos;
}
