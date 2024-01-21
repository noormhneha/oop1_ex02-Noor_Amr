#pragma once
#include "Location.h"

class Cat
{
public:
	Cat();
	Location getPosition() const;
	void setPosition(const Location& newPos);

	char getNextChar() const;
	void setNextChar(const char& oldPos);

private:
	Location m_cat;
	char m_nextChar;
};

