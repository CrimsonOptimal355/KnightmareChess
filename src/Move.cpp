// Move.cpp implementation
#include "Move.hpp"
#include "Board.hpp"
#include "MoveStruct.hpp"

#include <cmath>
#include <iostream>
#include <string>

bool isValidPawnMove(int startRow, int startCol, int endRow, int endCol,
                     char sourcepiece, char targetpiece) {
  if (sourcepiece == 'P') {
    if (endCol == startCol && endRow == startRow - 1 &&
        board[endRow][endCol] == '.')
      return true;
    if (startRow == 6 && endCol == startCol && endRow == startRow - 2 &&
        board[startRow - 1][startCol] == '.' && board[endRow][endCol] == '.')
      return true;
    if (endRow == startRow - 1 && std::abs(endCol - startCol) == 1) {
      // normal capture
      if (isBlack(targetpiece))
        return true;

      // en passant
      if (enPassantAvailable && endRow == enPassantRow &&
          endCol == enPassantCol && board[startRow][endCol] == 'p') {
        return true;
      }
    }
  }
  if (sourcepiece == 'p') {
    if (endCol == startCol && endRow == startRow + 1 &&
        board[endRow][endCol] == '.')
      return true;
    if (startRow == 1 && endCol == startCol && endRow == startRow + 2 &&
        board[startRow + 1][startCol] == '.' && board[endRow][endCol] == '.')
      return true;
    if (endRow == startRow + 1 && std::abs(endCol - startCol) == 1) {
      // normal capture
      if (isWhite(targetpiece))
        return true;

      // en passant
      if (enPassantAvailable && endRow == enPassantRow &&
          endCol == enPassantCol && board[startRow][endCol] == 'P') {
        return true;
      }
    }
  }
  return false;
}
bool isPathClear(int startRow, int startCol, int endRow, int endCol) {
  int rowDirection = 0;
  int colDirection = 0;

  // Vertical
  if (startCol == endCol) {
    rowDirection = (endRow > startRow) ? 1 : -1;
  }

  // Horizontal
  else if (startRow == endRow) {
    colDirection = (endCol > startCol) ? 1 : -1;
  }

  // Diagonal
  else {
    rowDirection = (endRow > startRow) ? 1 : -1;

    colDirection = (endCol > startCol) ? 1 : -1;
  }
  int currentRow = startRow + rowDirection;
  int currentCol = startCol + colDirection;

  while (currentRow != endRow || currentCol != endCol) {
    if (board[currentRow][currentCol] != '.') {
      return false;
    }

    currentRow += rowDirection;
    currentCol += colDirection;
  }

  return true;
}
bool isValidRookMove(int startRow, int startCol, int endRow, int endCol,
                     char sourcepiece, char targetpiece) {
  // Horizontal move
  if (startRow == endRow) {
    return isPathClear(startRow, startCol, endRow, endCol);
  }

  // Vertical move
  if (startCol == endCol) {
    return isPathClear(startRow, startCol, endRow, endCol);
  }
  // even though above both have same return statement , they are seperate to
  // not ot allow diagonal movenent of rook
  return false;
}
bool isValidBishopMove(int startRow, int startCol, int endRow, int endCol,
                       char sourcepiece, char targetpiece) {
  // Diagonal Move
  if (std::abs(endRow - startRow) == std::abs(endCol - startCol))
    return isPathClear(startRow, startCol, endRow, endCol);

  return false;
}
bool isValidKnightMove(int startRow, int startCol, int endRow, int endCol,
                       char sourcepiece, char targetpiece) {
  // Knight Move
  if ((std::abs(endRow - startRow) == 2 && std::abs(endCol - startCol) == 1) ||
      (std::abs(endRow - startRow) == 1 && std::abs(endCol - startCol) == 2))
    return true;

  return false;
}
bool isValidQueenMove(int startRow, int startCol, int endRow, int endCol,
                      char sourcepiece, char targetpiece) {
  // Bishop + Rook Move
  // Diagonal
  if (std::abs(endRow - startRow) == std::abs(endCol - startCol))
    return isPathClear(startRow, startCol, endRow, endCol);
  // Horizontal move
  if (startRow == endRow) {
    return isPathClear(startRow, startCol, endRow, endCol);
  }
  // Vertical move
  if (startCol == endCol) {
    return isPathClear(startRow, startCol, endRow, endCol);
  }

  return false;
}
bool isValidKingMove(int startRow, int startCol, int endRow, int endCol,
                     char sourcepiece, char targetpiece) {
  // Only One Side Tile
  if ((endRow - startRow == 0 && std::abs(endCol - startCol) == 1) ||
      (endCol - startCol == 0 && std::abs(endRow - startRow) == 1) ||
      (std::abs(endCol - startCol) == 1 && std::abs(endRow - startRow) == 1))
    return true;

  return false;
}

bool isKingInCheck(bool whiteKing) {
  char targetKing = whiteKing ? 'K' : 'k';
  int kRow = -1, kCol = -1;

  // 1. Find King
  for (int r = 0; r < 8; r++) {
    for (int c = 0; c < 8; c++) {
      if (board[r][c] == targetKing) {
        kRow = r;
        kCol = c;
        break;
      }
    }
    if (kRow != -1)
      break;
  }

  // 2. Scan for enemy
  for (int r = 0; r < 8; r++) {
    for (int c = 0; c < 8; c++) {
      char piece = board[r][c];
      if (piece == '.' || (whiteKing ? !isBlack(piece) : !isWhite(piece)))
        continue;

      char lowerPiece = std::tolower(piece);

      // pawn  logic
      if (lowerPiece == 'p') {
        int direction = whiteKing ? 1 : -1;
        if (kRow == r + direction && std::abs(kCol - c) == 1)
          return true;
        continue;
      }

      // Route slider and jumper pieces using their lower-case identifiers
      bool isCheck = false;
      isCheck = isValidMove(r, c, kRow, kCol, piece, targetKing);
      if (isCheck)
        return true;
    }
  }
  return false;
}

bool isAnyLegalMove(bool whiteTurn) {
  for (int sRow = 0; sRow < 8; sRow++) {
    for (int sCol = 0; sCol < 8; sCol++) {
      char piece = board[sRow][sCol];

      // skip empty
      if (piece == '.' || (whiteTurn ? !isWhite(piece) : !isBlack(piece)))
        continue;

      for (int eRow = 0; eRow < 8; eRow++) {
        for (int eCol = 0; eCol < 8; eCol++) {
          char target = board[eRow][eCol];

          // Not Attack on own piece
          if (target != '.' && (isWhite(piece) == isWhite(target)))
            continue;

          // Different attack for each piece
          bool validMove = false;
          char type = std::tolower(piece);

          validMove = isValidMove(sRow, sCol, eRow, eCol, piece, target);

          if (!validMove)
            continue;

          // Temp Move
          board[eRow][eCol] = piece;
          board[sRow][sCol] = '.';

          bool kingUnsafe = isKingInCheck(whiteTurn);

          // Reverse
          board[sRow][sCol] = piece;
          board[eRow][eCol] = target;

          if (!kingUnsafe)
            return true;
        }
      }
    }
  }
  return false;
}
// All Pieces Valid Movees in One Fkn
bool isValidMove(int startRow, int startCol, int endRow, int endCol,
                 char sourcePiece, char targetPiece) {
  if (sourcePiece == 'P' || sourcePiece == 'p') {
    return isValidPawnMove(startRow, startCol, endRow, endCol, sourcePiece,
                           targetPiece);
  }

  if (sourcePiece == 'R' || sourcePiece == 'r') {
    return isValidRookMove(startRow, startCol, endRow, endCol, sourcePiece,
                           targetPiece);
  }

  if (sourcePiece == 'B' || sourcePiece == 'b') {
    return isValidBishopMove(startRow, startCol, endRow, endCol, sourcePiece,
                             targetPiece);
  }

  if (sourcePiece == 'N' || sourcePiece == 'n') {
    return isValidKnightMove(startRow, startCol, endRow, endCol, sourcePiece,
                             targetPiece);
  }

  if (sourcePiece == 'Q' || sourcePiece == 'q') {
    return isValidQueenMove(startRow, startCol, endRow, endCol, sourcePiece,
                            targetPiece);
  }

  if (sourcePiece == 'K' || sourcePiece == 'k') {
    return isValidKingMove(startRow, startCol, endRow, endCol, sourcePiece,
                           targetPiece);
  }

  return false;
}

bool isValidCastling(int startRow, int startCol, int endRow, int endCol,
                     char sourcePiece, bool whiteKingMoved, bool blackKingMoved,
                     bool whiteLeftRookMoved, bool whiteRightRookMoved,
                     bool blackLeftRookMoved, bool blackRightRookMoved) {
  // only king
  if ((sourcePiece != 'K' && sourcePiece != 'k') || startCol != 4)
    return false;

  bool isWhite = (sourcePiece == 'K');
  int targetRow = isWhite ? 7 : 0;

  // king moving on same row
  if (startRow != targetRow || endRow != targetRow)
    return false;

  // king's first move
  if (isWhite ? whiteKingMoved : blackKingMoved)
    return false;

  // short castling
  if (endCol == 6) {
    bool rookMoved = isWhite ? whiteRightRookMoved : blackRightRookMoved;
    if (!rookMoved && board[targetRow][5] == '.' &&
        board[targetRow][6] == '.') {

      // Rule 1
      if (isKingInCheck(isWhite))
        return false;

      // Rule 2
      board[targetRow][4] = '.';
      board[targetRow][5] = (isWhite ? 'K' : 'k');
      bool throughCheck = isKingInCheck(isWhite);
      board[targetRow][5] = '.';
      board[targetRow][4] = (isWhite ? 'K' : 'k');
      if (throughCheck)
        return false;

      // Rule 3
      board[targetRow][4] = '.';
      board[targetRow][6] = (isWhite ? 'K' : 'k');
      bool landCheck = isKingInCheck(isWhite);
      board[targetRow][6] = '.';
      board[targetRow][4] = (isWhite ? 'K' : 'k');
      if (landCheck)
        return false;

      return true;
    }
  }

  // long castling
  if (endCol == 2) {
    bool rookMoved = isWhite ? whiteLeftRookMoved : blackLeftRookMoved;
    if (!rookMoved && board[targetRow][1] == '.' &&
        board[targetRow][2] == '.' && board[targetRow][3] == '.') {

      // Rule 1
      if (isKingInCheck(isWhite))
        return false;

      // Rule 2
      board[targetRow][4] = '.';
      board[targetRow][3] = (isWhite ? 'K' : 'k');
      bool throughCheck = isKingInCheck(isWhite);
      board[targetRow][3] = '.';
      board[targetRow][4] = (isWhite ? 'K' : 'k');
      if (throughCheck)
        return false;

      // Rule 3
      board[targetRow][4] = '.';
      board[targetRow][2] = (isWhite ? 'K' : 'k');
      bool landCheck = isKingInCheck(isWhite);
      board[targetRow][2] = '.';
      board[targetRow][4] = (isWhite ? 'K' : 'k');
      if (landCheck)
        return false;

      return true;
    }
  }

  return false;
}
void makeMove(Move &move) {
  move.wasEnPassant = false;
  move.prevEnPassantAvailable = enPassantAvailable;
  move.prevEnPassantRow = enPassantRow;
  move.prevEnPassantCol = enPassantCol;
  char piece = board[move.startRow][move.startCol];

  if ((piece == 'P' || piece == 'p') && move.endRow == enPassantRow &&
      move.endCol == enPassantCol && board[move.endRow][move.endCol] == '.') {
    move.wasEnPassant = true;
    if (piece == 'P') {
      board[move.endRow + 1][move.endCol] = '.';
    } else {
      board[move.endRow - 1][move.endCol] = '.';
    }
  }

  board[move.endRow][move.endCol] = piece;
  board[move.startRow][move.startCol] = '.';

  // reset
  enPassantAvailable = false;

  // white double push
  if (piece == 'P' && move.startRow == 6 && move.endRow == 4) {
    enPassantAvailable = true;
    enPassantRow = 5;
    enPassantCol = move.startCol;
  }

  // black double push
  if (piece == 'p' && move.startRow == 1 && move.endRow == 3) {
    enPassantAvailable = true;
    enPassantRow = 2;
    enPassantCol = move.startCol;
  }
}
void undoMove(const Move &move) {
  board[move.startRow][move.startCol] = move.movedPiece;
  board[move.endRow][move.endCol] = move.capturedPiece;

  if (move.wasEnPassant) {
    if (move.movedPiece == 'P') {
      board[move.endRow + 1][move.endCol] = 'p';
    } else {
      board[move.endRow - 1][move.endCol] = 'P';
    }
  }
  enPassantAvailable = move.prevEnPassantAvailable;
  enPassantRow = move.prevEnPassantRow;
  enPassantCol = move.prevEnPassantCol;
}
std::string moveToNotation(const Move &move) {
  std::string notation;

  char piece = std::toupper(move.movedPiece);

  // king castle
  if (std::toupper(move.movedPiece) == 'K' && move.startCol == 4 &&
      move.endCol == 6) {
    return "O-O";
  }

  // queen castle
  if (std::toupper(move.movedPiece) == 'K' && move.startCol == 4 &&
      move.endCol == 2) {
    return "O-O-O";
  }

  // pawn
  if (piece != 'P') {
    notation += piece;
  }

  // capture
  if (move.capturedPiece != '.') {
    // Pawn
    if (piece == 'P') {
      notation += char('a' + move.startCol);
    }

    notation += 'x';
  }

  notation += char('a' + move.endCol);

  notation += char('8' - move.endRow);

  return notation;
}
