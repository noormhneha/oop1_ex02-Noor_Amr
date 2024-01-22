#pragma once

#include "Cheese.h"
#include "Board.h"

Cheese::Cheese(Board& board) : m_cheeseCounter(0){
    for (const auto& line : board.getMap()) {
        for (int j = 0; j < line.length(); j++) {
            if (line[j] == CHEESE) {
                m_cheeseCounter++;
            }
        }
    }
}

int Cheese::getCheeseCounter() const
{
	return  m_cheeseCounter;
}
