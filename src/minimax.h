/**
 * minimax.h
 * Minimax algorithm header for Tic-Tac-Toe AI
 * Fundamentals of Programming 2 Project
 */

#ifndef MINIMAX_H
#define MINIMAX_H

#include "game.h"

/**
 * Represents a move with its position and score
 */
typedef struct {
    int row;
    int col;
    int score;
} Move;

/**
 * Finds the best move for the computer player
 * @param board Current game board
 * @param computer Player symbol for computer ('X' or 'O')
 * @param human Player symbol for human opponent
 * @return The best move found by minimax algorithm
 */
Move find_best_move(Board* board, char computer, char human);

/**
 * Minimax algorithm implementation
 * @param board Current game board
 * @param is_maximizing True if current player is computer (maximizing)
 * @param computer Computer symbol
 * @param human Human symbol
 * @return Score for current board state
 */
int minimax(Board* board, int is_maximizing, char computer, char human);

/**
 * Evaluates the current board state
 * @param board Current game board
 * @param computer Computer symbol
 * @param human Human symbol
 * @return +10 if computer wins, -10 if human wins, 0 otherwise
 */
int evaluate_board(Board* board, char computer, char human);

#endif