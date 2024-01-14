#include "Key.h"
#include "Board.h"

Key::Key(Board& board) : m_keyCounter(0), m_board(board)
{
    for (const auto& line : m_board.getMap()) {
        for (int j = 0; j < line.length(); j++) {
            if (line[j] == CHEESE) {
                m_keyCounter++;
            }
        }
    }
}


void Key::decreaseCounter() {
    m_keyCounter--;
}

int Key::getKeyCounter() const
{
    return m_keyCounter;
}

bool Key::remainingKeys() const {
    return m_keyCounter == 0;
}