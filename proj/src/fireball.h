#pragma once 

#include <lcom/lcf.h>
#include <minix/syslib.h>
#include <stdint.h>
#include <minix/sysutil.h>
#include <stdio.h>

#include "game.h"
#include "rotation.h"

/** @defgroup Fireball Fireball
 * @{
 * Functions to control the fireballs
 */

/**
 * @brief Creates a fireball next to the ship, and saves the rotation angle of the ship
 * 
 * @param game Pointer to Game struct with the game information
 */
void (create_fireball)(Game* game);

/**
 * @brief Updates the firballs' locations, rotates them towards the mouse and prints them on the screen
 * 
 * @param game Pointer to Game struct with the game information
 */
void (move_Fireballs)(Game* game);

/**
 * @brief Destroyds a fireball
 * 
 * @param game Pointer to Game struct with the game information
 * @param index Index of the array of fireballs that belongs to the fireball that is going to be destroyed
 */
void (destroy_fireball)(Game* game, int index);

