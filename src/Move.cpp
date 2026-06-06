// Move.cpp implementation
#include "Move.hpp"
#include "Board.hpp"
#include "MoveStruct.hpp"
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

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
  // Horiz
  if (startRow == endRow) {
    return isPathClear(startRow, startCol, endRow, endCol);
  }

  // Vert
  if (startCol == endCol) {
    return isPathClear(startRow, startCol, endRow, endCol);
  }
  // even though above both have same return statement , they are seperate to
  // not ot allow diagonal movenent of rook
  return false;
}
bool isValidBishopMove(int startRow, int startCol, int endRow, int endCol,
                       char sourcepiece, char targetpiece) {
  // Diag
  if (std::abs(endRow - startRow) == std::abs(endCol - startCol))
    return isPathClear(startRow, startCol, endRow, endCol);

  return false;
}
bool isValidKnightMove(int startRow, int startCol, int endRow, int endCol,
                       char sourcepiece, char targetpiece) {
  // Knight
  if ((std::abs(endRow - startRow) == 2 && std::abs(endCol - startCol) == 1) ||
      (std::abs(endRow - startRow) == 1 && std::abs(endCol - startCol) == 2))
    return true;

  return false;
}
bool isValidQueenMove(int startRow, int startCol, int endRow, int endCol,
                      char sourcepiece, char targetpiece) {
  // Bishop + Rook Move
  // Diag
  if (std::abs(endRow - startRow) == std::abs(endCol - startCol))
    return isPathClear(startRow, startCol, endRow, endCol);
  // Horiz
  if (startRow == endRow) {
    return isPathClear(startRow, startCol, endRow, endCol);
  }
  // Vert
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

  // king loc
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

  // enemy loc
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

      if (piece == '.' || (whiteTurn ? !isWhite(piece) : !isBlack(piece)))
        continue;

      for (int eRow = 0; eRow < 8; eRow++) {
        for (int eCol = 0; eCol < 8; eCol++) {
          char target = board[eRow][eCol];

          if (target != '.' && (isWhite(piece) == isWhite(target)))
            continue;

          if (!isValidMove(sRow, sCol, eRow, eCol, piece, target))
            continue;

          Move move;
          move.startRow = sRow;
          move.startCol = sCol;
          move.endRow = eRow;
          move.endCol = eCol;
          move.movedPiece = piece;
          move.capturedPiece = target;
          move.wasEnPassant = false;
          move.prevEnPassantAvailable = false;
          move.prevEnPassantRow = -1;
          move.prevEnPassantCol = -1;

          makeMove(move);
          bool kingUnsafe = isKingInCheck(whiteTurn);
          undoMove(move);

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

namespace {
bool isInsideBoard(int row, int col) {
  return row >= 0 && row < 8 && col >= 0 && col < 8;
}
} // namespace

void makeMove(Move &move) {
  if (!isInsideBoard(move.startRow, move.startCol) ||
      !isInsideBoard(move.endRow, move.endCol)) {
    return;
  }
  move.wasEnPassant = false;
  move.prevEnPassantAvailable = enPassantAvailable;
  move.prevEnPassantRow = enPassantRow;
  move.prevEnPassantCol = enPassantCol;
  char piece = board[move.startRow][move.startCol];

  if ((piece == 'P' || piece == 'p') && move.endRow == enPassantRow &&
      move.endCol == enPassantCol && board[move.endRow][move.endCol] == '.') {

    move.wasEnPassant = true;

    if (piece == 'P') {
      move.capturedPiece = board[move.endRow + 1][move.endCol];

      board[move.endRow + 1][move.endCol] = '.';
    } else {
      move.capturedPiece = board[move.endRow - 1][move.endCol];

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
  if (board[move.endRow][move.endCol] == 'P' && move.endRow == 0)
    board[move.endRow][move.endCol] = 'Q';
  if (board[move.endRow][move.endCol] == 'p' && move.endRow == 7)
    board[move.endRow][move.endCol] = 'q';
}
void undoMove(const Move &move) {
  if (!isInsideBoard(move.startRow, move.startCol) ||
      !isInsideBoard(move.endRow, move.endCol)) {
    return;
  }

  board[move.startRow][move.startCol] = move.movedPiece;
  board[move.endRow][move.endCol] = move.capturedPiece;

  if (move.wasEnPassant) {

    board[move.endRow][move.endCol] = '.';

    if (move.movedPiece == 'P') {
      board[move.endRow + 1][move.endCol] = move.capturedPiece;
    } else {
      board[move.endRow - 1][move.endCol] = move.capturedPiece;
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
std::vector<Move> generateLegalMoves(bool whiteTurn) {
  std::vector<Move> legalMoves;

  return legalMoves;
}
/*Insuff draw
  1) King + Bishop vs King
  2) King + Knight vs King
  3) King vs King
  4) King + Bishop vs King + Bishop (on same color tile)*/

bool isInsufficientMaterial() {
  // count peices
  int whiteKings = 0, blackKings = 0;
  int whiteBishops = 0, blackBishops = 0;
  int whiteKnights = 0, blackKnights = 0;
  int whiteOther = 0, blackOther = 0;

  // getting bishop tile colour
  int whiteBishopSquareColor = -1;
  int blackBishopSquareColor = -1;

  for (int r = 0; r < 8; r++) {
    for (int c = 0; c < 8; c++) {
      char p = board[r][c];
      if (p == '.')
        continue;

      switch (p) {
      case 'K':
        whiteKings++;
        break;
      case 'k':
        blackKings++;
        break;

      case 'B':
        whiteBishops++;
        whiteBishopSquareColor = (r + c) % 2;
        break;
      case 'b':
        blackBishops++;
        blackBishopSquareColor = (r + c) % 2;
        break;

      case 'N':
        whiteKnights++;
        break;
      case 'n':
        blackKnights++;
        break;

      // anything else = mating material
      case 'Q':
      case 'R':
      case 'P':
        whiteOther++;
        break;
      case 'q':
      case 'r':
      case 'p':
        blackOther++;
        break;
      }
    }
  }

  // If any other , no draw
  if (whiteOther > 0 || blackOther > 0)
    return false;

  // if more than one bihsop,knight or both , no draw.
  int whiteMinors = whiteBishops + whiteKnights;
  int blackMinors = blackBishops + blackKnights;

  // K vs K
  if (whiteMinors == 0 && blackMinors == 0)
    return true;

  // K+B vs K
  if (whiteBishops == 1 && whiteKnights == 0 && blackMinors == 0)
    return true;

  // K vs K+B
  if (blackBishops == 1 && blackKnights == 0 && whiteMinors == 0)
    return true;

  // K+N vs K
  if (whiteKnights == 1 && whiteBishops == 0 && blackMinors == 0)
    return true;

  // K vs K+N
  if (blackKnights == 1 && blackBishops == 0 && whiteMinors == 0)
    return true;

  // K+B vs K+B (same square color bishops)
  if (whiteBishops == 1 && blackBishops == 1 && whiteKnights == 0 &&
      blackKnights == 0 && whiteBishopSquareColor == blackBishopSquareColor)
    return true;

  return false;
}
