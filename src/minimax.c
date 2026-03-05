/**
 * minimax.c
 * Minimax algorithm implementation for Tic-Tac-Toe AI
 * Fundamentals of Programming 2 Project
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "minimax.h"

/**
 * Evaluates the current board state
 * @param board Current game board
 * @param computer Computer symbol
 * @param human Human symbol
 * @return +10 if computer wins, -10 if human wins, 0 otherwise
 */
int evaluate_board(Board* board, char computer, char human) {
    if (check_win(board, computer)) {
        return 10;
    } else if (check_win(board, human)) {
        return -10;
    }
    return 0;
}

/**
 * Minimax algorithm implementation
 * @param board Current game board
 * @param is_maximizing True if current player is computer (maximizing)
 * @param computer Computer symbol
 * @param human Human symbol
 * @return Score for current board state
 */
int minimax(Board* board, int is_maximizing, char computer, char human) {
    int score = evaluate_board(board, computer, human);
    
    // Terminal states
    if (score == 10 || score == -10) {
        return score;
    }
    
    if (is_board_full(board)) {
        return 0;
    }
    
    if (is_maximizing) {
        int best = INT_MIN;
        
        // Try all possible moves
        for (int i = 0; i < board->size; i++) {
            for (int j = 0; j < board->size; j++) {
                if (board->matrix[i][j] == ' ') {
                    // Make move
                    board->matrix[i][j] = computer;
                    
                    // Recursively call minimax
                    int current_score = minimax(board, 0, computer, human);
                    
                    // Undo move
                    board->matrix[i][j] = ' ';
                    
                    // Update best score
                    if (current_score > best) {
                        best = current_score;
                    }
                }
            }
        }
        return best;
    } else {
        int best = INT_MAX;
        
        // Try all possible moves
        for (int i = 0; i < board->size; i++) {
            for (int j = 0; j < board->size; j++) {
                if (board->matrix[i][j] == ' ') {
                    // Make move
                    board->matrix[i][j] = human;
                    
                    // Recursively call minimax
                    int current_score = minimax(board, 1, computer, human);
                    
                    // Undo move
                    board->matrix[i][j] = ' ';
                    
                    // Update best score
                    if (current_score < best) {
                        best = current_score;
                    }
                }
            }
        }
        return best;
    }
}

/**
 * Finds the best move for the computer player
 * @param board Current game board
 * @param computer Player symbol for computer ('X' or 'O')
 * @param human Player symbol for human opponent
 * @return The best move found by minimax algorithm
 */
Move find_best_move(Board* board, char computer, char human) {
    Move best_move;
    best_move.row = -1;
    best_move.col = -1;
    best_move.score = INT_MIN;
    
    // Try all possible moves
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            if (board->matrix[i][j] == ' ') {
                // Make move
                board->matrix[i][j] = computer;
                
                // Calculate score for this move
                int move_score = minimax(board, 0, computer, human);
                
                // Undo move
                board->matrix[i][j] = ' ';
                
                // Update best move
                if (move_score > best_move.score) {
                    best_move.row = i;
                    best_move.col = j;
                    best_move.score = move_score;
                }
            }
        }
    }
    
    return best_move;
}