#ifndef DEVICES_MANAGER_H
#define DEVICES_MANAGER_H

#include <lcom/lcf.h>
#include <minix/syslib.h>
#include <stdint.h>
#include <minix/sysutil.h>

#include "game.h"
#include "i8042.h"
#include "keyboard.h"
#include "mouse.h"

/** @defgroup DevicesManager DevicesManager
* @{
*/

/**
 * @brief Handles the scancode read, and tranforms it into a keyboard event. Also saves the direction in which the ship must move.
 * 
 * @param evt Pointer to variable of type keyboard_event that is going to have the information about which key was pressed
 * @param game Pointer to Game struct with the game information
 */
void (keyboard_manager)(keyboard_event_t* evt, Game* game);

/**
 * @brief Checks if the left button of the mouse is being pressed and saves that information in the right place and updates the mouse position according to mouse displacments received
 * 
 * @param game Pointer to Game struct with the game information
 */
int (mouse_manager)(Game* game);

/** @} end of DevicesManager */
#endif /* DEVICES_MANAGER_H */
