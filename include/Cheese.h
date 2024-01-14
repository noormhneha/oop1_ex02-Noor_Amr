#pragma once

class Board;

class Cheese {
public:
	Cheese(Board&);

	void decreaseCounter();

	int getCheeseCounter() const;
	bool remainingCheese() const;
private:
	int m_cheeseCounter;
	Board& m_board;
};