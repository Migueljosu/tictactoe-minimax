/**
 * utils.h
 * Utility functions header
 * Fundamentals of Programming 2 Project
 */

#ifndef UTILS_H
#define UTILS_H

/**
 * Clears the terminal screen (platform independent)
 * Works on Windows, Linux, and macOS
 */
void clear_screen();

/**
 * Pauses execution and waits for user to press Enter
 */
void wait_for_enter();

/**
 * Gets a valid integer input from user within a range
 * @param min Minimum acceptable value
 * @param max Maximum acceptable value
 * @param prompt Message to display to user
 * @return Validated integer input
 */
int get_valid_input(int min, int max, const char* prompt);

#endif