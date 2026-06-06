#include "ai.hpp"
#include "Board.hpp"

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

          Move m{};
          m.startRow = sr;
          m.startCol = sc;
          m.endRow = er;
          m.endCol = ec;
          m.movedPiece = piece;
          m.capturedPiece = target;
          m.wasEnPassant = false;

          makeMove(m);
          bool safe = !isKingInCheck(forWhite);
          undoMove(m);
          if (safe)
            moves.push_back(m);
        }
    }
  return moves;
}

// Piece-Square Tables for Evaluation

const int pawnTable[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},         {50, 50, 50, 50, 50, 50, 50, 50},
    {10, 10, 20, 30, 30, 20, 10, 10}, {5, 5, 10, 25, 25, 10, 5, 5},
    {0, 0, 0, 20, 20, 0, 0, 0},       {5, -5, -10, 0, 0, -10, -5, 5},
    {5, 10, 10, -20, -20, 10, 10, 5}, {0, 0, 0, 0, 0, 0, 0, 0}};

const int knightTable[8][8] = {{-50, -40, -30, -30, -30, -30, -40, -50},
                               {-40, -20, 0, 0, 0, 0, -20, -40},
                               {-30, 0, 10, 15, 15, 10, 0, -30},
                               {-30, 5, 15, 20, 20, 15, 5, -30},
                               {-30, 0, 15, 20, 20, 15, 0, -30},
                               {-30, 5, 10, 15, 15, 10, 5, -30},
                               {-40, -20, 0, 5, 5, 0, -20, -40},
                               {-50, -40, -30, -30, -30, -30, -40, -50}};

const int bishopTable[8][8] = {{-20, -10, -10, -10, -10, -10, -10, -20},
                               {-10, 0, 0, 0, 0, 0, 0, -10},
                               {-10, 0, 5, 10, 10, 5, 0, -10},
                               {-10, 5, 5, 10, 10, 5, 5, -10},
                               {-10, 0, 10, 10, 10, 10, 0, -10},
                               {-10, 10, 10, 10, 10, 10, 10, -10},
                               {-10, 5, 0, 0, 0, 0, 5, -10},
                               {-20, -10, -10, -10, -10, -10, -10, -20}};

const int rookTable[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},   {5, 10, 10, 10, 10, 10, 10, 5},
    {-5, 0, 0, 0, 0, 0, 0, -5}, {-5, 0, 0, 0, 0, 0, 0, -5},
    {-5, 0, 0, 0, 0, 0, 0, -5}, {-5, 0, 0, 0, 0, 0, 0, -5},
    {-5, 0, 0, 0, 0, 0, 0, -5}, {0, 0, 0, 5, 5, 0, 0, 0}};

const int queenTable[8][8] = {{-20, -10, -10, -5, -5, -10, -10, -20},
                              {-10, 0, 0, 0, 0, 0, 0, -10},
                              {-10, 0, 5, 5, 5, 5, 0, -10},
                              {-5, 0, 5, 5, 5, 5, 0, -5},
                              {0, 0, 5, 5, 5, 5, 0, -5},
                              {-10, 5, 5, 5, 5, 5, 0, -10},
                              {-10, 0, 5, 0, 0, 0, 0, -10},
                              {-20, -10, -10, -5, -5, -10, -10, -20}};

const int kingTable[8][8] = {{-30, -40, -40, -50, -50, -40, -40, -30},
                             {-30, -40, -40, -50, -50, -40, -40, -30},
                             {-30, -40, -40, -50, -50, -40, -40, -30},
                             {-30, -40, -40, -50, -50, -40, -40, -30},
                             {-20, -30, -30, -40, -40, -30, -30, -20},
                             {-10, -20, -20, -20, -20, -20, -20, -10},
                             {20, 20, 0, 0, 0, 0, 20, 20},
                             {20, 30, 10, 0, 0, 10, 30, 20}};

int pieceValue(char p) {
  switch (tolower(p)) {
  case 'p':
    return 100;
  case 'n':
    return 320;
  case 'b':
    return 330;
  case 'r':
    return 500;
  case 'q':
    return 900;
  case 'k':
    return 20000;
  default:
    return 0;
  }
}

int positionBonus(char piece, int row, int col) {
  int r = isWhite(piece) ? row : (7 - row);
  switch (tolower(piece)) {
  case 'p':
    return pawnTable[r][col];
  case 'n':
    return knightTable[r][col];
  case 'b':
    return bishopTable[r][col];
  case 'r':
    return rookTable[r][col];
  case 'q':
    return queenTable[r][col];
  case 'k':
    return kingTable[r][col];
  default:
    return 0;
  }
  // ai will go for promotion
  if (piece == 'p')
    return pawnTable[r][col] + (7 - r) * 20;

  if (piece == 'P')
    return pawnTable[r][col] + (7 - r) * 20;
}

// Positive = black advantage, negative = white advantage.
int evaluateBoard() {
  int score = 0;
  for (int r = 0; r < 8; r++)
    for (int c = 0; c < 8; c++) {
      char p = board[r][c];
      if (p == '.')
        continue;
      int val = pieceValue(p) + positionBonus(p, r, c);
      if (isBlack(p))
        score += val;
      else
        score -= val;
    }
  return score;
}

int minimax(int depth, bool whiteTurnNow, int alpha, int beta) {
  if (depth == 0)
    return evaluateBoard();
  if (isInsufficientMaterial())
    return 0;
  std::vector<Move> moves = getAllLegalMoves(whiteTurnNow);

  if (moves.empty()) {
    if (isKingInCheck(whiteTurnNow))
      return whiteTurnNow ? (100000 + depth) : (-100000 - depth); // checkmate
    return 0;                                                     // stalemate
  }

  if (!whiteTurnNow) {
    int best = INT_MIN;

    for (auto &m : moves) {
      makeMove(m);

      int score = minimax(depth - 1, !whiteTurnNow, alpha, beta);

      undoMove(m);

      best = std::max(best, score);

      alpha = std::max(alpha, best);

      if (alpha >= beta)
        break; // prune
    }

    return best;
  } else {
    int best = INT_MAX;

    for (auto &m : moves) {
      makeMove(m);

      int score = minimax(depth - 1, !whiteTurnNow, alpha, beta);

      undoMove(m);

      best = std::min(best, score);

      beta = std::min(beta, best);

      if (alpha >= beta)
        break; // prune
    }

    return best;
  }
}

Move getMinimaxMove(bool forWhite, int depth) {
  std::vector<Move> moves = getAllLegalMoves(forWhite);
  if (moves.empty())
    return Move{-1, -1, -1, -1, '.', '.', false};

  Move bestMove = moves[0];
  int bestScore = forWhite ? INT_MAX : INT_MIN;
  bool bestGivesCheck = false;

  for (auto &m : moves) {
    makeMove(m);
    bool givesCheck = isKingInCheck(!forWhite);
    int score = minimax(depth - 1, !forWhite, INT_MIN, INT_MAX);
    undoMove(m);

    bool better = forWhite ? (score < bestScore) : (score > bestScore);
    bool equalAndCheck = (score == bestScore) && givesCheck && !bestGivesCheck;

    if (better || equalAndCheck) {
      bestScore = score;
      bestMove = m;
      bestGivesCheck = givesCheck;
    }
  }
  return bestMove;
}
