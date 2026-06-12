#include "Board.hpp"
#include "MoveStruct.hpp"

#include <SFML/Graphics.hpp>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

thread_local char board[8][8] = {{'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
                                 {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
                                 {'.', '.', '.', '.', '.', '.', '.', '.'},
                                 {'.', '.', '.', '.', '.', '.', '.', '.'},
                                 {'.', '.', '.', '.', '.', '.', '.', '.'},
                                 {'.', '.', '.', '.', '.', '.', '.', '.'},
                                 {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
                                 {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}};
bool isWhite(char piece) { return std::isupper(piece); }
bool isBlack(char piece) { return std::islower(piece); }
sf::Vector2i findKing(bool whiteKing) {
  char king = whiteKing ? 'K' : 'k';

  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      if (board[row][col] == king) {
        return {row, col};
      }
    }
  }
  return {-1, -1};
}
void displayBoard() {
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      std::cout << board[row][col] << " ";
    }

    std::cout << std::endl;
  }
}
bool enPassantAvailable = false;
int enPassantRow = -1;
int enPassantCol = -1;
bool whiteTurn = true;
bool aiWhiteKingMoved = false;
bool aiBlackKingMoved = false;
bool aiWhiteLeftRookMoved = false;
bool aiWhiteRightRookMoved = false;
bool aiBlackLeftRookMoved = false;
bool aiBlackRightRookMoved = false;