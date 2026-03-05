/**
 * utils.c
 * Utility functions implementation
 * Fundamentals of Programming 2 Project
 */

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

void clear_screen() {
    #ifdef _WIN32
        system("cls");  // Windows command
    #else
        system("clear");  // Linux/macOS command
    #endif
}

void wait_for_enter() {
    printf("\nPress Enter to continue...");
    while (getchar() != '\n');
    getchar();  // Wait for Enter key
}

int get_valid_input(int min, int max, const char* prompt) {
    int input;
    int result;
    
    do {
        printf("%s (%d-%d): ", prompt, min, max);
        result = scanf("%d", &input);
        
        // Clear input buffer
        while (getchar() != '\n');
        
        if (result != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (input < min || input > max) {
            printf("Input out of range. Please enter between %d and %d.\n", 
                   min, max);
        }
        
    } while (input < min || input > max || result != 1);
    
    return input;
}