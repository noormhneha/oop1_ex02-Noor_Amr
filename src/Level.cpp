#include "Level.h"
#include "Board.h"

Level::Level(std::ifstream& level, Board& board) : m_level(level), m_board(board) {
    if (!level.is_open()) {
        std::cerr << "Error opening the level file." << std::endl;
        return;
    }
    std::string line;
    while (std::getline(level, line)) {
        std::cout << line << std::endl;
        m_board.getMap().push_back(line);
    }
    m_board.setMapColSize(line.length());
    m_board.setMapRowSize(m_board.getMap().size());
}

std::ifstream& Level::whichLevel()
{
    return m_level;
}