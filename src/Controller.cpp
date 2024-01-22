#pragma once
#include "Controller.h"
#include "Cat.h"

Controller::Controller() : playList("Levels.txt"), m_levels{ "Level001.txt" } 
{
	m_levels = setData();
	m_levelScore = { 3, 0, 0, 0 };
	for (auto i = m_levels.begin(); i != m_levels.end(); i++) {
		std::ifstream level(*i); // level map
		gameLevel(level);
		system("cls");
	}
} 
// -----------------------------------------------------------------------------

std::vector<std::string> Controller::setData()
{
	std::ifstream levellist(playList);
	if (!levellist.is_open()) {
		std::cerr << "Error opening file: ";
		exit(EXIT_FAILURE);
	}

	std::vector<std::string> levels;

	std::string line;
	while (std::getline(levellist, line)) {
		levels.push_back(line);
	}

	levellist.close();

	return levels;
}
// -----------------------------------------------------------------------------
void Controller::gameLevel(std::ifstream& level) {	
	bool exit = false;
	Board board(level); 
	m_allCatsLocation = board.getCatLocation();
	m_levelScore._cheese_counter = board.cheeseCounter();
	while (!exit && m_levelScore._cheese_counter > 0) {
		Location temp = board.getMouse().getPosition();
		printScore(board);
 		const auto c = _getch();
		switch (c)
		{
		case 0: case Keys::SPECIAL_KEY: case ' ':
			handleSpecialKey(board);
			break;
		//case Keys::ESCAPE: 
		//	system("cls");
		//	return;
		default:
			exit = handleRegularKey();
			break;
		}
		if (differentLocation(temp, board.getMouse().getPosition()))
			moveCat(board);
	}
	m_levelScore._score += 25;
	m_levelScore._score += (5 * int(board.getCat().size()));
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
	case ' ': default:
		break;
	}
}
// -----------------------------------------------------------------------------


void Controller::handleSpecialKey(Board& board)
{
	Screen::setLocation(board.getMouse().getPosition());
	const auto c = _getch();
	Location nextLocation = board.getMouse().getPosition();
	whichPressed(c, nextLocation);
	nextStep(board, nextLocation);
}
// -----------------------------------------------------------------------------

void Controller::nextStep(Board& board, Location nextLocation) {
	if (checkScoreStep(board, nextLocation)) {
		board.printStep(ROAD, RESET);
		Screen::setLocation(nextLocation);
		board.printStep(MOUSE, MOUSECOLOR);
		board.setMouse().setPosition(nextLocation);
	}
}
// -----------------------------------------------------------------------------


bool Controller::handleRegularKey()
{
	return true;
}

bool Controller::checkScoreStep(Board& board, Location& location)
{
	static size_t counter = 0;
	char c = board.getMap()[location.row].at(location.col);
	switch (c)
	{
	case CAT:  return (catCatch(board, location)); // lose 

	case DOOR: return (doorOpen(board, location)); // check key 

	case KEY:
		// get
		m_levelScore._counter_key++;
		board.getMap()[location.row].at(location.col) = ROAD;
		break;

	case CHEESE:
		// eat
		board.getMap()[location.row].at(location.col) = ROAD;
		m_levelScore._cheese_counter--;
		m_levelScore._score += 10;
		break;

	case GIFT:
		// delete cat
		removeCat(board, counter);
		counter++;
		m_levelScore._score += 5;
		board.getMap()[location.row].at(location.col) = ROAD;
		break;
	case WALL:
		// stop
		return false;
		break;
	}
	
	return true;
}

bool Controller::catCatch(Board& board, Location& location)
{
	if (m_levelScore._lives_remaining > 0) {
		m_levelScore._lives_remaining--;
		location = board.getMouseLocation();
		restPos(board, location);
		return true; 
	}
	else {
		//losegame
		system("cls");
		exit(EXIT_FAILURE); // exit;
	}
}

bool Controller::doorOpen(Board& board, Location& location) {
	if (m_levelScore._counter_key > 0) {
		m_levelScore._score += 2;
		m_levelScore._counter_key--;
		board.getMap()[location.row].at(location.col) = ROAD;
		return true;
	}
	return false;
}

void Controller::removeCat(Board& board, size_t counter) {
	Cat cat = board.setCat().at(counter);
	char tempChar = cat.getNextChar();
	Location location = board.getMouseLocation();
	
	Location temp = cat.getPosition();
	board.printStep(ROAD, RESET);	
	Screen::setLocation(temp);
	board.printColoredStep(tempChar, board);
	board.getMap()[temp.row].at(temp.col) = tempChar;

	m_allCatsLocation.erase(m_allCatsLocation.begin() + counter);
	m_allCatsLocation.shrink_to_fit();	
	
	board.setCat().erase(board.setCat().begin() + counter);
	board.setCat().shrink_to_fit();

	//cat.setPosition(temp);
		
	Screen::setLocation(location);
}

void Controller::moveCat(Board& board)
{
	for (auto& cat : board.setCat()) {
		char temp = cat.getNextChar();
		Location tempLoc = cat.getPosition();
		Location nextLoc = calculateDistance(board, cat);
		cat.setNextChar(cat.getNextChar());
		Screen::setLocation(tempLoc);
		if (differentLocation(tempLoc, nextLoc)) {
			board.printColoredStep(temp, board);
			Screen::setLocation(nextLoc);
			board.printStep(CAT, CATCOLOR);

			board.getMap()[tempLoc.row].at(tempLoc.col) = temp;
			board.getMap()[nextLoc.row].at(nextLoc.col) = CAT;
			cat.setPosition(nextLoc);

			if (collision(board)) {
				Location tempMouseLoct = { 1,1 };
				catCatch(board, tempMouseLoct);
				board.setMouse().setPosition(tempMouseLoct);
				restPos(board, tempMouseLoct);
			}
		}
	}
}

Location Controller::randomMove(Board& board, Cat& cat)
{
	srand((unsigned)time(NULL));
	int moves[4] = { SpecialKeys::UP, SpecialKeys:: DOWN,
					SpecialKeys:: RIGHT, SpecialKeys:: LEFT };

	Location nextStep = cat.getPosition();
	Location tempLocation = nextStep;
	int i = rand() % 4;
	whichPressed(moves[i] , nextStep);

	if (checkNextCatStep(board, nextStep, cat))
		return nextStep;
	
	return tempLocation;
}

Location Controller::calculateDistance(Board& board, Cat& cat)
{
	Location mousePos = board.getMouse().getPosition();
	Location catPos = cat.getPosition();
	Location nextStep{ 0,0 };

	nextStep.row = abs(catPos.row - mousePos.row);
	nextStep.col = abs(catPos.col - mousePos.col);

	if (nextStep.row >= nextStep.col) {
		nextStep.col = catPos.col;
		if (catPos.row - mousePos.row < 0)
			nextStep.row = catPos.row + 1;
		else
			nextStep.row = catPos.row - 1;
	}
	else {
		nextStep.row = catPos.row;
		if (catPos.col - mousePos.col < 0)
			nextStep.col = catPos.col + 1;
		else
			nextStep.col = catPos.col - 1;
	}

	if (checkNextCatStep(board, nextStep, cat))
		return nextStep;

	return catPos;
}

bool Controller::checkNextCatStep(Board& board, Location& nextPos, Cat& cat)
{
	char c = board.getMap()[nextPos.row].at(nextPos.col);
	switch (c)
	{
	case CAT: case DOOR: case WALL: return false;

	case KEY: case CHEESE: case GIFT: case MOUSE: case ROAD:
		cat.setNextChar(c);
		return true;
	}
	return true;
}

bool Controller::collision(Board& board)
{
	Location temp = board.getMouse().getPosition();
	for (auto& cat : board.getCat()) {
		if (!differentLocation(cat.getPosition(), temp))
			return true;
	}
	return false;
}

bool Controller::differentLocation(const Location& first, const Location& second)
{
	return !(first.col == second.col && first.row == second.row);
}

void Controller::restPos(Board& board, Location& location) {
	std::vector<Location> catLoc = m_allCatsLocation;
	std::vector<Cat> cats = board.setCat();
	
	board.printStep(ROAD, RESET);
	Screen::setLocation(location);

	for (int i = 0; i < catLoc.size(); i++) {
		Screen::setLocation(cats[i].getPosition());
		board.printStep(ROAD, RESET);
		//board.SetCell(cats[i].getPosition(), ROAD);

		Screen::setLocation(catLoc[i]);
		board.setCat().at(i).setPosition(catLoc[i]);
		board.printStep(CAT, CATCOLOR);
	}
}

void Controller::printScore(Board board) const
{
	size_t size = board.getMap().size() + 1;
	Screen::setLocation({int(size), 0});
	std::cout << "Score -> " << m_levelScore._score << std::endl;
	std::cout << "Lives -> " << m_levelScore._lives_remaining << std::endl;
	std::cout << "Keys -> " << m_levelScore._counter_key << std::endl;
}




