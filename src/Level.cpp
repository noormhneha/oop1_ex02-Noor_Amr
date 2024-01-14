#include "Level.h"
#include "Board.h"

Level::Level(std::ifstream& level, Board& board) : m_level(lv) {
    if (!level.is_open()) {
        std::cerr << "Error opening the level file." << std::endl;
        return;
    }
    std::string line;
    while (std::getline(level, line)) {
        std::cout << line << std::endl;
        m_board.getMap().push_back(line);
    }
}

std::ifstream& Level::whichLevel()
{
    return m_level;
}