# Tic-Tac-Toe with Minimax AI

## Fundamentals of Programming 2 Project

### Description
A complete implementation of the classic Tic-Tac-Toe game featuring:
- Two-player mode (human vs human)
- Player vs Computer mode with unbeatable AI using the Minimax algorithm
- Desktop interface built in C with GTK
- Match mode selection, symbol selection, and live scoreboard
- Proper memory management (no leaks)

### Technologies Used
- **Language:** C (C11 standard)
- **GUI:** GTK 3
- **Compiler:** GCC
- **Build Tool:** Make
- **Version Control:** Git/GitHub

### Project Structure
- `src/game.c` and `src/minimax.c`: shared game engine and AI
- `src/main.c`: GTK application entry point
- `src/gtk_ui.c`: desktop interface, callbacks, and styling

### Build

#### GTK interface
```bash
make
make run
```

The project expects GTK 3 development files to be available through `pkg-config`.
If `make` reports that GTK was not found, install `gtk+-3.0` and ensure `pkg-config` can resolve it.

### Notes
- The project now runs exclusively through the GTK graphical interface.
- The interface reuses the original Minimax AI instead of duplicating game logic.
- You can switch between `Human vs Human` and `Human vs Computer`, choose whether the human starts as `X` or `O`, and track the running score.
