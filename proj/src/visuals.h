#ifndef VISUALS_H
#define VISUALS_H

#include <lcom/lcf.h>
#include <minix/syslib.h>
#include <stdint.h>
#include <minix/sysutil.h>
#include <stdio.h>

#include "game.h"
#include "graphics.h"
#include "gamePlay.h"
#include "asteroid.h"
#include "rotation.h"
#include "fireball.h"
#include "highscores.h"

/** @defgroup Visuals Visuals
* @{
*/

/**
 * @brief Copies the content in the second buffer to the VRAM, and calls updateFrame()
 * 
 * @param game Pointer to Game struct with the game information
 */
void (showFrame)(Game* game);

/**
 * @brief Checks the current state of the game, and prints the corret menu if it is in a menu state. The the game is running, this funtion is responsible for controlling the game: Prints the mouse, the ship, creates and moves fireballs and asteroids, detects collisions, and checks if the round has finished
 * 
 * @param game Pointer to Game struct with the game information
 */
void (updateFrame)(Game* game);

/**
 * @brief Updates the ship position and its roation and prints is on the screen
 * 
 * @param game Pointer to Game struct with the game information
 */

void (print_ship)(Game* game);

/**
 * @brief Check the current position of the mouse so that it does not exceed the screen limits and prints is on the screen
 * 
 * @param game Pointer to Game struct with the game information
 */
void (print_mouse)(Game* game);

/** @} end of Visuals*/
#endif /* VISUALS_H */
