#pragma once

class Board;

class Cheese {
public:
	Cheese(Board&);
	int getCheeseCounter() const;

private:
	int m_cheeseCounter;
};