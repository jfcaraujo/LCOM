#pragma once

#include <lcom/lcf.h>
#include <minix/syslib.h>
#include <stdint.h>
#include <minix/sysutil.h>
#include "math.h"

#include "game.h"
#include "asteroid.h"
#include "fireball.h"
#include "rtc.h"
#include "highscores.h"

/** @defgroup GamePlay GamePlay
 * @{
 */

/**
 * @brief Detects collisions between to sprites
 * 
 * @param x1 X position of the first sprite
 * @param y1 Y position of the first sprite
 * @param width1 Width of the first sprite
 * @param height1 Height of the first sprite
 * @param x2 X position of the second sprite
 * @param y2 Y position of the second sprite
 * @param width2 Width of the second sprite
 * @param height2 Height of the second sprite
 * @return True if the was indeeed a collision
 */
bool (detectCollision)(int16_t x1, int16_t y1, uint16_t width1, uint16_t height1, int16_t x2, int16_t y2, uint16_t width2, uint16_t height2);

/**
 * @brief Detects if there was any collision between a fireball and an asteroid. If there is a collision, the fireball is destroyed and the asteroids is split in two
 * 
 * @param game Pointer to Game struct with the game information
 */
void (checkFireballCollision)(Game* game);

/**
 * @brief Detects if there was any collision betwen the ship and an asteroid. If there was, it is goind to check the player reached a new highscore
 * 
 * @param game Pointer to Game struct with the game information
 */
void (checkShipCollision)(Game* game);

/**
 * @brief Ends the current round, eliminating all the asteroids and fireballs
 * 
 * @param game Pointer to Game struct with the game information
 */
void (end_round)(Game* game);

/**
 * @brief Goes to the next round, updating the dificulty of the game
 * 
 * @param game Pointer to Game struct with the game information
 */
void (next_round)(Game* game);

/**
 * @brief Adds the stats of the player to the high scores leaderboard
 * 
 * @param game Pointer to Game struct with the game information
 */
void (add_to_highscores)(Game* game);

/**
 * @brief Detects if the player achieved a new highscore
 * 
 * @param game Pointer to Game struct with the game information
 */
bool (check_highscores)(Game* game);

/**
 *  @brief Prints on the screen the current round and score
 * 
 * @param game Pointer to Game struct with the game information
 */
void (print_HUD)(Game* game);

/**
 * @brief Calculates the length of the given number
 * 
 * @param number Positive integer number 
 * @return Length of the number given
 */
int (get_number_length)(int number);

