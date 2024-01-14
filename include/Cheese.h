

class Board;

class Cheese {
public:
	Cheese(Board&);

	void decreaseCounter();

	int getCheeseCounter();

private:
	int m_cheeseCounter;
	Board& m_board;
};