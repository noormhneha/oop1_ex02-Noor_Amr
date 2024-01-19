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
            printColoredStep(line[i]);
        }
//        std::cout << line << std::endl;
        m_board.getMap().push_back(line);
    }
    m_board.setMapColSize(m_board.getMap().at(0).length());
    m_board.setMapRowSize(m_board.getMap().size());
}

void Level::printColoredStep(const char c){
    switch (c){
        case MOUSE:
            printStep(c, BROWN);
            break; // brown
        case DOOR:
            printStep(c, GREEN);
            break; // green
        case CHEESE:
            printStep(c, yellow);
            break; // yellow
        case GIFT:
            printStep(c, blue);
            break; // blue
        case KEY:
            printStep(c, KEY);
            break; // key
        case CAT:
            printStep(c, RED);
            break; // red
        default:
            printStep(c, RESET);
            break;
    }
}

std::ifstream& Level::whichLevel()
{
    return m_level;
}