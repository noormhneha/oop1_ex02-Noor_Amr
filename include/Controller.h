#pragma once

#include "Board.h"
#include <cstdlib>
#include <conio.h>
#include <io.h>

#include "Score.h"

class Controller {
public:
	Controller();

private:
	void gameLevel(std::ifstream& level, int counter);
	void whichPressed(const auto c, Location& location);
	void handleSpecialKey(Board& board);
	bool handleRegularKey();
	void nextStep(Board& board, Location location);

	bool checkScoreStep(Board& board, Location& location);
	bool catCatch(Board board);
	bool doorOpen();

	void printScore(Board board) const;

	std::vector<std::string> m_levels;
	Score m_levelScore;
};