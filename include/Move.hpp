#pragma once
#include "MoveStruct.hpp"
#include <vector>
#include <string>
#include <iostream>

bool isValidPawnMove(int startRow, int startCol, int endRow, int endCol,
                     char sourcepiece, char targetpiece);
bool isValidBishopMove(int startRow, int startCol, int endRow, int endCol,
                       char sourcepiece, char targetpiece);
bool isValidKnightMove(int startRow, int startCol, int endRow, int endCol,
                       char sourcepiece, char targetpiece);
bool isValidQueenMove(int startRow, int startCol, int endRow, int endCol,
                      char sourcepiece, char targetpiece);
bool isValidKingMove(int startRow, int startCol, int endRow, int endCol,
                     char sourcepiece, char targetpiece);
bool isValidRookMove(int startRow, int startCol, int endRow, int endCol,
                     char sourcepiece, char targetpiece);
bool isPathClear(int startRow, int startCol, int endRow, int endCol);
bool isKingInCheck(bool whiteKing);
bool isMovemakeKingCheck(int startRow, int startCol, int endRow, int endCol,
                         char sourcepiece, char targetpiece);
bool isAnyLegalMove(bool whiteKing);
bool isValidMove(int startRow, int startCol, int endRow, int endCol,
                 char sourcePiece, char targetPiece);
bool isValidCastling(int startRow, int startCol, int endRow, int endCol,
                     char sourcePiece, bool whiteKingMoved, bool blackKingMoved,
                     bool whiteLeftRookMoved, bool whiteRightRookMoved,
                     bool blackLeftRookMoved, bool blackRightRookMoved);
void makeMove(Move &move);
void undoMove(const Move &move);
std::string moveToNotation(const Move &move);
std::vector<Move> generateLegalMoves(bool whiteTurn);