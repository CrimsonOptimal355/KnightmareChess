#pragma once
#include <SFML/System/Vector2.hpp>

extern thread_local char board[8][8];

bool isWhite(char piece);
bool isBlack(char piece);
sf::Vector2i findKing(bool whiteKing);
void displayBoard();
extern bool enPassantAvailable;
extern int enPassantRow;
extern int enPassantCol;
extern bool aiWhiteKingMoved;
extern bool aiBlackKingMoved;
extern bool aiWhiteLeftRookMoved;
extern bool aiWhiteRightRookMoved;
extern bool aiBlackLeftRookMoved;
extern bool aiBlackRightRookMoved;
extern bool whiteTurn;