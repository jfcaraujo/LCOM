#pragma once

#include <lcom/lcf.h>
#include <minix/syslib.h>
#include <stdint.h>
#include <minix/sysutil.h>

#include <math.h>

#include "graphicsMACROS.h"

//Copys to the VRAM the sprite specified in its argument at the specified coordinates
int (print_sprite)(uint16_t x, uint16_t y, char* sprite, int width, int height);
//Clears the previously displayed sprite and prints a new one at the new coordinates
//xDirection = -1, Sprite moves to the left
//xDirection = 0, Sprite does not move horizantally
//xDirection = 1, Sprite moves to the right
//yDirection = -1, Sprite moves up
//yDirection = 0, Sprite does not move vertically
//yDirection = 1, Sprite moves down
int (move_sprite)(char* sprite, int width, int height, uint16_t* x, uint16_t* y, int16_t speed, int16_t* framesCounter, int8_t xDirection, int8_t yDirection);
//
int32_t (select_color)(uint16_t row,uint16_t column, uint32_t first, uint8_t step, uint8_t no_rectangles);
unsigned (getHeight)();
unsigned (getWidth)();
