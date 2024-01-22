#include "Level.h"
#include "Board.h"

Level::Level(std::ifstream& level, Board& board) : m_level(level), m_board(board) {
    if (!level.is_open()) {
        std::cerr << "Error opening the level file." << std::endl;
        return;
    }
    std::string line;
    while (std::getline(level, line)) {
        for (int i = 0; i < line.length(); i++) {
            m_board.printColoredStep(line[i], board);
        }
        std::cout << std::endl;
        m_board.getMap().push_back(line);
    }
    m_board.setMapColSize(m_board.getMap().at(0).length());
    m_board.setMapRowSize(m_board.getMap().size());
}
