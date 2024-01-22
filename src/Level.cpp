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

    // Set column size to the maximum length of any row
    if (!m_board.getMap().empty()) {
        auto maxColSize = std::max_element(
            m_board.getMap().begin(), m_board.getMap().end(),
            [](const std::string& a, const std::string& b) {
                return a.length() < b.length();
            }
        )->length();
        m_board.setMapColSize(maxColSize);
    }

    // Set row size to the number of rows
    m_board.setMapRowSize(m_board.getMap().size());
}
