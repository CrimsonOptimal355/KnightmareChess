<div align="center">
  <img src="https://images.unsplash.com/photo-1618005182384-a83a8bd57fbe?w=100&fit=crop&q=80" alt="logo" height="80" style="border-radius: 12px; margin-bottom: 10px;" />
  <h1>KnightmareChess</h1>
  <p><em>A fully functional chess game built with C++17 and SFML 3, featuring two-player and AI opponent modes, complete move validation, castling, check/checkmate/stalemate detection, sound effects, move history, and a clean graphical interface.</em></p>
</div>

---


[![github stars](https://img.shields.io/github/stars/CrimsonOptimal355/KnightmareChess.svg?style=flat)](https://github.com/CrimsonOptimal355/KnightmareChess/stargazers) [![license](https://img.shields.io/github/license/CrimsonOptimal355/KnightmareChess.svg?style=flat)](https://github.com/CrimsonOptimal355/KnightmareChess/blob/main/LICENSE)


<hr style="height: 2px; border: none; background-color: #262626; margin: 20px 0;" />


<p align="center">
  <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/cplusplus/cplusplus-original.svg" alt="C++" width="40" height="40" />
  &nbsp;&nbsp;
  <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/sfml/sfml-original.svg" alt="SFML" width="40" height="40" />
  &nbsp;&nbsp;
  <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/cmake/cmake-original.svg" alt="CMake" width="40" height="40" />
  &nbsp;&nbsp;
  <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/powershell/powershell-original.svg" alt="PowerShell" width="40" height="40" />
</p>


<hr style="height: 2px; border: none; background-color: #262626; margin: 20px 0;" />


## 🚀 About KnightmareChess

KnightmareChess is a fully functional chess game developed with C++17 and SFML 3, offering both two-player local matches and an AI opponent mode. It boasts comprehensive move validation, including special moves like castling, pawn promotion, and precise detection for check, checkmate, and stalemate scenarios.

### ✨ Key Features

-   **Complete Chess Rules** — Accurate legal move validation for all chess pieces (Pawn, Rook, Knight, Bishop, Queen, King).
-   **Castling** — Full legality checks for both kingside and queenside castling.
-   **Pawn Promotion** — Pawns automatically promote to Queen upon reaching the back rank.
-   **Check & Checkmate Detection** — Visual highlighting for kings in check, with clear game-ending checkmate detection.
-   **Stalemate Detection** — Recognizes draw conditions when no legal moves are available.
-   **Legal Move Highlighting** — Click a piece to instantly visualize all its valid destination squares as dots.
-   **Last Move Highlighting** — Visually tracks the previous move's start and end squares in yellow.
-   **AI Opponent** — Challenge yourself against an intelligent Minimax + Alpha-Beta pruning AI.
-   **Sound Effects** — Engaging audio feedback for in-game actions.
-   **Move History** — Keep track of all moves made during a game.
-   **Clean Graphical Interface** — A user-friendly and aesthetically pleasing game board.

## 🛠️ Installation

To build and run KnightmareChess, you'll need a C++17 compatible compiler (like GCC or MSVC) and CMake (3.15+ recommended). The project also relies on the SFML 3 library. This project is primarily developed for Windows.

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/CrimsonOptimal355/KnightmareChess.git
    cd KnightmareChess
    ```

2.  **Set up SFML:**
    Ensure SFML 3.x is properly installed and configured on your system. Refer to the [official SFML documentation](https://www.sfml-dev.org/download.php) for installation instructions specific to your platform and IDE.

3.  **Build the project with CMake:**
    ```bash
    mkdir build
    cd build
    cmake ..
    cmake --build .
    ```

    If you are on Windows using Visual Studio, you might run `cmake .. -G "Visual Studio 17 2022"` (or your preferred version) and then open the generated `.sln` file to build.

## 🎮 Usage

After successfully building the project, you can find the executable in the `build/Debug` or `build/Release` directory (depending on your build configuration).

Simply launch the executable to start the game. You can then choose to play against another human player locally or challenge the built-in AI opponent.


<hr style="height: 2px; border: none; background-color: #262626; margin: 20px 0;" />


<p align="center">
  <a href="https://github.com/CrimsonOptimal355/KnightmareChess/graphs/contributors">
    <img src="https://contrib.rocks/image?repo=CrimsonOptimal355/KnightmareChess&max=12" alt="Contributors" />
  </a>
</p>

![GitHub Stats](https://github-readme-stats.vercel.app/api?username=CrimsonOptimal355&theme=dark&show_icons=true&hide_rank=false)