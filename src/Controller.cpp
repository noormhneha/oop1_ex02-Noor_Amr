#pragma once
#include "Controller.h"
#include "Cat.h"

Controller::Controller() : playList("Levels.txt"), m_levels{ "Level001.txt" } 
{
	m_levels = setData();
	int counter = 0;
	for (auto i = m_levels.begin(); i != m_levels.end(); i++) {
		std::ifstream level(*i); // level map
		m_levelScore = { 3, 0, 0, 0 };
		gameLevel(level, counter);
		system("cls");
		counter++;
	}
}

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

void Controller::gameLevel(std::ifstream& level, int counter)
{	
	bool exit = false;
	Board board(counter, level); 
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
		board.printStep(MOUSE, MOUSECOLOR);
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
	static size_t counter = 0;
	char c = board.getMap()[location.row].at(location.col);
	switch (c)
	{
	case DOOR:
		if (doorOpen()) {
			board.getMap()[location.row].at(location.col) = ROAD;
			return true;
		} 
		return false; // check key 
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

	if (collision(board)) 
		catCatch(board);
		
	
	return true;
}

bool Controller::catCatch(Board board)
{
	if (m_levelScore._lives_remaining > 0) {
		m_levelScore._lives_remaining--;
		restPos(board);
		return true; 
	}
	else {
		//losegame
		system("cls");
		exit(EXIT_FAILURE); // exit;
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

void Controller::removeCat(Board& board, size_t counter) {
	std::vector cat = board.getCat();
	Location location = board.getMouseLocation();
	if (counter < cat.size()) {
		board.printStep(ROAD, RESET);	
		Location temp = cat[counter].getPosition();
		Screen::setLocation(temp);
		board.printStep(ROAD, RESET);
		board.getMap()[temp.row].at(temp.col) = ROAD;
	}
	Screen::setLocation(location);
}

void Controller::moveCat(Board& board)
{
	for (auto& cat : board.setCat()) {
		Location tempLoc = cat.getPosition();
		Location nextLoc = randomMove(board, cat);

		if (differentLocation(tempLoc, nextLoc)) {
			
			Screen::setLocation(tempLoc);
			board.printStep(cat.getNextChar(), RESET);

			Screen::setLocation(nextLoc);
			board.printStep(CAT, CATCOLOR);

			cat.setPosition(nextLoc);

			if (collision(board)) 
				catCatch(board);
				
			

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
	whichPressed(moves[rand() % 4], nextStep);
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
	case DOOR:
		return false; 
	case KEY:
		cat.setNextChar(KEY);
		return true;
	case CHEESE:
		cat.setNextChar(CHEESE);
		return true;
	case GIFT:
		cat.setNextChar(GIFT);
		return true;
	case WALL:
		// stop
		return false;
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
	if (first.col != second.col || first.row != second.row)
		return true;
	return false;
}

void Controller::restPos(Board& board)
{
	
	std::vector<Location> catLoc = board.getCatLocation();
	std::vector<Cat> cats = board.setCat();

	Location mouseLoc = board.getMouseLocation();
	
	Screen::setLocation(board.getMouse().getPosition());
	board.printStep(ROAD, RESET);

	Screen::setLocation(mouseLoc);
	board.printStep(MOUSE,MOUSECOLOR);
	board.SetCell(mouseLoc, MOUSE);
	board.setMouse().setPosition(mouseLoc);

	for (int i = 0; i < catLoc.size(); i++) {
		Screen::setLocation(cats[i].getPosition());
		board.printStep(ROAD, RESET);

		cats[i].setPosition(catLoc[i]);
		Screen::setLocation(catLoc[i]);
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




