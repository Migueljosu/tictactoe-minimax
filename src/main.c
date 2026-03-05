/**
 * main.c
 * Main program entry point for Tic-Tac-Toe with Minimax AI
 * Fundamentals of Programming 2 Project
 */

#include <stdio.h>
#include <stdlib.h>
#include "game.h"

/**
 * Main function - program entry point
 * Currently tests basic board functionality
 * Will be expanded with menu and game modes
 */
int main() {
    printf("====================================\n");
    printf("   TIC-TAC-TOE WITH MINIMAX AI\n");
    printf("   Fundamentals of Programming 2\n");
    printf("====================================\n\n");
    
    // Test board creation and display
    printf("Creating test board...\n");
    Board* game_board = create_board();
    
    printf("Empty board:\n");
    display_board(game_board);
    
    // Clean up memory
    free_board(game_board);
    printf("\nMemory freed successfully. Program ending.\n");
    
    return 0;
}