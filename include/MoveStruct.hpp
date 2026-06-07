#pragma once
#include <string>
// #include <iostream>

struct Move {

  int startRow;
  int startCol;

  int endRow;
  int endCol;

  char movedPiece;
  char capturedPiece;

  bool wasPromotion = false;
  char promotionPiece = '.';

  bool wasCastling = false;
  bool wasEnPassant = false;

  bool isCastling = false;

  bool prevEnPassantAvailable;
  int prevEnPassantRow;
  int prevEnPassantCol;

  bool prevWhiteKingMoved;
  bool prevBlackKingMoved;

  bool prevWhiteLeftRookMoved;
  bool prevWhiteRightRookMoved;

  bool prevBlackLeftRookMoved;
  bool prevBlackRightRookMoved;

  std::string suffix; // "+", "#", or ""
};