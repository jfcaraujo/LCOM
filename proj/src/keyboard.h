#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <lcom/lcf.h>
#include <minix/syslib.h>
#include <stdint.h>
#include <minix/sysutil.h>

#include "i8042.h"
#include "game.h"


/** @defgroup Keyboard Keyboard
* @{ 
*/

#ifdef LAB3
int sys_inb_cnt(port_t port, uint32_t *byte);
#else
#define sys_inb_cnt(p,q) sys_inb(p,q)
#endif

/**
 * @brief Subscribed keyboard interrupts
 * 
 * @param bit_no Pointer to bit number to be set in the interrupt mask
 * @return returns 0 if successfull and -1 if an error occured
 */
int (keyboard_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes mouse interrupts
 * 
 * @return returns 0 if successfull and -1 if an error occured
 */
int (keyboard_unsubscribe_int)();

/**
 * @brief Reads the output buffer and saves the scancode read
 */
void (kbc_ih)();


/** @} end of Keyboard */
#endif /* KEYBOARD_H */
