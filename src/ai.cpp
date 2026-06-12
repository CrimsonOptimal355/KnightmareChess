#include "ai.hpp"
#include "Board.hpp"
#include <algorithm>

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
          if (sr == er && sc == ec)
            continue;
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
  // castling
  for (int sr = 0; sr < 8; sr++) {
    for (int sc = 0; sc < 8; sc++) {
      char piece = board[sr][sc];
      if ((forWhite && piece != 'K') || (!forWhite && piece != 'k'))
        continue;

      for (int endCol : {2, 6}) {
        if (isValidCastling(sr, sc, sr, endCol, piece, aiWhiteKingMoved,
                            aiBlackKingMoved, aiWhiteLeftRookMoved,
                            aiWhiteRightRookMoved, aiBlackLeftRookMoved,
                            aiBlackRightRookMoved)) {
          Move m{};
          m.startRow = sr;
          m.startCol = sc;
          m.endRow = sr;
          m.endCol = endCol;
          m.movedPiece = piece;
          m.capturedPiece = '.';
          m.wasEnPassant = false;
          moves.push_back(m);
        }
      }
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
    if (piece == 'P')
      return pawnTable[r][col] + (7 - row) * 25; // oppose white promo
    else
      return pawnTable[r][col] + row * 25; // go for promo
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
}

// Positive = black advantage, negative = white advantage.
int evaluateBoard() {
  int score = 0;
  int whiteKingRow = -1, whiteKingCol = -1;
  int blackKingRow = -1, blackKingCol = -1;
  int whiteMaterial = 0, blackMaterial = 0;
  for (int r = 0; r < 8; r++)
    for (int c = 0; c < 8; c++) {
      char p = board[r][c];
      if (p == '.')
        continue;
      int val = pieceValue(p) + positionBonus(p, r, c);
      if (isBlack(p)) {
        score += val;
        blackMaterial += pieceValue(p);
      } else {
        score -= val;
        whiteMaterial += pieceValue(p);
      }

      if (p == 'K') {
        whiteKingRow = r;
        whiteKingCol = c;
      } else if (p == 'k') {
        blackKingRow = r;
        blackKingCol = c;
      }
    }
  // when only king is remianign , push it to cornere
  if (whiteKingRow != -1 && blackKingRow != -1) {
    int diff = blackMaterial - whiteMaterial;

    if (diff > 500) {
      // push white king
      int edgeDist = std::max(
          {whiteKingRow, 7 - whiteKingRow, whiteKingCol, 7 - whiteKingCol});
      int kingDist = std::abs(whiteKingRow - blackKingRow) +
                     std::abs(whiteKingCol - blackKingCol);
      score += edgeDist * 50;
      score -= kingDist * 25;
    }

    if (diff < -500) {
      // push black king
      int edgeDist = std::max(
          {blackKingRow, 7 - blackKingRow, blackKingCol, 7 - blackKingCol});
      int kingDist = std::abs(whiteKingRow - blackKingRow) +
                     std::abs(whiteKingCol - blackKingCol);
      score -= edgeDist * 50;
      score += kingDist * 25;
    }
  }

  return score;
}
// algo to solve horizon effect.like if its on d =
//     3 and at last search its capturing oppo piece and
//     thinks its winning despite being not able to see loss after this dues
//     depth
//         restriction.hece this algo will help to stop at quiet moves
int quiescence(int alpha, int beta, bool whiteTurnNow, int qDepth = 4) {
  if (qDepth == 0)
    return evaluateBoard();
  int standPat = evaluateBoard();

  if (!whiteTurnNow) {
    if (standPat >= beta)
      return beta;

    alpha = std::max(alpha, standPat);
  } else {
    if (standPat <= alpha)
      return alpha;

    beta = std::min(beta, standPat);
  }

  auto moves = getAllLegalMoves(whiteTurnNow);

  std::sort(moves.begin(), moves.end(), [](const Move &a, const Move &b) {
    return (pieceValue(a.capturedPiece)) > (pieceValue(b.capturedPiece));
  });

  for (auto &m : moves) {
    if (m.capturedPiece == '.')
      continue;

    // skip if capture not useful
    int captureVal = pieceValue(m.capturedPiece);
    if (!whiteTurnNow && standPat + captureVal + 200 < alpha)
      continue;
    if (whiteTurnNow && standPat - captureVal - 200 > beta)
      continue;

    makeMove(m);
    int score = quiescence(alpha, beta, !whiteTurnNow, qDepth - 1);
    undoMove(m);

    if (!whiteTurnNow) {
      alpha = std::max(alpha, score);
      if (alpha >= beta)
        break;
    } else {
      beta = std::min(beta, score);
      if (alpha >= beta)
        break;
    }
  }

  return whiteTurnNow ? beta : alpha;
}

int minimax(int depth, bool whiteTurnNow, int alpha, int beta) {
  if (depth == 0)
    return quiescence(alpha, beta, whiteTurnNow);
  // return evaluateBoard();
  if (isInsufficientMaterial())
    return 0;
  std::vector<Move> moves = getAllLegalMoves(whiteTurnNow);
  std::sort(moves.begin(), moves.end(), [](const Move &a, const Move &b) {
    return pieceValue(a.capturedPiece) > pieceValue(b.capturedPiece);
  });

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
  std::sort(moves.begin(), moves.end(), [](const Move &a, const Move &b) {
    return pieceValue(a.capturedPiece) > pieceValue(b.capturedPiece);
  });
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
    if (!forWhite && bestScore >= 99000)
      break;
    if (forWhite && bestScore <= -99000)
      break;
  }
  return bestMove;
}