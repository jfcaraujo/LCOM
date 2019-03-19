#pragma once

#include <lcom/lcf.h>
#include <minix/syslib.h>
#include <stdint.h>
#include <minix/sysutil.h>

#include "game.h"

/** @defgroup Asteroid Asteroid
 * @{
 * Functions to handle the asteroids
 */
 
/**
 * @brief Creates an asteroid with random size, at a random location
 * 
 * @param game Pointer to Game struct with the game information 
 */
void (create_asteroid)(Game* game);

/**
 * @brief Updates asteroids' positions and prints them on screen
 * 
 * @param game Pointer to Game struct with the game information 
 */
void (move_asteroids)(Game* game);

/**
 * @brief Destroys an asteroid
 * 
 * @param game Pointer to Game struct with the game information 
 * @param index Index in the array of asteroids that belongs to the one that is going to be destroyed
 */
void (destroy_asteroid)(Game* game, int index);

/**
 * @brief Splits an asteroid in two it its size is differente from small
 * 
 * @param game Pointer to Game struct with the game information 
 * @param index Index in the array of asteroids that belongs to the one that is goind to be splited
 */
void (split_asteroid)(Game* game, int index);
 
/** @} end of Asteroid */

