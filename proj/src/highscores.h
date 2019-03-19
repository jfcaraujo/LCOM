#ifndef HIGHSCORES_H
#define HIGHSCORES_H

#include <lcom/lcf.h>
#include <minix/syslib.h>
#include <stdint.h>
#include <minix/sysutil.h>
#include <stdio.h>

#include "game.h"
#include "graphics.h"
#include "gamePlay.h"

/** @defgroup HighScores HighScores
* @{
*/

/**
 * @brief Displays on the screen the string given, at the coordinates give
 * 
 * @param game Pointer to Game struct with the game information
 * @param x X coordinate of where the string is going to be displayed
 * @param y Y coordinate of where the string is going to be displayed
 * @param string C string that is going to be displayed
 * @param size Size of the C string
 */
void (print_string)(Game* game, int x, int y, char * string, unsigned int size);

/**
 * @brief Saves into a file the top 3 highscores ever recorded on the game
 * 
 * @param game Pointer to Game struct with the game information
 */
void (save_highscores)(Game* game);

/**
 * @brief Loads from a file the top 3 highscores ever recorded. If the file does not exist, it creates a new one
 * 
 * @param game Pointer to Game struct with the game information
 */
void (load_highscores)(Game* game);

/**
 * @brief Checks if the current player achieved a new top 3 highscore
 * 
 * @param game Pointer to Game struct with the game information
 * @return Returns index to insert new highscore, 3 if it isn't a higher highscore than the others and its already full
 */
int (check_new_highscore)(Game* game);

/**
 * @brief Saves the current player's stats in the array of highscores
 * 
 * @param game Pointer to Game struct with the game information
 * @param position Index of the array of the highscores where the current player's stats are going to be inserted
 */
void (write_new_highscore)(Game* game, unsigned int position);

/**
 * @brief Prints on the screen the highscores array, with the name of each player, the score, the round reached and the date and time of when the highscore was achieved
 * 
 * @param game Pointer to Game struct with the game information
 */
void (print_highscores)(Game* game);

/**
 * @brief Auxiliary function of print_highscores() to print on the coordinates given the date of the highscore.
 * 
 * @param game Pointer to Game struct with the game information
 * @param date Pointer to struct of type Date where the highscore date information is stored
 * @param x X coordinate of where the date is going to be displayed
 * @param y Y coordinate of where the date is going to be displayed
 */
void (print_date)(Game* game, Date* date, int x, int y);

/** @} end of HighScores */
#endif /* HIGHSCORES_H */
