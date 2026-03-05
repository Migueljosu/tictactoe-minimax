/**
 * game.c
 * Tic-Tac-Toe game logic implementation
 * Fundamentals of Programming 2 Project
 */

#include <stdio.h>
#include <stdlib.h>
#include "game.h"

/**
 * Creates a new empty board
 * Allocates memory for the board structure and the 3x3 matrix
 * All positions are initialized with space character ' '
 * 
 * @return Pointer to newly created board, or NULL if allocation fails
 */
Board* create_board() {
    // Allocate memory for the board structure
    Board* board = (Board*)malloc(sizeof(Board));
    if (board == NULL) {
        printf("Error: Failed to allocate memory for board structure.\n");
        return NULL;
    }
    
    board->size = 3;  // Standard Tic-Tac-Toe board
    
    // Allocate memory for the matrix rows (array of pointers)
    board->matrix = (char**)malloc(board->size * sizeof(char*));
    if (board->matrix == NULL) {
        printf("Error: Failed to allocate memory for board rows.\n");
        free(board);
        return NULL;
    }
    
    // Allocate memory for each row and initialize with spaces
    for (int i = 0; i < board->size; i++) {
        board->matrix[i] = (char*)malloc(board->size * sizeof(char));
        if (board->matrix[i] == NULL) {
            printf("Error: Failed to allocate memory for row %d.\n", i);
            // Free previously allocated rows to avoid memory leak
            for (int j = 0; j < i; j++) {
                free(board->matrix[j]);
            }
            free(board->matrix);
            free(board);
            return NULL;
        }
        
        // Initialize row with empty spaces
        for (int j = 0; j < board->size; j++) {
            board->matrix[i][j] = ' ';
        }
    }
    
    return board;
}

/**
 * Frees all memory associated with the board
 * First frees each row, then the matrix array, then the board structure
 * 
 * @param board Pointer to the board to be freed
 */
void free_board(Board* board) {
    if (board == NULL) {
        return;  // Nothing to free
    }
    
    // Free each row
    if (board->matrix != NULL) {
        for (int i = 0; i < board->size; i++) {
            if (board->matrix[i] != NULL) {
                free(board->matrix[i]);
            }
        }
        // Free the array of pointers
        free(board->matrix);
    }
    
    // Free the board structure itself
    free(board);
}

/**
 * Displays the current board state in the terminal
 * Shows a grid with row and column separators
 * 
 * @param board Current game board
 */
void display_board(Board* board) {
    if (board == NULL || board->matrix == NULL) {
        printf("Error: Cannot display NULL board.\n");
        return;
    }
    
    printf("\n");
    for (int i = 0; i < board->size; i++) {
        printf("  %c | %c | %c  \n", 
               board->matrix[i][0], 
               board->matrix[i][1], 
               board->matrix[i][2]);
        
        if (i < board->size - 1) {
            printf(" ---+---+---\n");
        }
    }
    printf("\n");
}

/**
 * Makes a move on the board
 * Validates if the position is within bounds and empty
 * 
 * @param board Current game board
 * @param row Row index (0-2)
 * @param col Column index (0-2)
 * @param player Player symbol ('X' or 'O')
 * @return 1 if move successful, 0 if invalid
 */
int make_move(Board* board, int row, int col, char player) {
    // Validate board exists
    if (board == NULL || board->matrix == NULL) {
        printf("Error: Board not initialized.\n");
        return 0;
    }
    
    // Validate position is within bounds
    if (row < 0 || row >= board->size || col < 0 || col >= board->size) {
        printf("Error: Position (%d, %d) is out of bounds.\n", row, col);
        return 0;
    }
    
    // Validate position is empty
    if (board->matrix[row][col] != ' ') {
        printf("Error: Position (%d, %d) is already occupied.\n", row, col);
        return 0;
    }
    
    // Validate player symbol
    if (player != 'X' && player != 'O') {
        printf("Error: Invalid player symbol. Use 'X' or 'O'.\n");
        return 0;
    }
    
    // Make the move
    board->matrix[row][col] = player;
    return 1;
}

/**
 * Checks if the specified player has won
 * Checks all rows, columns, and diagonals
 * 
 * @param board Current game board
 * @param player Player symbol to check ('X' or 'O')
 * @return 1 if player wins, 0 otherwise
 */
int check_win(Board* board, char player) {
    if (board == NULL || board->matrix == NULL) {
        return 0;
    }
    
    // Check rows
    for (int i = 0; i < board->size; i++) {
        if (board->matrix[i][0] == player &&
            board->matrix[i][1] == player &&
            board->matrix[i][2] == player) {
            return 1;
        }
    }
    
    // Check columns
    for (int j = 0; j < board->size; j++) {
        if (board->matrix[0][j] == player &&
            board->matrix[1][j] == player &&
            board->matrix[2][j] == player) {
            return 1;
        }
    }
    
    // Check main diagonal (top-left to bottom-right)
    if (board->matrix[0][0] == player &&
        board->matrix[1][1] == player &&
        board->matrix[2][2] == player) {
        return 1;
    }
    
    // Check other diagonal (top-right to bottom-left)
    if (board->matrix[0][2] == player &&
        board->matrix[1][1] == player &&
        board->matrix[2][0] == player) {
        return 1;
    }
    
    return 0;  // No win detected
}

/**
 * Checks if the board is completely filled (draw)
 * 
 * @param board Current game board
 * @return 1 if board is full, 0 otherwise
 */
int is_board_full(Board* board) {
    if (board == NULL || board->matrix == NULL) {
        return 0;
    }
    
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            if (board->matrix[i][j] == ' ') {
                return 0;  // Found an empty cell
            }
        }
    }
    
    return 1;  // All cells are filled
}

/**
 * Resets the board to empty state
 * Fills all positions with space character ' '
 * 
 * @param board Board to be cleared
 */
void clear_board(Board* board) {
    if (board == NULL || board->matrix == NULL) {
        return;
    }
    
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            board->matrix[i][j] = ' ';
        }
    }
}
