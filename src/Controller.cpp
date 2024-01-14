#include "Controller.h"

Controller::Controller() :
m_levels{ "Level001.txt","Level002.txt" ,"Level003.txt" ,"Level004.txt" ,"Level005.txt" ,"Level006.txt" } 
{
	int counter = 0;
	for (auto i = m_levels.begin(); i != m_levels.end(); i++) {
		std::ifstream level(*i); // level map
		gameLevel(level, counter);
		system("cls");
		counter++;
	}
}

void Controller::gameLevel(std::ifstream& level, int counter)
{	
	bool exit = false;
	Board board(counter, level);
	while (!exit) {
		const auto c = _getch();
		switch (c)
		{
		case 0: case Keys::SPECIAL_KEY:
			handleSpecialKey(board);
			break;
		default:
			exit = handleRegularKey();
			break;
		}
	}
}

void Controller::whichPressed(const auto c, Location& location) {
	switch (c)
	{
	case SpecialKeys::UP:
		location.row--;
		break;
	case SpecialKeys::DOWN:
		location.row++;
		break;
	case SpecialKeys::LEFT:
		location.col--;
		break;
	case SpecialKeys::RIGHT:
		location.col++;
		break;
	default:
		break;
	}
}


void Controller::handleSpecialKey(Board board)
{
	Screen::setLocation(board.getMouse().getPosition());
	//validDirection(lv, cheese, score);
	const auto c = _getch();
	Location nextLocation = board.getMouse().getPosition();
	whichPressed(c, nextLocation);
	nextStep(board, nextLocation);
}

void Controller::nextStep(Board board, Location nextLocation) {
	if (checkScoreStep(board, nextLocation)) {
		// score
		board.printStep(ROAD);
		Screen::setLocation(board.getMouse().getPosition());
		board.printStep(MOUSE);
	}
	else {
		board.setMouse().setPosition(nextLocation);
	}
}

bool Controller::handleRegularKey()
{
	return true;
}

bool Controller::checkScoreStep(Board board, Location location)
{
	//Location location = board.getMouse().getPosition(); send temp
	char c = board.getMap()[location.row].at(location.col);
	switch (c)
	{
	case CAT:
		// lose 
		return catCatch(board);
		break;
	case DOOR:
		return doorOpen(); // check key 
	case KEY:
		// get
		m_levelScore._counter_key++;
		break;
	case CHEESE:
		// eat
		m_levelScore._score += 10;
		break;
	case GIFT:
		// delete cat
		m_levelScore._score += 5;
		break;
	case WALL:
		// stop
		return false;
		break;
	}
	return true;
}

bool Controller::catCatch(Board board)
{
	if (m_levelScore._lives_remaining >= 0) {
		m_levelScore._lives_remaining--;
		Location firstLocation = board.SetStartPos();
		board.setMouse().setPosition(firstLocation);
		board.printStep(MOUSE);
		
	}
	else {
		//losegame
		return false;
	}
}

bool Controller::doorOpen() {
	if (m_levelScore._counter_key > 0) {
		m_levelScore._score += 2;
		m_levelScore._counter_key--;
		return true;
	}
	return false;
}



