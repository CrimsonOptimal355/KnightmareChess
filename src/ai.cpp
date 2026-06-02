#include "ai.hpp"
#include "Board.hpp"
#include <cstdlib>
#include <ctime>

Move getRandomMove(bool forWhite) {
  std::vector<Move> moves = getAllLegalMoves(forWhite);

  if (moves.empty())
    return Move{-1, -1, -1, -1, '.', '.', false};

  int idx = rand() % moves.size();
  return moves[idx];
}

std::vector<Move> getAllLegalMoves(bool forWhite) {
  std::vector<Move> moves;
  for (int sr = 0; sr < 8; sr++)
    for (int sc = 0; sc < 8; sc++) {
      char piece = board[sr][sc];
      if (piece == '.')
        continue;
      if (forWhite && isBlack(piece))
        continue;
      if (!forWhite && isWhite(piece))
        continue;

      for (int er = 0; er < 8; er++)
        for (int ec = 0; ec < 8; ec++) {
          char target = board[er][ec];
          if ((isWhite(piece) && isWhite(target)) ||
              (isBlack(piece) && isBlack(target)))
            continue;
          if (!isValidMove(sr, sc, er, ec, piece, target))
            continue;

          Move m{sr, sc, er, ec, piece, target, false};
          makeMove(m);
          bool safe = !isKingInCheck(forWhite);
          undoMove(m);
          if (safe)
            moves.push_back(m);
        }
    }
  return moves;
}