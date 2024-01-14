#include "Cheese.h"
#include "Board.h"

Cheese::Cheese(Board& board) : m_cheeseCounter(0) , m_board(board)
{
    for (const auto& line : m_board.getMap()) {
        for (int j = 0; j < line.length(); j++) {
            if (line[j] == CHEESE) {
                m_cheeseCounter++;
            }
        }
    }
}


void Cheese::decreaseCounter() {
    m_cheeseCounter--;
}

int Cheese::getCheeseCounter()
{
	return  m_cheeseCounter;
}
