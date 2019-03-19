#pragma once

#include <lcom/lcf.h>
#include <minix/syslib.h>
#include <stdint.h>
#include <minix/sysutil.h>

#include <math.h>

#include "graphicsMACROS.h"

/** @defgroup Graphics Graphics
* @{
*/

/**
* @brief Copys to the the second buffer the sprite specified in its argument at the specified coordinates
* 
* @param x X coordinate of the sprite
* @param y Y coordinate of the sprite
* @param xpm XPM struct of the sprite 
* @return 0 if it was successfull, 1 if it was not
*/
int (print_sprite)(int16_t x, int16_t y, xpm_image_t xpm);

/**
 * @brief Returns Horizontal resolution of the screen
 * 
 * @return Horizontal resolution of the screen
 */
unsigned (getHorRes)();

/**
 * @brief Return the Vetical resolution of the screen
 * 
 * @return Vertical Resolution of the screen
 */
unsigned (getVerRes)();

/**
 * @brief Returns the size of the screen (Vetical resolution * horizontal resolution)
 * 
 * @return Size of the Screen
 */
size_t (getScreenSize)();

/**
 * @brief Initializes the global variable second_buffer with the value in its parameter
 * 
 * @param new_address New pointer value of the second buffer
 */
void (setSecondBuffer)(uint8_t* new_address);

/**
 * @brief Return the number of bytes per pixel used in the current graphics mode
 * 
 * @return Number of bytes per pixel
 */
int (getBytesPerPixel)();

/**
 * @brief Updates the coordinates given so that if the sprite exceeds the screen, it automatically moves to the other side of the screen
 * 
 * @param width Width of the sprite
 * @param height Height of the sprite
 * @param x Pointer to the x coordinate of the sprite
 * @param y Pointer to the y coordinate of the sprite
 */
void (checkCoordinates)(int width, int height, int16_t* x, int16_t* y);

/** @} end of Graphics */

