#include "Controller.h"

Controller::Controller() :
m_levels{ "Level001.txt","Level002.txt" ,"Level003.txt" ,"Level004.txt" ,"Level005.txt" ,"Level006.txt" } 
{
	int counter = 0;
	for (auto i = m_levels.begin(); i != m_levels.end(); i++) {
		std::ifstream level(*i); // level map
		m_levelScore = { 3, 0 , 0 ,0 }; 
		gameLevel(level, counter);
		system("cls");
		counter++;
	}
}

void Controller::gameLevel(std::ifstream& level, int counter)
{	
	bool exit = false;
	Board board(counter, level); 
	m_levelScore._cheese_counter = board.cheeseCounter();
	while (!exit && m_levelScore._cheese_counter > 0) {
		printScore(board);
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
	case Keys::ESCAPE:
		break;
	default:
		break;
	}
}


void Controller::handleSpecialKey(Board& board)
{
	Screen::setLocation(board.getMouse().getPosition());
	const auto c = _getch();
	Location nextLocation = board.getMouse().getPosition();
	whichPressed(c, nextLocation);
	nextStep(board, nextLocation);
}

void Controller::nextStep(Board& board, Location nextLocation) {
	if (checkScoreStep(board, nextLocation)) {
		board.printStep(ROAD, RESET);
		Screen::setLocation(nextLocation);
		board.printStep(MOUSE, BROWN);
		board.setMouse().setPosition(nextLocation);
	}
	//else exit(EXIT_FAILURE);
}

bool Controller::handleRegularKey()
{
	return true;
}

bool Controller::checkScoreStep(Board& board, Location& location)
{
	char c = board.getMap()[location.row].at(location.col);
	switch (c)
	{
	case CAT:
		// lose 
		if (catCatch(board)) {
			location = board.getMouseLocation();
			return true;
		}
		else {
			exit(EXIT_FAILURE);
		}
		break;
	case DOOR:
		return doorOpen(); // check key 
	case KEY:
		// get
		m_levelScore._counter_key++;
		break;
	case CHEESE:
		// eat
		board.getMap()[location.row].at(location.col) = ' ';
		m_levelScore._cheese_counter--;
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
	if (m_levelScore._lives_remaining > 0) {
		m_levelScore._lives_remaining--;
		return true; 
	}
	else {
		//losegame
		return false; // exit;
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

void Controller::printScore(Board board) const
{
	int size = board.getMap().size() + 1;
	Screen::setLocation({size, 0});
	std::cout << "Score -> " << m_levelScore._score << std::endl;
	std::cout << "Lives -> " << m_levelScore._lives_remaining << std::endl;
	std::cout << "Keys -> " << m_levelScore._counter_key << std::endl;
}



