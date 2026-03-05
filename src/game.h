/**
 * game.h
 * Tic-Tac-Toe game logic header
 * Fundamentals of Programming 2 Project
 */

#ifndef GAME_H
#define GAME_H

/**
 * Board structure
 * matrix: dynamic 2D array representing the board
 * size: board dimension (always 3 for Tic-Tac-Toe)
 */
typedef struct {
    char** matrix;  // Dynamic 3x3 matrix
    int size;       // Board size (3)
} Board;

/**
 * Creates a new empty board
 * @return Pointer to allocated Board structure
 */
Board* create_board();

/**
 * Frees all memory associated with the board
 * @param board Pointer to the board to be freed
 */
void free_board(Board* board);

/**
 * Displays the current board state in the terminal
 * @param board Current game board
 */
void display_board(Board* board);

/**
 * Makes a move on the board
 * @param board Current game board
 * @param row Row index (0-2)
 * @param col Column index (0-2)
 * @param player Player symbol ('X' or 'O')
 * @return 1 if move was valid and executed, 0 otherwise
 */
int make_move(Board* board, int row, int col, char player);

/**
 * Checks if the current player has won
 * @param board Current game board
 * @param player Player symbol to check ('X' or 'O')
 * @return 1 if player wins, 0 otherwise
 */
int check_win(Board* board, char player);

/**
 * Checks if the board is full (draw)
 * @param board Current game board
 * @return 1 if board is full, 0 otherwise
 */
int is_board_full(Board* board);

/**
 * Resets the board to empty state
 * @param board Board to be cleared
 */
void clear_board(Board* board);

#endif