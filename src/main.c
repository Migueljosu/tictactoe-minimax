/**
 * main.c
 * Main program entry point for Tic-Tac-Toe with Minimax AI
 * Fundamentals of Programming 2 Project
 */

#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "utils.h"
#include "minimax.h"

/**
 * Runs a two-player game mode
 */
void play_two_players() {
    Board* game_board = create_board();
    char current_player = 'X';
    int row, col;
    int game_over = 0;
    
    while (!game_over) {
        clear_screen();
        printf("====================================\n");
        printf("         TWO PLAYER MODE\n");
        printf("====================================\n\n");
        
        display_board(game_board);
        printf("\nPlayer %c's turn\n", current_player);
        printf("Enter row and column (1-3, or 0 to quit): \n");
        
        // Get row input
        row = get_valid_input(0, 3, "Row") - 1;  // Convert to 0-based index
        if (row == -1) {  // User entered 0
            break;
        }
        
        // Get column input
        col = get_valid_input(1, 3, "Column") - 1;  // Convert to 0-based index
        
        // Try to make the move
        if (make_move(game_board, row, col, current_player)) {
            // Check if current player won
            if (check_win(game_board, current_player)) {
                clear_screen();
                display_board(game_board);
                printf("\nPLAYER %c WINS!\n", current_player);
                printf("\nCongratulations!\n\n");
                game_over = 1;
                wait_for_enter();
            }
            // Check for draw
            else if (is_board_full(game_board)) {
                clear_screen();
                display_board(game_board);
                printf("\nIT'S A DRAW!\n");
                printf("\nGood game!\n\n");
                game_over = 1;
                wait_for_enter();
            }
            else {
                // Switch player
                current_player = (current_player == 'X') ? 'O' : 'X';
            }
        } else {
            printf("\nInvalid move! Press Enter to try again...");
            wait_for_enter();
        }
    }
    
    free_board(game_board);
}

/**
 * Runs the computer vs player mode
 */
void play_vs_computer() {
    Board* game_board = create_board();
    char human = 'X';
    char computer = 'O';
    int row, col;
    int game_over = 0;
    int player_turn = 1;  // 1 for human, 0 for computer
    
    // Let player choose symbol
    clear_screen();
    printf("====================================\n");
    printf("      VS COMPUTER MODE\n");
    printf("====================================\n\n");
    printf("Choose your symbol:\n");
    printf("1. Play as X (goes first)\n");
    printf("2. Play as O (goes second)\n");
    int choice = get_valid_input(1, 2, "Choice");
    
    if (choice == 1) {
        human = 'X';
        computer = 'O';
        player_turn = 1;  // Human starts
    } else {
        human = 'O';
        computer = 'X';
        player_turn = 0;  // Computer starts
    }
    
    while (!game_over) {
        clear_screen();
        printf("====================================\n");
        printf("      VS COMPUTER MODE\n");
        printf("====================================\n\n");
        
        display_board(game_board);
        
        if (player_turn) {
            // Human turn
            printf("\nYour turn (%c)\n", human);
            printf("Enter row and column (1-3, or 0 to quit): \n");
            
            row = get_valid_input(0, 3, "Row") - 1;
            if (row == -1) break;
            
            col = get_valid_input(1, 3, "Column") - 1;
            
            if (make_move(game_board, row, col, human)) {
                if (check_win(game_board, human)) {
                    clear_screen();
                    display_board(game_board);
                    printf("\nYOU WIN!\n");
                    printf("\nCongratulations!\n\n");
                    game_over = 1;
                    wait_for_enter();
                } else if (is_board_full(game_board)) {
                    clear_screen();
                    display_board(game_board);
                    printf("\nIT'S A DRAW!\n");
                    game_over = 1;
                    wait_for_enter();
                } else {
                    player_turn = 0;  // Switch to computer
                }
            } else {
                printf("\nInvalid move! Press Enter to try again...");
                wait_for_enter();
            }
        } else {
            // Computer turn
            printf("\nComputer's turn (%c)...\n", computer);
            wait_for_enter();  // Give user time to see computer's turn
            
            Move best = find_best_move(game_board, computer, human);
            
            if (best.row != -1 && best.col != -1) {
                make_move(game_board, best.row, best.col, computer);
                
                if (check_win(game_board, computer)) {
                    clear_screen();
                    display_board(game_board);
                    printf("\nCOMPUTER WINS!\n");
                    printf("\nBetter luck next time!\n\n");
                    game_over = 1;
                    wait_for_enter();
                } else if (is_board_full(game_board)) {
                    clear_screen();
                    display_board(game_board);
                    printf("\nIT'S A DRAW!\n");
                    game_over = 1;
                    wait_for_enter();
                } else {
                    player_turn = 1;  // Switch to human
                }
            } else {
                // No moves left (shouldn't happen if board not full)
                game_over = 1;
            }
        }
    }
    
    free_board(game_board);
}

/**
 * Displays the main menu and handles user choice
 */
void show_menu() {
    int choice;
    
    do {
        clear_screen();
        printf("====================================\n");
        printf("   TIC-TAC-TOE WITH MINIMAX AI\n");
        printf("   Fundamentals of Programming 2\n");
        printf("====================================\n\n");
        printf("1. Two Players\n");
        printf("2. vs Computer (Minimax AI)\n");
        printf("3. How to Play\n");
        printf("4. Exit\n\n");
        
        choice = get_valid_input(1, 4, "Choose an option");
        
        switch(choice) {
            case 1:
                play_two_players();
                break;
            case 2:
                play_vs_computer();
                break;
            case 3:
                clear_screen();
                printf("====================================\n");
                printf("         HOW TO PLAY\n");
                printf("====================================\n\n");
                printf("Tic-Tac-Toe is a classic two-player game.\n");
                printf("Players take turns placing X and O on a 3x3 grid.\n");
                printf("The first player to get 3 in a row (horizontally,\n");
                printf("vertically, or diagonally) wins!\n\n");
                printf("Controls:\n");
                printf("- Enter row and column numbers from 1 to 3\n");
                printf("- Enter 0 at any time to quit the current game\n\n");
                printf("VS Computer Mode:\n");
                printf("- The computer uses the Minimax algorithm\n");
                printf("- It is unbeatable! The best you can do is draw\n\n");
                wait_for_enter();
                break;
            case 4:
                printf("\nThanks for playing! Goodbye!\n");
                break;
        }
        
    } while(choice != 4);
}

/**
 * Main function - program entry point
 */
int main() {
    show_menu();
    return 0;
}