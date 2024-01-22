#pragma once

#include "Board.h"
#include "Score.h"
#include <cstdlib>
#include <conio.h>
#include <io.h>

class Controller {
public:
	Controller();

private:
	void printChooseLevel();

	void gameLevel(std::ifstream& level, const std::string fileName);
	void whichPressed(const auto c, Location& location);
	void handleSpecialKey(Board& board);
	bool handleRegularKey();
	void nextStep(Board& board, Location location);

	bool checkScoreStep(Board& board, Location& location);
	bool catCatch(Board& board);
	bool doorOpen();

	void printScore(Board board, const std::string fileName) const;


	void removeCat(Board& board, size_t counter);
	void moveCat(Board& board);

	Location randomMove(Board& board, Cat& catLocation);
	Location calculateDistance(Board& board, Cat& cat);

	bool checkNextCatStep(Board& board, Location& nextPos, Cat& cat);
	bool collision(Board& board);
	bool differentLocation(const Location& first, const Location& second);
	void restPos(Board& board);

	std::vector<std::string> setData(); // taking data from levels file 

	std::string playList;
	std::vector<std::string> m_levels;

	std::vector<Location> m_allCatsLocation;

	Score m_levelScore;

	bool resetCats = false;
	
	bool m_easyLevel = true;
};