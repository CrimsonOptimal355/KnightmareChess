<h1 align="center">♟️ Chess — C++ & SFML</h1>

<p align="center">
  A fully functional two-player chess game built with C++17 and SFML 3, featuring move validation, castling, check/checkmate detection, stalemate, sound effects, move history notation, and a clean graphical interface.
</p>

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-17-blue?style=flat-square&logo=cplusplus" />
  <img src="https://img.shields.io/badge/SFML-3.0-green?style=flat-square" />
  <img src="https://img.shields.io/badge/CMake-3.15+-red?style=flat-square&logo=cmake" />
  <img src="https://img.shields.io/badge/Platform-Windows-lightgrey?style=flat-square&logo=windows" />
</p>

---

## Features

- **Complete Chess Rules** — Legal move validation for all pieces: Pawn, Rook, Knight, Bishop, Queen, King
- **Castling** — Both kingside and queenside castling with full legality checks
- **Pawn Promotion** — Pawns automatically promote to Queen upon reaching the back rank
- **Check & Checkmate Detection** — Visual highlight on the king in check; game ends on checkmate
- **Stalemate Detection** — Recognizes draws when no legal moves are available
- **Legal Move Highlighting** — Click a piece to see all valid destination squares shown as dots
- **Last Move Highlighting** — The previous move's start and end squares are highlighted in yellow
- **Move History Notation** — Algebraic-style move log displayed alongside the board
- **Sound Effects** — Distinct sounds for move, capture, castle, check, and checkmate
- **Restart / Rematch / Exit** — In-game buttons to restart at any time or after game over

---

## Project Structure

```
Chess/
├── src/                  # C++ source files
│   ├── main.cpp          # Entry point, game loop, rendering
│   ├── Board.cpp         # Board state and display
│   ├── Game.cpp          # Core game logic
│   ├── Move.cpp          # Move validation, castling, notation
│   └── Piece.cpp         # Piece helper functions
├── include/              # Header files
│   ├── Board.hpp
│   ├── Game.hpp
│   ├── Move.hpp
│   ├── MoveStruct.hpp
│   ├── Piece.hpp
│   └── Constants.hpp
├── assets/
│   ├── textures/         # PNG piece sprites (wp, wk, wq, ... bp, bk, bq ...)
│   ├── sounds/           # MP3 sound effects (move, capture, castle, check, checkmate)
│   └── fonts/            # UbuntuMono-Regular.ttf
├── bin/                  # Compiled executable + SFML DLLs (Windows)
├── tools/                # Linting and formatting configs
├── .github/workflows/    # CI pipelines (Windows, Ubuntu, macOS, Docs)
└── CMakeLists.txt
```

---

## Getting Started

### Prerequisites

- **C++17** compatible compiler (MSVC, GCC, Clang)
- **CMake** 3.15+
- **SFML 3.0** — [Download here](https://www.sfml-dev.org/download/)

### Build (Windows with MinGW)

```bash
# Clone the repo
git clone https://github.com/CrimsonOptimal355/Chess-CPP-SFML.git
cd Chess-CPP-SFML

# Update CMakeLists.txt SFML_DIR path to your local SFML installation
# Then configure and build
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
```

### Running the Game

After building, copy the compiled executable into the `bin/` directory alongside the SFML DLLs and assets, then run it from there:

```bash
# Copy the built executable to bin/
cp build/Chess.exe bin/

# Run from bin/ so assets are found at the correct relative paths
cd bin
./Chess.exe
```

> The `bin/` folder already contains the required SFML `.dll` files and `assets/` folder. The executable must be run from this directory.

---

## How to Play

| Action | Input |
|---|---|
| Select a piece | Left click on it |
| Move a piece | Left click on a highlighted destination dot |
| Deselect | Click an invalid square |
| Restart | Click the **Restart** button (left of the board) |
| Rematch / Exit | Use buttons on the game-over screen |

- **White** pieces (uppercase) always move first.
- Valid moves are shown as **grey dots** after selecting a piece.
- The **king's square turns red** when in check.
- The game ends automatically on **checkmate** or **stalemate**.

---

## Dependencies

| Library | Version | Purpose |
|---|---|---|
| [SFML](https://www.sfml-dev.org/) | 3.0 | Windowing, graphics, audio |
| CMake | 3.15+ | Build system |

---

## Acknowledgements

- Piece textures inspired by classic chess sets
- Sound effects sourced for chess move/capture/check events
- Font: [Ubuntu Mono](https://fonts.google.com/specimen/Ubuntu+Mono) (SIL Open Font License)
