# Tic-Tac-Toe GTK Project Report

## 1. Introduction

This project is a desktop implementation of the classic Tic-Tac-Toe game. It was developed in C and uses GTK 3 to provide a graphical user interface. The application supports both a two-player local mode and a human-versus-computer mode. The computer opponent uses the Minimax algorithm, which allows it to always choose the optimal move.

The project was structured to keep the game rules, the AI logic, and the graphical interface separated. This makes the code easier to understand, maintain, and explain during a presentation.

## 2. Objectives

The main objectives of the project are:

- Implement the rules of Tic-Tac-Toe correctly.
- Provide a graphical interface instead of a console-only experience.
- Support both human-versus-human and human-versus-computer gameplay.
- Use the Minimax algorithm to create an unbeatable AI opponent.
- Organize the project in a modular way, separating interface and game logic.

## 3. Technologies Used

### 3.1 C (C11)

C is the main programming language used in the project. It is responsible for:

- implementing the game rules
- handling board state and memory allocation
- integrating the graphical interface with the game logic

### 3.2 GTK 3

GTK 3 is the graphical toolkit used to build the desktop interface. It is responsible for:

- creating the application window
- displaying buttons, labels, and selection boxes
- handling user interaction events
- applying custom visual styling through CSS

### 3.3 GCC

GCC is the compiler used to build the project. It translates the C source code into an executable program.

### 3.4 Makefile

The Makefile automates the build and run process. It is useful because:

- it keeps the compilation process consistent
- it avoids manually typing long compiler commands
- it simplifies project maintenance

### 3.5 MSYS2

On Windows, MSYS2 provides:

- the GCC compiler
- GTK development libraries
- pkg-config, which resolves the GTK include and linker flags

## 4. Project Structure

The project is organized into separate modules:

- [src/main.c](C:/Users/migue/Videos/FoP2_TicTacToe/src/main.c:1): application entry point
- [src/gtk_ui.c](C:/Users/migue/Videos/FoP2_TicTacToe/src/gtk_ui.c:1): graphical interface and interaction flow
- [src/gtk_ui.h](C:/Users/migue/Videos/FoP2_TicTacToe/src/gtk_ui.h:1): interface declarations
- [src/game.c](C:/Users/migue/Videos/FoP2_TicTacToe/src/game.c:1): game rules and board logic
- [src/game.h](C:/Users/migue/Videos/FoP2_TicTacToe/src/game.h:1): game declarations
- [src/minimax.c](C:/Users/migue/Videos/FoP2_TicTacToe/src/minimax.c:1): AI algorithm
- [src/minimax.h](C:/Users/migue/Videos/FoP2_TicTacToe/src/minimax.h:1): AI declarations
- [Makefile](C:/Users/migue/Videos/FoP2_TicTacToe/Makefile:1): build automation
- [run_tictactoe.bat](C:/Users/migue/Videos/FoP2_TicTacToe/run_tictactoe.bat:1): Windows launcher with the correct runtime path

## 5. Program Logic

### 5.1 Main Entry Point

The program starts in [src/main.c](C:/Users/migue/Videos/FoP2_TicTacToe/src/main.c:1). Its role is simple:

- create a `GtkApplication`
- connect the `activate` signal to the UI startup function
- run the application loop

This means `main.c` does not contain the game itself. It only initializes the application.

### 5.2 Graphical Interface Logic

The interface is built in [src/gtk_ui.c](C:/Users/migue/Videos/FoP2_TicTacToe/src/gtk_ui.c:1). This file is responsible for:

- creating the window
- creating the board buttons
- creating match settings such as mode and symbol selection
- displaying the game status
- reacting to button clicks
- updating the board after every move

The UI file does not contain the low-level game rules. Instead, it calls functions from `game.c` and `minimax.c`.

### 5.3 Game State

The board is represented by the `Board` structure declared in [src/game.h](C:/Users/migue/Videos/FoP2_TicTacToe/src/game.h:14). It stores:

- a dynamic 2D matrix of characters
- the board size

Each board cell contains:

- `'X'`
- `'O'`
- `' '` for an empty position

## 6. Game Logic

The core game logic is implemented in [src/game.c](C:/Users/migue/Videos/FoP2_TicTacToe/src/game.c:1).

### 6.1 Board Creation

The function `create_board()` allocates memory for:

- the board structure
- the row pointers
- each row of the matrix

It then initializes every cell as empty.

### 6.2 Making a Move

The function `make_move()` is responsible for validating and applying a move. It checks:

- whether the board exists
- whether the coordinates are inside the valid range
- whether the selected cell is empty
- whether the symbol is valid

If all checks pass, the symbol is placed on the board.

### 6.3 Detecting a Win

The function `check_win()` checks:

- all rows
- all columns
- the main diagonal
- the secondary diagonal

If any of these lines contain the same symbol in all three positions, that player wins.

### 6.4 Detecting a Draw

The function `is_board_full()` scans all cells of the board. If no empty cell remains and no player has won, the match ends in a draw.

## 7. AI Logic with Minimax

The artificial intelligence is implemented in [src/minimax.c](C:/Users/migue/Videos/FoP2_TicTacToe/src/minimax.c:1).

### 7.1 Purpose of Minimax

Minimax is a decision algorithm used in two-player turn-based games. Its goal is to simulate future moves and choose the best possible action.

In this project:

- the computer tries to maximize the score
- the human player is treated as the minimizing opponent

### 7.2 Board Evaluation

The function `evaluate_board()` gives meaning to a board state:

- `+10` if the computer wins
- `-10` if the human wins
- `0` otherwise

### 7.3 Recursive Search

The function `minimax()` recursively explores all possible future states:

- if it is the computer's turn, it tries to maximize the score
- if it is the human's turn, it tries to minimize the score

This continues until the algorithm reaches a terminal state:

- computer win
- human win
- draw

### 7.4 Best Move Selection

The function `find_best_move()` tries every available move, evaluates it with `minimax()`, and returns the move with the best score.

Because Tic-Tac-Toe has a very small search space, Minimax can fully explore all possibilities. This makes the AI effectively unbeatable.

## 8. Interface Flow

The interface follows a clear event-driven structure.

### 8.1 Starting a Match

The function `app_start_match()`:

- reads the selected mode
- reads the selected player symbol
- resets the board
- initializes the current turn
- updates the status message

If the computer starts, the AI move is scheduled automatically.

### 8.2 Clicking a Cell

When the player clicks a cell, `on_cell_clicked()` is executed. The function:

1. checks whether the match is already over
2. checks whether input is allowed
3. applies the move using `make_move()`
4. refreshes the board
5. verifies win or draw conditions
6. switches turns
7. triggers the AI if needed

### 8.3 AI Turn

The AI turn is handled in `app_run_ai_turn()`. This function:

1. calls `find_best_move()`
2. places the computer move on the board
3. updates the interface
4. verifies whether the match ended
5. hands the turn back to the human if needed

## 9. Memory Management

Memory management is important in a C project.

### 9.1 Dynamic Allocation

The board uses dynamic allocation through `malloc()`. This is useful because it demonstrates manual memory handling in C.

### 9.2 Memory Release

The function `free_board()` releases:

- each allocated row
- the row pointer array
- the board structure itself

The GTK application state is also released when the window is destroyed.

This prevents memory leaks and shows proper ownership of allocated resources.

## 10. Design Decisions

Some important design choices were made during development:

- The interface and game logic were separated to improve clarity.
- GTK was chosen because it supports graphical desktop applications in pure C.
- Minimax was chosen because it is a classic AI solution for Tic-Tac-Toe.
- CSS styling was used to give the interface a more polished and modern appearance.
- The board buttons are updated dynamically so the interface always reflects the real board state.

## 11. Possible Questions and Answers

### Why use GTK?

GTK allows the project to have a real graphical interface while keeping the implementation in C.

### Why separate `game.c` and `gtk_ui.c`?

Because game rules and interface responsibilities are different. This makes the project easier to explain, test, and maintain.

### How does the program know who won?

It checks all rows, columns, and diagonals for three equal symbols.

### Why is the AI strong?

Because Minimax evaluates all possible future plays and always selects the optimal one.

### Can the AI lose?

If the Minimax logic is correct, the AI should never lose. At worst, it draws.

### Why use dynamic memory for the board?

It shows explicit memory management in C and makes the structure flexible and educational.

## 12. Conclusion

This project combines several important programming concepts:

- modular design
- event-driven graphical interfaces
- dynamic memory management
- validation of game rules
- artificial intelligence through Minimax

It is a strong academic project because it is not only functional, but also demonstrates clear separation of concerns and a well-known decision-making algorithm.

The result is a complete desktop Tic-Tac-Toe application that can be used both as a playable game and as a study case for C programming, GTK interfaces, and AI fundamentals.
