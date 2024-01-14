#pragma once
#include "Location.h"

class Cat
{
public:
	Cat() = default;
	Location getPosition() const;
	void setPosition(const Location& newPos);

private:
	Location m_cat;
};

