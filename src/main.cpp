// Board.cpp implementation
#include "Board.hpp"
#include "Move.hpp"
#include "MoveStruct.hpp"
#include "ai.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <atomic>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

enum class GameMode { None, PvP, PvAI };
GameMode gameMode = GameMode::None;
bool showMainMenu = true;

void loadTexture(std::unordered_map<char, sf::Texture> &textures, char key,
                 const std::string &path) {
  if (!textures[key].loadFromFile(path)) {
    std::cerr << "Failed to load: " << path << std::endl;
  }
}
void loadSound(sf::SoundBuffer &Buffer, const std::string &path) {
  if (!Buffer.loadFromFile(path)) {
    std::cerr << "Falied to load: " << path << std::endl;
  }
}

int main() {
  srand(time(0));

  const float windowHeight = 1800.f;
  const float windowWidth = 1800.f;

  sf::RenderWindow window(
      sf::VideoMode({static_cast<unsigned int>(windowWidth),
                     static_cast<unsigned int>(windowHeight)}),
      "Chess");

  const int numRows = 8;
  const int numCols = 8;

  const float tileSize = 125.f;

  const float totalGridWidth = numCols * tileSize;
  const float totalGridHeight = numRows * tileSize;

  const float offsetX = (windowWidth - totalGridWidth) / 2.f;
  const float offsetY = (windowHeight - totalGridHeight) / 2.f;

  sf::RectangleShape square(sf::Vector2f(tileSize, tileSize));

  // From Array to Board
  std::unordered_map<char, sf::Texture> textures;
  // White Pieces
  loadTexture(textures, 'R', "assets/textures/wr.png");
  loadTexture(textures, 'N', "assets/textures/wn.png");
  loadTexture(textures, 'B', "assets/textures/wb.png");
  loadTexture(textures, 'Q', "assets/textures/wq.png");
  loadTexture(textures, 'K', "assets/textures/wk.png");
  loadTexture(textures, 'P', "assets/textures/wp.png");
  // Black Pieces
  loadTexture(textures, 'r', "assets/textures/br.png");
  loadTexture(textures, 'n', "assets/textures/bn.png");
  loadTexture(textures, 'b', "assets/textures/bb.png");
  loadTexture(textures, 'q', "assets/textures/bq.png");
  loadTexture(textures, 'k', "assets/textures/bk.png");
  loadTexture(textures, 'p', "assets/textures/bp.png");
  // Sounds
  sf::SoundBuffer moveBuffer;
  sf::SoundBuffer captureBuffer;
  sf::SoundBuffer castleBuffer;
  sf::SoundBuffer checkBuffer;
  sf::SoundBuffer checkmateBuffer;
  loadSound(moveBuffer, "assets/sounds/move.mp3");
  loadSound(captureBuffer, "assets/sounds/capture.mp3");
  loadSound(castleBuffer, "assets/sounds/castle.mp3");
  loadSound(checkBuffer, "assets/sounds/check.mp3");
  loadSound(checkmateBuffer, "assets/sounds/checkmate.mp3");
  sf::Sound moveSound(moveBuffer);
  sf::Sound captureSound(captureBuffer);
  sf::Sound castleSound(castleBuffer);
  sf::Sound checkSound(checkBuffer);
  sf::Sound checkmateSound(checkmateBuffer);

  sf::Font font;
  if (!font.openFromFile("assets/fonts/UbuntuMono-Regular.ttf")) {
    std::cerr << "Failed\n";
  }
  // Highlighting Box
  sf::RectangleShape boardbox(sf::Vector2f(tileSize, tileSize));
  boardbox.setOutlineThickness(0.f);
  sf::Color newGreen(114, 148, 105);
  boardbox.setFillColor(newGreen);

  // Changed Box Highlighting
  sf::RectangleShape boardbox1(sf::Vector2f(tileSize, tileSize));
  boardbox1.setOutlineThickness(0.1f);
  sf::Color newyellow(196, 212, 101);
  boardbox1.setFillColor(newyellow);

  // Checked Box Highlighting
  sf::RectangleShape boardbox2(sf::Vector2f(tileSize, tileSize));
  boardbox2.setOutlineThickness(0.1f);
  sf::Color newred(180, 40, 30, 180);
  boardbox2.setFillColor(newred);

  // Restart Button
  sf::RectangleShape restartButton(sf::Vector2f(180.f, 70.f));
  restartButton.setFillColor(sf::Color(70, 70, 70));
  restartButton.setPosition({offsetX - 320.f, offsetY + 100.f});

  sf::Text restartText(font);
  restartText.setCharacterSize(32);
  restartText.setFillColor(sf::Color::White);
  restartText.setString("Restart");
  restartText.setPosition({offsetX - 285.f, offsetY + 115.f});

  // MainMenu Backgroung
  sf::RectangleShape menuBackground(sf::Vector2f(windowWidth, windowHeight));
  menuBackground.setFillColor(sf::Color(74, 35, 10));
  menuBackground.setPosition({0.f, 0.f});

  // MainMenu Text
  sf::Text menuTitle(font);
  menuTitle.setCharacterSize(80);
  menuTitle.setFillColor(sf::Color(185, 148, 82));
  menuTitle.setStyle(sf::Text::Bold);
  menuTitle.setString("KNIGHTMARE CHESS");
  sf::FloatRect titleBounds = menuTitle.getLocalBounds();
  menuTitle.setOrigin({titleBounds.position.x + titleBounds.size.x / 2.f,
                       titleBounds.position.y + titleBounds.size.y / 2.f});
  menuTitle.setPosition({windowWidth / 2.f, windowHeight / 3.f});

  // MainMenu Buttons
  // PvP
  sf::RectangleShape pvpButton(sf::Vector2f(580.f, 110.f));
  pvpButton.setFillColor(sf::Color(185, 148, 82));
  pvpButton.setOutlineColor(sf::Color(210, 175, 100));
  pvpButton.setOutlineThickness(3.f);
  pvpButton.setPosition(
      {windowWidth / 2.f - 290.f, windowHeight / 2.f - 130.f});

  sf::Text pvpText(font);
  pvpText.setCharacterSize(42);
  pvpText.setFillColor(sf::Color(40, 20, 5));
  pvpText.setStyle(sf::Text::Bold);
  pvpText.setString("PLAYER  vs  PLAYER");
  sf::FloatRect pvpBounds = pvpText.getLocalBounds();
  pvpText.setOrigin({pvpBounds.position.x + pvpBounds.size.x / 2.f,
                     pvpBounds.position.y + pvpBounds.size.y / 2.f});
  pvpText.setPosition({windowWidth / 2.f, windowHeight / 2.f - 75.f});

  // PvAI
  sf::RectangleShape pvaiButton(sf::Vector2f(580.f, 110.f));
  pvaiButton.setFillColor(sf::Color(185, 148, 82));
  pvaiButton.setOutlineColor(sf::Color(210, 175, 100));
  pvaiButton.setOutlineThickness(3.f);
  pvaiButton.setPosition(
      {windowWidth / 2.f - 290.f, windowHeight / 2.f + 20.f});

  sf::Text pvaiText(font);
  pvaiText.setCharacterSize(42);
  pvaiText.setFillColor(sf::Color(40, 20, 5));
  pvaiText.setStyle(sf::Text::Bold);
  pvaiText.setString("PLAYER  vs  AI");
  sf::FloatRect pvaiBounds = pvaiText.getLocalBounds();
  pvaiText.setOrigin({pvaiBounds.position.x + pvaiBounds.size.x / 2.f,
                      pvaiBounds.position.y + pvaiBounds.size.y / 2.f});
  pvaiText.setPosition({windowWidth / 2.f, windowHeight / 2.f + 75.f});

  // Final Result Window
  sf::RectangleShape gameOverPanel(sf::Vector2f(500.f, 300.f));
  gameOverPanel.setFillColor(sf::Color(40, 40, 40, 230));
  gameOverPanel.setPosition(
      {windowWidth / 2.f - 250.f, windowHeight / 2.f - 150.f});

  sf::Text resultText(font);
  resultText.setCharacterSize(50);
  resultText.setFillColor(sf::Color::White);

  // Rematch
  sf::RectangleShape rematchButton(sf::Vector2f(180.f, 70.f));
  rematchButton.setFillColor(sf::Color(70, 70, 70));
  rematchButton.setPosition(
      {windowWidth / 2.f - 210.f, windowHeight / 2.f + 40.f});

  sf::Text rematchText(font);
  rematchText.setCharacterSize(30);
  rematchText.setString("Rematch");
  rematchText.setFillColor(sf::Color::White);
  rematchText.setPosition(
      {windowWidth / 2.f - 180.f, windowHeight / 2.f + 55.f});

  // Exit
  sf::RectangleShape exitButton(sf::Vector2f(180.f, 70.f));
  exitButton.setFillColor(sf::Color(70, 70, 70));
  exitButton.setPosition({windowWidth / 2.f + 30.f, windowHeight / 2.f + 40.f});

  sf::Text exitText(font);
  exitText.setCharacterSize(30);
  exitText.setString("Exit");
  exitText.setFillColor(sf::Color::White);
  exitText.setPosition({windowWidth / 2.f + 90.f, windowHeight / 2.f + 55.f});

  // Promotion
  std::unordered_map<char, sf::Texture> texture_promo;
  // White Pieces
  loadTexture(texture_promo, 'R', "assets/textures/wr.png");
  loadTexture(texture_promo, 'N', "assets/textures/wn.png");
  loadTexture(texture_promo, 'B', "assets/textures/wb.png");
  loadTexture(texture_promo, 'Q', "assets/textures/wq.png");
  // Black Piecese
  loadTexture(texture_promo, 'r', "assets/textures/br.png");
  loadTexture(texture_promo, 'n', "assets/textures/bn.png");
  loadTexture(texture_promo, 'b', "assets/textures/bb.png");
  loadTexture(texture_promo, 'q', "assets/textures/bq.png");

  std::vector<char> promotionOptions;
  // promotionOptions = {'Q','R','B','N'};
  // promotionOptions = {'q','r','b','n'};

  int selectedRow = -1;
  int selectedCol = -1;
  int clickCount = 0;
  int tomoverow;
  int tomovecol;
  int lastStartRow = -1;
  int lastStartCol = -1;
  int lastEndRow = -1;
  int lastEndCol = -1;
  bool move_made = false;
  bool whiteTurn = true;
  bool kingUnsafe = false;
  bool whiteKingMoved = false;
  bool blackKingMoved = false;
  bool whiteLeftRookMoved = false;
  bool whiteRightRookMoved = false;
  bool blackLeftRookMoved = false;
  bool blackRightRookMoved = false;
  bool gameOver = false;
  bool promotionPending = false;
  bool promotionWhite = true;
  int promotionRow = -1;
  int promotionCol = -1;
  bool aiMovePending = false;
  int ScrollHistory = 0;
  int selectedDepth = 3; // default depth
  bool showDepthMenu = false;

  // for draw by repetation
  std::map<std::string, int> positionCount;
  /*using multithreading for ai , this will run ai on another thread and
   hence i will see board after white move and before black move */
  std::atomic<bool> aiMoveReady(false);
  std::atomic<bool> aiThinking(false);
  Move aiChosenMove = {-1, -1, -1, -1, '.', '.', false};
  std::mutex aiMoveMutex;
  std::string gameResult = "";

  std::vector<Move> moveHistory;
  std::vector<sf::Vector2i> legalMoves;

  // Depth selector
  sf::RectangleShape depthButtons[5];
  std::vector<sf::Text> depthTexts;
  std::vector<sf::Text> diffLabels;

  for (int i = 0; i < 5; i++) {
    depthTexts.emplace_back(font);
    diffLabels.emplace_back(font);
  }

  for (int i = 0; i < 5; i++) {
    depthButtons[i].setSize(sf::Vector2f(80.f, 80.f));
    depthButtons[i].setFillColor(sf::Color(70, 70, 70));
    depthButtons[i].setPosition(
        {windowWidth / 2.f - 220.f + i * 110.f, windowHeight / 2.f - 40.f});

    depthTexts[i].setFont(font);
    depthTexts[i].setCharacterSize(36);
    depthTexts[i].setString(std::to_string(i + 1));
    depthTexts[i].setFillColor(sf::Color::White);
    depthTexts[i].setPosition(
        {windowWidth / 2.f - 200.f + i * 110.f, windowHeight / 2.f - 20.f});
  }

  // Depth menu title
  sf::Text depthTitle(font);
  depthTitle.setCharacterSize(36);
  depthTitle.setFillColor(sf::Color::White);
  depthTitle.setString("Select Difficulty");
  depthTitle.setPosition(
      {windowWidth / 2.f - 200.f, windowHeight / 2.f - 120.f});

  // Difficulty labels
  std::string labelNames[5] = {"Baby", "Easy", "Medium", "Hard", "Expert"};
  for (int i = 0; i < 5; i++) {
    diffLabels[i].setFont(font);
    diffLabels[i].setCharacterSize(18);
    diffLabels[i].setFillColor(sf::Color(180, 180, 180));
    diffLabels[i].setString(labelNames[i]);
    diffLabels[i].setPosition(
        {windowWidth / 2.f - 220.f + i * 110.f, windowHeight / 2.f + 50.f});
  }
  while (window.isOpen()) {
    // EVENTS
    while (auto event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
      if (const auto *scroll = event->getIf<sf::Event::MouseWheelScrolled>()) {
        if (scroll->wheel == sf::Mouse::Wheel::Vertical) {
          int totalPairs = ((int)moveHistory.size() + 1) / 2;
          int maxVisible = static_cast<int>(totalGridHeight / 35.f);

          if (scroll->delta < 0) {
            ScrollHistory = std::min(ScrollHistory + 1,
                                     std::max(0, totalPairs - maxVisible));
          } else {
            ScrollHistory = std::max(ScrollHistory - 1, 0);
          }
        }
      }
      // Selction on Mouse Click
      if (const auto *mouseClick =
              event->getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseClick->button == sf::Mouse::Button::Left) {

          int clickedCol =
              static_cast<int>((mouseClick->position.x - offsetX) / tileSize);
          int clickedRow =
              static_cast<int>((mouseClick->position.y - offsetY) / tileSize);
          sf::Vector2f mousePos(mouseClick->position.x, mouseClick->position.y);
          // PROMOTION
          if (promotionPending) {
            float startX = 650.f;
            float startY = 700.f;

            char whitePieces[4] = {'Q', 'R', 'B', 'N'};
            char blackPieces[4] = {'q', 'r', 'b', 'n'};

            for (int i = 0; i < 4; i++) {
              sf::FloatRect box({startX + i * 130.f, startY}, {120.f, 120.f});

              if (box.contains(mousePos)) {
                char chosenPiece =
                    promotionWhite ? whitePieces[i] : blackPieces[i];

                board[promotionRow][promotionCol] = chosenPiece;

                promotionPending = false;
                selectedRow = -1;
                selectedCol = -1;
                clickCount = 0;

                whiteTurn = !whiteTurn;
                if (gameMode == GameMode::PvAI && !whiteTurn) {
                  aiMovePending = true;
                }
                legalMoves.clear();
                bool inCheck = isKingInCheck(whiteTurn);
                bool hasMoves = isAnyLegalMove(whiteTurn);

                break;
              }
            }

            continue;
          }
          if (showDepthMenu) {
            for (int i = 0; i < 5; i++) {
              if (depthButtons[i].getGlobalBounds().contains(mousePos)) {
                selectedDepth = i + 1;
                gameMode = GameMode::PvAI;
                showDepthMenu = false;
              }
            }
            continue;
          }
          // MainMenu
          if (showMainMenu) {
            sf::Vector2f mousePos(mouseClick->position.x,
                                  mouseClick->position.y);

            if (pvpButton.getGlobalBounds().contains(mousePos)) {
              gameMode = GameMode::PvP;
              showMainMenu = false;
            }
            if (pvaiButton.getGlobalBounds().contains(mousePos)) {
              showDepthMenu = true;
              showMainMenu = false;
            }
            continue;
          }
          if (restartButton.getGlobalBounds().contains(mousePos)) {
            char resetBoard[8][8] = {{'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
                                     {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
                                     {'.', '.', '.', '.', '.', '.', '.', '.'},
                                     {'.', '.', '.', '.', '.', '.', '.', '.'},
                                     {'.', '.', '.', '.', '.', '.', '.', '.'},
                                     {'.', '.', '.', '.', '.', '.', '.', '.'},
                                     {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
                                     {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}};

            for (int row = 0; row < 8; row++) {
              for (int col = 0; col < 8; col++) {
                board[row][col] = resetBoard[row][col];
              }
            }

            moveHistory.clear();

            whiteTurn = true;

            move_made = false;

            selectedRow = -1;
            selectedCol = -1;

            clickCount = 0;

            legalMoves.clear();

            whiteKingMoved = false;
            blackKingMoved = false;

            whiteLeftRookMoved = false;
            whiteRightRookMoved = false;

            blackLeftRookMoved = false;
            blackRightRookMoved = false;
            showMainMenu = true;
            gameMode = GameMode::None;
            aiMovePending = false;
            positionCount.clear();
            aiThinking = false;
            aiChosenMove = {-1, -1, -1, -1, '.', '.', false};
            ScrollHistory = 0;
            showDepthMenu = false;
            showMainMenu = true;
            selectedDepth = 3; // default
          }
          if (gameOver && exitButton.getGlobalBounds().contains(mousePos)) {
            window.close();
          }
          if (gameOver && rematchButton.getGlobalBounds().contains(mousePos)) {
            gameOver = false;
            gameResult = "";

            char resetBoard[8][8] = {{'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
                                     {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
                                     {'.', '.', '.', '.', '.', '.', '.', '.'},
                                     {'.', '.', '.', '.', '.', '.', '.', '.'},
                                     {'.', '.', '.', '.', '.', '.', '.', '.'},
                                     {'.', '.', '.', '.', '.', '.', '.', '.'},
                                     {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
                                     {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}};

            for (int row = 0; row < 8; row++) {
              for (int col = 0; col < 8; col++) {
                board[row][col] = resetBoard[row][col];
              }
            }

            moveHistory.clear();

            whiteTurn = true;

            move_made = false;

            selectedRow = -1;
            selectedCol = -1;

            clickCount = 0;

            legalMoves.clear();

            whiteKingMoved = false;
            blackKingMoved = false;

            whiteLeftRookMoved = false;
            whiteRightRookMoved = false;

            blackLeftRookMoved = false;
            blackRightRookMoved = false;

            positionCount.clear();
          }
          if (!gameOver && !aiMovePending &&
              !(gameMode == GameMode::PvAI && !whiteTurn) && clickedCol >= 0 &&
              clickedCol < 8 && clickedRow >= 0 && clickedRow < 8) {
            if (clickCount == 0) {

              if (board[clickedRow][clickedCol] != '.') {
                char selectedPiece = board[clickedRow][clickedCol];
                if ((whiteTurn && isBlack(selectedPiece)) ||
                    (!whiteTurn && isWhite(selectedPiece))) {
                  continue;
                }
                selectedCol = clickedCol;
                selectedRow = clickedRow;
                tomoverow = clickedRow;
                tomovecol = clickedCol;
                clickCount = 1;

                // Showing Valid Moves
                legalMoves.clear();

                char sourcePiece = board[selectedRow][selectedCol];

                if (sourcePiece == '.')
                  ;

                for (int endRow = 0; endRow < 8; ++endRow) {
                  for (int endCol = 0; endCol < 8; ++endCol) {
                    char targetPiece = board[endRow][endCol];

                    if (((isWhite(sourcePiece) && isWhite(targetPiece)) ||
                         (isBlack(sourcePiece) && isBlack(targetPiece))) ||
                        !isValidMove(selectedRow, selectedCol, endRow, endCol,
                                     sourcePiece, targetPiece)) {
                      continue;
                    }

                    Move move{selectedRow, selectedCol, endRow, endCol,
                              sourcePiece, targetPiece, false};

                    makeMove(move);
                    bool kingSafe = !isKingInCheck(whiteTurn);
                    undoMove(move);

                    if (kingSafe) {
                      legalMoves.push_back({endRow, endCol});
                    }
                  }
                }
              }
            } else {

              char sourcePiece = board[tomoverow][tomovecol];
              char targetPiece = board[clickedRow][clickedCol];

              if ((isWhite(sourcePiece) && isWhite(targetPiece)) ||
                  (isBlack(sourcePiece) && isBlack(targetPiece))) {
                selectedRow = -1;
                selectedCol = -1;
                clickCount = 0;
              } else {
                Move move;
                move.startRow = tomoverow;
                move.startCol = tomovecol;
                move.endRow = clickedRow;
                move.endCol = clickedCol;
                move.movedPiece = sourcePiece;
                move.capturedPiece = targetPiece;
                bool wasEnPassant = false;

                // Verify standard and castling rules
                bool validMove =
                    isValidMove(move.startRow, move.startCol, move.endRow,
                                move.endCol, sourcePiece, targetPiece);
                bool castlingMove = isValidCastling(
                    move.startRow, move.startCol, move.endRow, move.endCol,
                    sourcePiece, whiteKingMoved, blackKingMoved,
                    whiteLeftRookMoved, whiteRightRookMoved, blackLeftRookMoved,
                    blackRightRookMoved);

                validMove = validMove || castlingMove;

                if (validMove) {
                  int rookRow = (sourcePiece == 'K') ? 7 : 0;
                  char rookPiece = (sourcePiece == 'K') ? 'R' : 'r';
                  int rookStartCol = -1, rookEndCol = -1;

                  bool isCastlingAttempt =
                      castlingMove &&
                      (sourcePiece == 'K' || sourcePiece == 'k') &&
                      move.startCol == 4;

                  if (isCastlingAttempt) {
                    rookStartCol = (move.endCol == 6) ? 7 : 0;
                    rookEndCol = (move.endCol == 6) ? 5 : 3;
                  }

                  legalMoves.clear();

                  makeMove(move);
                  if (isCastlingAttempt && rookStartCol != -1) {
                    board[rookRow][rookEndCol] = rookPiece;
                    board[rookRow][rookStartCol] = '.';
                  }

                  if (isKingInCheck(whiteTurn)) {
                    undoMove(move);
                    if (isCastlingAttempt && rookStartCol != -1) {
                      board[rookRow][rookStartCol] = rookPiece;
                      board[rookRow][rookEndCol] = '.';
                    }

                    selectedRow = -1;
                    selectedCol = -1;
                    clickCount = 0;
                  } else {
                    if (sourcePiece == 'P' && clickedRow == 0) {
                      promotionPending = true;
                      promotionWhite = true;
                      promotionRow = move.endRow;
                      promotionCol = move.endCol;
                    }
                    if (sourcePiece == 'p' && clickedRow == 7) {
                      promotionPending = true;
                      promotionWhite = false;
                      promotionRow = move.endRow;
                      promotionCol = move.endCol;
                    }
                    if (move.capturedPiece == 'R') {
                      if (move.endRow == 7 && move.endCol == 0)
                        whiteLeftRookMoved = true;
                      if (move.endRow == 7 && move.endCol == 7)
                        whiteRightRookMoved = true;
                    }
                    if (move.capturedPiece == 'r') {
                      if (move.endRow == 0 && move.endCol == 0)
                        blackLeftRookMoved = true;
                      if (move.endRow == 0 && move.endCol == 7)
                        blackRightRookMoved = true;
                    }

                    if (castlingMove)
                      castleSound.play();
                    else if (move.capturedPiece != '.')
                      captureSound.play();
                    else
                      moveSound.play();

                    lastStartRow = move.startRow;
                    lastStartCol = move.startCol;
                    lastEndRow = move.endRow;
                    lastEndCol = move.endCol;
                    move_made = true;

                    if (!promotionPending) {
                      whiteTurn = !whiteTurn;
                    }
                    if (gameMode == GameMode::PvAI && !whiteTurn) {
                      aiMovePending = true;
                    }
                    bool inCheck = isKingInCheck(whiteTurn);
                    bool hasMoves = isAnyLegalMove(whiteTurn);

                    if (inCheck)
                      checkSound.play();
                    if (!hasMoves) {
                      gameOver = true;
                      gameResult =
                          inCheck ? (whiteTurn ? "Black Wins" : "White Wins")
                                  : "Draw by StaleMate";
                    }

                    if (!hasMoves) {
                      gameOver = true;
                      if (inCheck) {
                        checkmateSound.play();
                        gameResult = whiteTurn ? "Black Wins" : "White Wins";
                      } else {
                        gameResult = "Draw by StaleMate";
                      }
                    } else if (isInsufficientMaterial()) {
                      gameOver = true;
                      gameResult = "Draw by\nInsufficient Material";
                    }
                    bool givesCheck = isKingInCheck(false);
                    bool givesMate = givesCheck && !isAnyLegalMove(false);
                    move.suffix = givesMate ? "#" : (givesCheck ? "+" : "");
                    moveHistory.push_back(move);
                    std::cout << "Evaluation: " << evaluateBoard() << '\n';
                    {
                      int tp = ((int)moveHistory.size() + 1) / 2;
                      int mv = static_cast<int>(totalGridHeight / 35.f);
                      ScrollHistory = std::max(0, tp - mv);
                    }
                    selectedRow = -1;
                    selectedCol = -1;
                    clickCount = 0;
                    std::string pos = "";
                    for (int r = 0; r < 8; r++)
                      for (int c = 0; c < 8; c++)
                        pos += board[r][c];
                    positionCount[pos]++;
                    if (positionCount[pos] >= 3) {
                      gameOver = true;
                      gameResult = "Draw by Repetition";
                    }
                  }
                } else {

                  selectedRow = -1;
                  selectedCol = -1;
                  clickCount = 0;
                }
              }
            }
          }
        }
      }
    }

    // AI on diff thread
    if (gameMode == GameMode::PvAI && !whiteTurn && !gameOver &&
        !promotionPending && !aiThinking && aiMovePending && !aiMoveReady) {

      aiThinking = true;

      std::thread([mainBoard = &board, mainEPA = &enPassantAvailable,
                   mainEPR = &enPassantRow, mainEPC = &enPassantCol,
                   wKM = whiteKingMoved, bKM = blackKingMoved,
                   wLRM = whiteLeftRookMoved, wRRM = whiteRightRookMoved,
                   bLRM = blackLeftRookMoved, bRRM = blackRightRookMoved,
                   selectedDepth, &aiMoveMutex, &aiChosenMove, &aiThinking,
                   &aiMoveReady]() {
        // Copy the main thread board state to this thread
        for (int r = 0; r < 8; ++r) {
          for (int c = 0; c < 8; ++c) {
            board[r][c] = (*mainBoard)[r][c];
          }
        }
        enPassantAvailable = *mainEPA;
        enPassantRow = *mainEPR;
        enPassantCol = *mainEPC;

        aiWhiteKingMoved = wKM;
        aiBlackKingMoved = bKM;
        aiWhiteLeftRookMoved = wLRM;
        aiWhiteRightRookMoved = wRRM;
        aiBlackLeftRookMoved = bLRM;
        aiBlackRightRookMoved = bRRM;

        int pieceCount = 0;
        for (int r = 0; r < 8; r++)
          for (int c = 0; c < 8; c++)
            if (board[r][c] != '.')
              pieceCount++;

        int searchDepth = (pieceCount <= 8) ? selectedDepth + 2 : selectedDepth;
        Move result = getMinimaxMove(false, searchDepth);

        std::lock_guard<std::mutex> lock(aiMoveMutex);
        aiChosenMove = result;
        aiThinking = false;
        aiMoveReady = true; // signal that move is ready to apply
      }).detach();
    }

    // Apply AI move after thinking
    if (gameMode == GameMode::PvAI && !aiThinking && aiMovePending &&
        aiMoveReady && !gameOver && !promotionPending) {

      std::lock_guard<std::mutex> lock(aiMoveMutex);
      if (aiChosenMove.startRow != -1) {
        makeMove(aiChosenMove);
        // castling
        if (aiChosenMove.capturedPiece == 'R') {
          if (aiChosenMove.endRow == 7 && aiChosenMove.endCol == 0)
            whiteLeftRookMoved = true;
          if (aiChosenMove.endRow == 7 && aiChosenMove.endCol == 7)
            whiteRightRookMoved = true;
        }
        if (aiChosenMove.capturedPiece == 'r') {
          if (aiChosenMove.endRow == 0 && aiChosenMove.endCol == 0)
            blackLeftRookMoved = true;
          if (aiChosenMove.endRow == 0 && aiChosenMove.endCol == 7)
            blackRightRookMoved = true;
        }
        char piece = aiChosenMove.movedPiece;
        if ((piece == 'K' || piece == 'k') &&
            std::abs(aiChosenMove.endCol - aiChosenMove.startCol) == 2) {
          int rookRow = (piece == 'K') ? 7 : 0;
          char rookPiece = (piece == 'K') ? 'R' : 'r';
          if (aiChosenMove.endCol == 6) {
            board[rookRow][5] = rookPiece;
            board[rookRow][7] = '.';
            if (piece == 'K')
              whiteRightRookMoved = true;
            else
              blackRightRookMoved = true;
          } else {
            board[rookRow][3] = rookPiece;
            board[rookRow][0] = '.';
            if (piece == 'K')
              whiteLeftRookMoved = true;
            else
              blackLeftRookMoved = true;
          }
          castleSound.play();
          if (piece == 'K')
            whiteKingMoved = true;
          else
            blackKingMoved = true;
        }
        if (aiChosenMove.capturedPiece != '.')
          captureSound.play();
        else
          moveSound.play();

        lastStartRow = aiChosenMove.startRow;
        lastStartCol = aiChosenMove.startCol;
        lastEndRow = aiChosenMove.endRow;
        lastEndCol = aiChosenMove.endCol;
        move_made = true;
        bool givesCheck = isKingInCheck(true);
        bool givesMate = givesCheck && !isAnyLegalMove(true);
        aiChosenMove.suffix = givesMate ? "#" : (givesCheck ? "+" : "");
        moveHistory.push_back(aiChosenMove);
        std::string pos = "";
        for (int r = 0; r < 8; r++)
          for (int c = 0; c < 8; c++)
            pos += board[r][c];
        positionCount[pos]++;
        if (positionCount[pos] >= 3) {
          gameOver = true;
          gameResult = "Draw by Repetition";
        }
        {
          int tp = ((int)moveHistory.size() + 1) / 2;
          int mv = static_cast<int>(totalGridHeight / 35.f);
          ScrollHistory = std::max(0, tp - mv);
        }

        whiteTurn = true;
        aiMovePending = false;
        aiMoveReady = false;
        aiChosenMove = {-1, -1, -1, -1, '.', '.', false}; // reset

        bool inCheck = isKingInCheck(whiteTurn);
        bool hasMoves = isAnyLegalMove(whiteTurn);
        if (inCheck)
          checkSound.play();
        if (!hasMoves) {
          gameOver = true;
          gameResult = inCheck ? "Black Wins" : "Draw by StaleMate";
          if (inCheck)
            checkmateSound.play();
        }
      }
    }
    window.clear();
    // Msg while ai is thinking
    if (aiThinking) {
      sf::Text thinkText(font);
      thinkText.setCharacterSize(28);
      thinkText.setFillColor(sf::Color::Yellow);
      thinkText.setString("AI Thinking...");
      thinkText.setPosition({offsetX - 350.f, offsetY + totalGridHeight / 2.f});
      window.draw(thinkText);
    }
    sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
    // darken on hover
    pvpButton.setFillColor(pvpButton.getGlobalBounds().contains(mousePos)
                               ? sf::Color(160, 125, 65)
                               : sf::Color(185, 148, 82));

    pvaiButton.setFillColor(pvaiButton.getGlobalBounds().contains(mousePos)
                                ? sf::Color(160, 125, 65)
                                : sf::Color(185, 148, 82));

    // displayBoard();
    // Main Menu
    if (showMainMenu) {
      window.draw(menuBackground);
      window.draw(menuTitle);
      window.draw(pvpButton);
      window.draw(pvpText);
      window.draw(pvaiButton);
      window.draw(pvaiText);
      window.display();
      continue;
    }
    // depth selector
    if (showDepthMenu) {
      window.draw(depthTitle);

      for (int i = 0; i < 5; i++) {
        if (i + 1 == selectedDepth)
          depthButtons[i].setFillColor(sf::Color(100, 150, 100));
        else
          depthButtons[i].setFillColor(sf::Color(70, 70, 70));

        window.draw(depthButtons[i]);
        window.draw(depthTexts[i]);
        window.draw(diffLabels[i]);
      }

      window.display();
      continue;
    }

    // Drawing Board
    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
        square.setPosition(
            sf::Vector2f(offsetX + col * tileSize, offsetY + row * tileSize));
        // Alternate Colouring
        if ((row + col) % 2 == 0) {
          square.setFillColor(sf::Color(240, 217, 181));
        } else {
          square.setFillColor(sf::Color(181, 136, 99));
        }
        window.draw(square);
      }
    }

    // FILES (a-h)
    for (int col = 0; col < 8; col++) {
      sf::Text fileText(font);

      fileText.setCharacterSize(24);
      fileText.setStyle(sf::Text::Bold);

      if ((7 + col) % 2 == 0) {
        fileText.setFillColor(sf::Color(181, 136, 99));
      } else {
        fileText.setFillColor(sf::Color(240, 217, 181));
      }

      std::string letter = std::string(1, 'a' + col);

      fileText.setString(letter);
      fileText.setPosition({offsetX + col * tileSize + tileSize - 15.f,
                            offsetY + totalGridHeight - 28.f});

      window.draw(fileText);
    }

    // RANKS (1-8)
    for (int row = 0; row < 8; row++) {
      sf::Text rankText(font);

      rankText.setCharacterSize(24);
      rankText.setStyle(sf::Text::Bold);

      if ((row + 0) % 2 == 0) {
        rankText.setFillColor(sf::Color(181, 136, 99));
      } else {
        rankText.setFillColor(sf::Color(240, 217, 181));
      }

      std::string number = std::to_string(8 - row);

      rankText.setString(number);
      rankText.setPosition({offsetX + 5.f, offsetY + row * tileSize + 5.f});

      window.draw(rankText);
    }
    // Highlighting the box
    if (selectedRow != -1 && selectedCol != -1) {
      boardbox.setPosition(
          {offsetX + selectedCol * tileSize, offsetY + selectedRow * tileSize});

      window.draw(boardbox);
    }
    // Highlight the Changed Squares
    if (move_made) {
      // Previous square
      boardbox1.setPosition({offsetX + lastStartCol * tileSize,
                             offsetY + lastStartRow * tileSize});

      window.draw(boardbox1);

      // New square
      boardbox1.setPosition(
          {offsetX + lastEndCol * tileSize, offsetY + lastEndRow * tileSize});

      window.draw(boardbox1);
    }
    if (isKingInCheck(true)) {
      sf::Vector2i kingPos = findKing(true);

      boardbox2.setPosition(
          {offsetX + kingPos.y * tileSize, offsetY + kingPos.x * tileSize});

      window.draw(boardbox2);
    }
    if (isKingInCheck(false)) {
      sf::Vector2i kingPos2 = findKing(false);

      boardbox2.setPosition(
          {offsetX + kingPos2.y * tileSize, offsetY + kingPos2.x * tileSize});

      window.draw(boardbox2);
    }
    // Valid Moves

    sf::CircleShape moveDot(15.f);

    moveDot.setFillColor(sf::Color(80, 80, 80, 180));

    for (const auto &move : legalMoves) {
      moveDot.setPosition({offsetX + move.y * tileSize + 47.f,
                           offsetY + move.x * tileSize + 47.f});

      window.draw(moveDot);
    }

    // Drawing Pieces
    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
        char piece = board[row][col];

        if (piece == '.')
          continue;
        sf::Sprite sprite(textures[piece]);

        sprite.setPosition(
            {offsetX + col * tileSize, offsetY + row * tileSize});

        sprite.setScale({tileSize / textures[piece].getSize().x,
                         tileSize / textures[piece].getSize().y});

        window.draw(sprite);
      }
    }
    // Promotion
    if (promotionPending) {
      float startX = 650.f;
      float startY = 700.f;
      char whitePieces[4] = {'Q', 'R', 'B', 'N'};
      char BlackPieces[4] = {'q', 'r', 'b', 'n'};
      char piece;
      for (int i = 0; i < 4; i++) {
        if (promotionWhite) {
          piece = whitePieces[i];
        } else {
          piece = BlackPieces[i];
        }
        sf::RectangleShape optionBox({120.f, 120.f});
        optionBox.setFillColor(sf::Color(50, 50, 50, 240));
        optionBox.setPosition({startX + i * 130.f, startY});

        window.draw(optionBox);

        sf::Sprite sprite(texture_promo[piece]);

        sprite.setPosition({startX + i * 130.f + 10.f, startY + 10.f});

        sprite.setScale({100.f / texture_promo[piece].getSize().x,
                         100.f / texture_promo[piece].getSize().y});

        window.draw(sprite);
      }
    }
    // Notation chnaged to totally depend on gameResult from above
    float historyX = offsetX + totalGridWidth + 40.f;
    float historyY = offsetY;
    float lineHeight = 35.f;
    int maxVisible = static_cast<int>(totalGridHeight / lineHeight);
    int totalPairs = ((int)moveHistory.size() + 1) / 2;

    for (int pair = ScrollHistory; pair < totalPairs; pair++) {
      int i = pair * 2;

      std::string whiteMove =
          moveToNotation(moveHistory[i]) + moveHistory[i].suffix;
      std::string blackMove = "";
      if (i + 1 < (int)moveHistory.size())
        blackMove =
            moveToNotation(moveHistory[i + 1]) + moveHistory[i + 1].suffix;

      std::string line = std::to_string(pair + 1) + ". " + whiteMove;
      while (line.size() < 12)
        line += ' ';
      line += blackMove;

      sf::Text text(font);
      text.setCharacterSize(28);
      text.setString(line);
      text.setFillColor(sf::Color::White);
      text.setPosition(
          {historyX, historyY + (pair - ScrollHistory) * lineHeight});
      window.draw(text);
    }
    if (gameOver) {
      sf::Text finalText(font);
      finalText.setCharacterSize(28);
      finalText.setFillColor(sf::Color::Red);
      finalText.setString(gameResult);
      finalText.setPosition({offsetX - 350.f, offsetY + totalGridHeight / 2.f});
      window.draw(finalText);
    }
    sf::Vector2f mousePos2 = sf::Vector2f(sf::Mouse::getPosition(window));

    restartButton.setFillColor(
        restartButton.getGlobalBounds().contains(mousePos2)
            ? sf::Color(100, 100, 100)
            : sf::Color(70, 70, 70));

    window.draw(restartButton);
    window.draw(restartText);
    window.display();
  }
}