// Board.cpp implementation
#include "Board.hpp"
#include "Move.hpp"
#include "MoveStruct.hpp"
#include "ai.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
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
  // Printing Board on Console
  displayBoard();

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

  // MainMenu Buttons
  sf::RectangleShape pvpButton(sf::Vector2f(350.f, 100.f));
  pvpButton.setPosition({725.f, 700.f});

  sf::RectangleShape pvaiButton(sf::Vector2f(350.f, 100.f));
  pvaiButton.setPosition({725.f, 850.f});

  sf::Text pvpText(font);
  pvpText.setString("Player vs Player");
  pvpText.setCharacterSize(40);
  pvpText.setPosition({740.f, 725.f});
  pvpText.setFillColor(sf::Color::Black);

  sf::Text pvaiText(font);
  pvaiText.setString("Player vs AI");
  pvaiText.setPosition({770.f, 875.f});
  pvaiText.setCharacterSize(40);
  pvaiText.setFillColor(sf::Color::Black);

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

  std::string gameResult = "";

  std::vector<Move> moveHistory;
  std::vector<sf::Vector2i> legalMoves;

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
                legalMoves.clear();
                bool inCheck = isKingInCheck(whiteTurn);
                bool hasMoves = isAnyLegalMove(whiteTurn);

                break;
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
              gameMode = GameMode::PvAI;
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
            ScrollHistory = 0;
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
                    }

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

    // AI moves
    if (gameMode == GameMode::PvAI && !whiteTurn && !gameOver &&
        !promotionPending && !aiMovePending) {
      aiMovePending = true;
    }

    if (gameMode == GameMode::PvAI && aiMovePending && !gameOver &&
        !promotionPending) {
      Move aiMove = getRandomMove(false);
      if (aiMove.startRow == -1) {
        whiteTurn = !whiteTurn;
        aiMovePending = false;
      } else {
        makeMove(aiMove);

        if (aiMove.capturedPiece != '.')
          captureSound.play();
        else
          moveSound.play();

        lastStartRow = aiMove.startRow;
        lastStartCol = aiMove.startCol;
        lastEndRow = aiMove.endRow;
        lastEndCol = aiMove.endCol;
        move_made = true;
        moveHistory.push_back(aiMove);
        {
          int tp = ((int)moveHistory.size() + 1) / 2;
          int mv = static_cast<int>(totalGridHeight / 35.f);
          ScrollHistory = std::max(0, tp - mv);
        }

        whiteTurn = !whiteTurn;
        aiMovePending = false; // reset for next turn

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
    // displayBoard();
    // Main Menu
    if (showMainMenu) {
      window.draw(pvpButton);
      window.draw(pvaiButton);

      window.draw(pvpText);
      window.draw(pvaiText);

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
    // Notation
    float historyX = offsetX + totalGridWidth + 40.f;
    float historyY = offsetY;
    float lineHeight = 35.f;
    int maxVisible = static_cast<int>(totalGridHeight / lineHeight);
    int totalPairs = ((int)moveHistory.size() + 1) / 2;

    for (int pair = ScrollHistory; pair < totalPairs; pair++) {
      int i = pair * 2;

      std::string whiteMove = moveToNotation(moveHistory[i]);
      std::string blackMove = "";
      if (i + 1 < (int)moveHistory.size())
        blackMove = moveToNotation(moveHistory[i + 1]);

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

    if (!isAnyLegalMove(whiteTurn)) {
      sf::Text finalText(font);
      finalText.setCharacterSize(28);
      finalText.setFillColor(sf::Color::Red);

      if (isKingInCheck(whiteTurn))
        finalText.setString(whiteTurn ? "CHECKMATE - Black Wins"
                                      : "CHECKMATE - White Wins");
      else
        finalText.setString("Draw by STALEMATE");

      int maxVisible = static_cast<int>(totalGridHeight / 35.f);
      int totalPairs = ((int)moveHistory.size() + 1) / 2;
      int visibleRows = std::min(totalPairs - ScrollHistory, maxVisible);

      float textY = historyY + visibleRows * 35.f + 10.f;

      float maxY = offsetY + totalGridHeight - 40.f;
      if (textY > maxY)
        textY = maxY;

      finalText.setPosition({offsetX - 350.f, offsetY + totalGridHeight / 2.f});
      window.draw(finalText);
    }
    window.draw(restartButton);
    window.draw(restartText);
    window.display();
  }
}
