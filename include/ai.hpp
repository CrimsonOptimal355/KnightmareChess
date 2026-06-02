#pragma once
#include "Move.hpp"
#include <vector>

std::vector<Move> getAllLegalMoves(bool forWhite);
Move getRandomMove(bool forWhite);
int evaluateBoard();