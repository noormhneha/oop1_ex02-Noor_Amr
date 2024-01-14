#pragma once

class Board;

class Key {
public:
	Key(Board&);

	void decreaseCounter();

	int getKeyCounter() const;
	bool remainingKeys() const;
private:
	int m_keyCounter;
	Board& m_board;
};