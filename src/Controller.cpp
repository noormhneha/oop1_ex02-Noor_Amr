#pragma once
#include "Controller.h"
#include "Cat.h"

// Constructor for the Controller class
Controller::Controller() : playList("Levels.txt"), m_levels{ "Level001.txt" }
{
	// level
	printChooseLevel();
	// Initialize m_levels vector with data from the setData() function
	m_levels = setData();
	// Initialize m_levelScore with values
	m_levelScore = { 3, 0, 0, 0 };
	// Iterate through each level in m_levels
	for (auto i = m_levels.begin(); i != m_levels.end(); i++) {
		// Open the level file for reading (level map)
		std::ifstream level(*i);

		// Call the gameLevel function to process the level
		gameLevel(level);

		// Clear the console screen 
		system("cls");
	}
}

// -----------------------------------------------------------------------------
void Controller::printChooseLevel() {
	std::cout << "Choose your level - press 1 for easy following | press 2 for hard following\n";
	int i = 0;
	std::cin >> i;
	while (i < 1 || i > 2) {
		std::cout << "Wrong choise! \n1 - easy | 2 - hard \n";
		std::cin >> i;
	}
	i == 1 ? m_easyLevel = true : m_easyLevel = false;
	std::cout << "Your choosed: " << (i == 1 ? "easy level" : "hard level") << " - Press Enter to continue" << std::endl;
	std::cin.get(); std::cin.get();
	system("cls");
}

// -----------------------------------------------------------------------------

// Function to read level names from a file and return a vector of strings
std::vector<std::string> Controller::setData()
{
	// Open the file containing the list of level names
	std::ifstream levellist(playList);

	// Check if the file is successfully opened
	if (!levellist.is_open()) {
		// If the file cannot be opened, print an error message and exit the program
		std::cerr << "Error opening file!";
		exit(EXIT_FAILURE);
	}

	// Vector to store the level names
	std::vector<std::string> levels;

	// String to store each line read from the file
	std::string line;

	// Read each line from the file and add it to the vector
	while (std::getline(levellist, line)) {
		levels.push_back(line);
	}

	// Close the file after reading all lines
	levellist.close();

	// Return the vector containing the level names
	return levels;
}

// -----------------------------------------------------------------------------
// Function to manage the gameplay for a given level
void Controller::gameLevel(std::ifstream& level) {
	
	bool exit = false; // Variable to determine if the player wants to exit the level
	Board board(level); 	// Create a Board object, initializing it with the level read from the file

	m_allCatsLocation = board.getCatLocation();	// Get the initial positions of all cats on the board
	m_levelScore._cheese_counter = board.cheeseCounter(); 	// Initialize the cheese counter with the number of cheeses on the board

	// Loop until the player exits or runs out of cheese
	while (!exit && m_levelScore._cheese_counter > 0) {
		resetCats = false; // Flag to determine if the cats need to be reset

		Location temp = board.getMouse().getPosition(); // Get the current position of the mouse
		printScore(board); // Print the current score and board
		const auto c = _getch(); // Wait for user input
		// Handle user input based on the pressed key
		switch (c) {
		case 0: case Keys::SPECIAL_KEY: case ' ':
			handleSpecialKey(board);
			break;
		default:
			exit = handleRegularKey();
			break;
		}
		// If the mouse moved and the cats don't need to be reset, move the cats
		if (differentLocation(temp, board.getMouse().getPosition()) && !resetCats)
			moveCat(board);
	}
	// Update the score based on the completed level
	m_levelScore._score += 25;
	m_levelScore._score += (5 * int(board.getCat().size()));
}

// -----------------------------------------------------------------------------
// Function to update the location based on the pressed key
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
// Function to handle special key input
void Controller::handleSpecialKey(Board& board) {
	const auto c = _getch();
	Location nextLocation = board.getMouse().getPosition();
	whichPressed(c, nextLocation);
	nextStep(board, nextLocation);
}

// -----------------------------------------------------------------------------
// check the next step and if it valid go throw and update board
void Controller::nextStep(Board& board, Location nextLocation) {
	if (checkScoreStep(board, nextLocation)) {
		Screen::setLocation(board.getMouse().getPosition());
		board.printStep(ROAD, RESET);
		board.SetCell(board.getMouse().getPosition(), ROAD);

		Screen::setLocation(nextLocation);
		board.printStep(MOUSE, MOUSECOLOR);
		board.setMouse().setPosition(nextLocation);
		board.SetCell(nextLocation, MOUSE);
	}
}
// -----------------------------------------------------------------------------
bool Controller::handleRegularKey() {
	return true;
}
// -----------------------------------------------------------------------------
// checking if valid step - and count the score if there is.
bool Controller::checkScoreStep(Board& board, Location& location) {
	static size_t counter = 0;
	char c = board.getMap()[location.row].at(location.col);
	switch (c)
	{
	case CAT:  return (catCatch(board)); // lose 
	case DOOR: return (doorOpen()); // check key 
	case KEY: // get
		m_levelScore._counter_key++;
		break;
	case CHEESE: // eat
		m_levelScore._cheese_counter--;
		m_levelScore._score += 10;
		break;
	case GIFT: // delete cat
		removeCat(board, counter);
		counter++;
		m_levelScore._score += 5;
		break;
	case WALL: return false; // stop
	}

	return true;
}
// -----------------------------------------------------------------------------
// did the cat catch you?
bool Controller::catCatch(Board& board) {
	if (m_levelScore._lives_remaining > 0) {
		m_levelScore._lives_remaining--;
		restPos(board);
		return false; 
	}
	else {
		//losegame
		system("cls");
		exit(EXIT_FAILURE); // exit;
	}
}

// -----------------------------------------------------------------------------
// if you got the door - check if you have the key
bool Controller::doorOpen() {
	if (m_levelScore._counter_key > 0) {
		m_levelScore._score += 2;
		m_levelScore._counter_key--;
		return true;
	}
	return false;
}

// -----------------------------------------------------------------------------
// removing the cat after you got a gift
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
		
	Screen::setLocation(location);
}
// -----------------------------------------------------------------------------
// moving the cat bassed algorithm - you can choose which one
// if random or smart algo.
void Controller::moveCat(Board& board) {
	for (auto& cat : board.setCat()) {     // Iterate over each cat on the board
		char temp = cat.getNextChar();
		Location tempLoc = cat.getPosition();
		Location nextLoc;
		!m_easyLevel ? nextLoc = calculateDistance(board, cat) : nextLoc = randomMove(board, cat);
		cat.setNextChar(cat.getNextChar());
		Screen::setLocation(tempLoc);
		if (differentLocation(tempLoc, nextLoc)) {
			board.printColoredStep(temp, board);
			Screen::setLocation(nextLoc);
			board.printStep(CAT, CATCOLOR);

			// Update the map with the new positions of the cat and the previous character
			board.getMap()[tempLoc.row].at(tempLoc.col) = temp;
			board.getMap()[nextLoc.row].at(nextLoc.col) = CAT;
			cat.setPosition(nextLoc);   // Update the cat's position

			if (collision(board)) {  // Check for collision with the mouse
				catCatch(board); // check catch and update location
				restPos(board);
			}
		}
	}
}
// -----------------------------------------------------------------------------
// Function to randomly determine the next move for a cat
Location Controller::randomMove(Board& board, Cat& cat) {
	srand((unsigned)time(NULL));
	int moves[4] = { SpecialKeys::UP, SpecialKeys:: DOWN,
					SpecialKeys:: RIGHT, SpecialKeys:: LEFT };

	Location nextStep = cat.getPosition();
	Location tempLocation = nextStep;
	int i = rand() % 4;     // Randomly select one of the four possible directions
	whichPressed(moves[i] , nextStep);

	if (checkNextCatStep(board, nextStep, cat))
		return nextStep;
	
	return tempLocation;
}
// -----------------------------------------------------------------------------
// Function to calculate the next step for a cat based on Manhattan distance to the mouse
Location Controller::calculateDistance(Board& board, Cat& cat)
{
	Location mousePos = board.getMouse().getPosition();
	Location catPos = cat.getPosition();
	Location nextStep{ 0,0 };

	nextStep.row = abs(catPos.row - mousePos.row);
	nextStep.col = abs(catPos.col - mousePos.col);

	// Determine the direction with the larger difference and update nextStep accordingly
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

// -----------------------------------------------------------------------------
// Function to check if the next step for a cat is valid on the game board
bool Controller::checkNextCatStep(Board& board, Location& nextPos, Cat& cat)
{
	char c = board.getMap()[nextPos.row].at(nextPos.col);
	switch (c)
	{
	case CAT: case DOOR: case WALL: return false;

	case KEY: case CHEESE: case GIFT: case ROAD:
		cat.setNextChar(c);
		return true;
	}
	return true;
}

// -----------------------------------------------------------------------------
// Function to check if there is a collision between the mouse and any cat on the board
bool Controller::collision(Board& board)
{
	Location temp = board.getMouse().getPosition();
	for (auto& cat : board.getCat()) {
		if (!differentLocation(cat.getPosition(), temp))
			return true;
	}
	return false;
}

// -----------------------------------------------------------------------------
// check if the same location or different
bool Controller::differentLocation(const Location& first, const Location& second)
{
	return !(first.col == second.col && first.row == second.row);
}

// -----------------------------------------------------------------------------
// Function to reset the positions of the mouse and cats after a collision
void Controller::restPos(Board& board) {
	std::vector<Location> catLoc = m_allCatsLocation;
	std::vector<Cat> cats = board.setCat();

	Location tempMouse = board.getMouse().getPosition();
	Location nextLocation = board.getMouseLocation();

	// Reset the cell at the current mouse position to ROAD
	board.SetCell(tempMouse, ROAD);
	Screen::setLocation(tempMouse);
	board.printStep(ROAD, RESET);

	// Set the cursor to the next mouse location and print the mouse character
	Screen::setLocation(nextLocation);
	board.printStep(MOUSE, MOUSECOLOR);
	board.setMouse().setPosition(nextLocation);
	board.SetCell(nextLocation, MOUSE);

	for (int i = 0; i < cats.size(); i++) {     // Iterate over each cat
		Screen::setLocation(cats[i].getPosition());
		board.printColoredStep(cats[i].getNextChar(), board);

		board.SetCell(cats[i].getPosition(), cats[i].getNextChar());

		cats[i].setNextChar(ROAD);

		Screen::setLocation(catLoc[i]);
		board.printStep(CAT, CATCOLOR);
		board.setCat().at(i).setPosition(catLoc[i]);
		board.SetCell(catLoc[i], CAT);
	}

	// Set the flag indicating that the cats has been reset
	resetCats = true;
}

void Controller::printScore(Board board) const
{
	size_t size = board.getMap().size() + 1;
	Screen::setLocation({int(size), 0});
	std::cout << "Score -> " << m_levelScore._score << std::endl;
	std::cout << "Lives -> " << m_levelScore._lives_remaining << std::endl;
	std::cout << "Keys -> " << m_levelScore._counter_key << std::endl;
}