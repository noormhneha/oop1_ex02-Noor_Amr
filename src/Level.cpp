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
            printColoredStep(line[i], board);
        }
        std::cout << std::endl;
//        std::cout << line << std::endl;
        m_board.getMap().push_back(line);
    }
    m_board.setMapColSize(m_board.getMap().at(0).length());
    m_board.setMapRowSize(m_board.getMap().size());
}

void Level::printColoredStep(const char c, Board& board){
    switch (c){
        case MOUSE:
            board.printStep(c, MOUSECOLOR);
            break; 
        case DOOR:
            board.printStep(c, DOORCOLOR);
            break; 
        case CHEESE:
            board.printStep(c, CHEESECOLOR);
            break; 
        case GIFT:
            board.printStep(c, GIFTCOLOR);
            break; 
        case KEY:
            board.printStep(c, KEYCOLOR);
            break; 
        case CAT:
            board.printStep(c, CATCOLOR);
            break; 
        default:
            board.printStep(c, RESET);
            break;
    }
}

std::ifstream& Level::whichLevel()
{
    return m_level;
}