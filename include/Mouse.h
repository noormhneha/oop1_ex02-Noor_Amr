#pragma once
#include "Location.h"

class Mouse
{
public:
	Mouse() = default;
	Location getPosition() const;
	void setPosition(const Location& newPos);

private:
	Location m_mouse;
};

