#pragma once

#include <lcom/lcf.h>
#include <minix/syslib.h>
#include <stdint.h>
#include <minix/sysutil.h>
#include <stdio.h>

#include "game.h"

/** @defgroup Rotation Rotation
* @{
*/

/**
 * @brief Rotates the given xpm image the angles given by its parameter
 * 
 * @param game Pointer to Game struct with the game information
 * @param sprite Struct of type xom_image_t of the sprite that is going to be rotated
 * @param angle Angle that the sprite is going to rotate by
 * @return pointer to the base bixel of the new rotated sprite
 */
uint8_t* (rotate_sprite)(Game* game, xpm_image_t sprite, double angle);

/**
 * @brief Auxiliar function of rotate_sprite that calculates the rotation on the x axis 
 * 
 * @param angle Angle that the sprite is going to rotate by
 * @param x_axis 1 if we want the value for the x coordinate after  rotation 0 if we dont
 * @param y_axis 1 if we want the value for the y coordinate after rotation 0 if we dont
 * @return Value to use in the algorithm
 */
double (rotate_x)(double angle, double x_axis, double y_axis);

/**
 * @brief Auxiliar function of rotate_sprite that calculates the rotation on the y axis 
 * 
 * @param angle Angle that the sprite is going to rotate by
 * @param x_axis 1 if we want the value for the x coordinate after  rotation 0 if we dont
 * @param y_axis 1 if we want the value for the y coordinate after rotation 0 if we dont
 * @return Value to use in the algorithm
 */
double (rotate_y)(double angle, double x_axis, double y_axis);

/** @} end of Rotation */
