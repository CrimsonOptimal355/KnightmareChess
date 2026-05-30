#pragma once

struct Move {
  int startRow;
  int startCol;

  int endRow;
  int endCol;

  char movedPiece;
  char capturedPiece;

  bool castling = false;
  bool promotion = false;
  bool wasEnPassant = false;
  bool prevEnPassantAvailable;
  int prevEnPassantRow;
  int prevEnPassantCol;
};
