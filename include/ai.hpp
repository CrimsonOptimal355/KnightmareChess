#pragma once
#include "Move.hpp"
#include <atomic>
#include <vector>


extern std::atomic<long long> nodesSearched;
std::vector<Move> getAllLegalMoves(bool forWhite);
Move getRandomMove(bool forWhite);
int evaluateBoard();
Move getBestMove(bool forWhite);
int minimax(int depth, bool maximizing);
Move getMinimaxMove(bool forWhite, int depth);