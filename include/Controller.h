#pragma once

#include <queue>
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
	bool handleRegularKey() const;
	void nextStep(Board& board, const Location& location);

	bool checkScoreStep(Board& board, const Location& location);
	bool catCatch(Board& board);
	bool doorOpen();

	void printScore(size_t size, const std::string fileName) const;
	void printInformation(size_t size) const;

	void removeCat(Board& board, size_t counter);
	void moveCat(Board& board);

	Location randomMove(Board& board, Cat& catLocation);
	Location calculateDistance(Board& board, Cat& cat);

	bool checkNextCatStep(Board& board, Location& nextPos, Cat& cat);
	bool collision(Board& board) const;
	bool differentLocation(const Location& first, const Location& second) const;
	void restPos(Board& board);


	std::vector<std::string> setData(); // taking data from levels file 

	std::string m_playList;
	std::vector<std::string> m_levels;

	std::vector<Location> m_allCatsLocation;

	Score m_levelScore;

	bool m_resetCats = false;
	
	bool m_easyLevel = true;
};