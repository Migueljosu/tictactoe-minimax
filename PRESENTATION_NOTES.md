# Presentation Notes

## Short Introduction

This project is a Tic-Tac-Toe desktop application written in C using GTK 3. It supports two modes: human versus human and human versus computer. The AI is based on the Minimax algorithm, which allows it to choose the best possible move.

## What I Built

- a graphical interface in C using GTK
- a complete Tic-Tac-Toe game engine
- an AI opponent using Minimax
- a modular structure separating interface, game logic, and AI

## Project Structure

- `main.c`: starts the GTK application
- `gtk_ui.c`: creates the interface and handles user interaction
- `game.c`: implements board rules and move validation
- `minimax.c`: implements the computer decision logic

## How the Game Works

The board is stored as a 3x3 matrix. Each cell can contain `X`, `O`, or be empty.

When a player clicks a cell:

1. the program checks if the move is valid
2. it places the symbol on the board
3. it checks for a win or a draw
4. it changes the turn
5. if the mode is against the computer, the AI chooses its move

## How the AI Works

The AI uses Minimax.

- it simulates all possible future moves
- it gives positive scores to winning computer states
- it gives negative scores to winning human states
- it chooses the move with the best final score

Because Tic-Tac-Toe is a small game, the algorithm can search the entire game tree.

## Why the Structure Matters

I separated the interface from the game logic because:

- it makes the code easier to read
- it makes debugging easier
- it makes the project easier to explain
- the interface can change without rewriting the game rules

## Key Technical Points

- written in `C`
- graphical interface built with `GTK 3`
- compiled with `GCC`
- build automated with a `Makefile`
- uses dynamic memory allocation for the board

## Likely Questions

### Why did you use GTK?

Because I wanted a graphical interface while keeping the project in pure C.

### Why did you use Minimax?

Because it is a classic decision algorithm for turn-based games and fits Tic-Tac-Toe very well.

### Can the AI be beaten?

No, if Minimax is implemented correctly. The best a human can do is draw.

### Why separate the files?

To keep responsibilities clear:

- UI in one module
- rules in another
- AI in another

## Short Conclusion

This project is a complete Tic-Tac-Toe application that demonstrates GUI programming, modular design, game logic, and basic artificial intelligence in C.
