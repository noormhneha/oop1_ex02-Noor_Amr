#include "Controller.h"

Controller::Controller() :
m_levels{ "Level001.txt","Level002.txt" ,"Level003.txt" ,"Level004.txt" ,"Level005.txt" ,"Level006.txt" } 
{
	Board board(1, m_levels[0]);
}