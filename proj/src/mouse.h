#ifndef MOUSE_H
#define MOUSE_H

#include <lcom/lcf.h>
#include <minix/syslib.h>
#include <stdint.h>
#include <minix/sysutil.h>

#include "i8042.h"
#include "game.h"

/** @defgroup Mouse Mouse
* @{
*/

/**
 * @brief Subscribes mouse interrupts
 * 
 * @param bit_no Pointer to bit number to be set in the interrupt mask
 * @return returns 0 if successfull and -1 if an error occured
 */
int (mouse_subscribe)(uint8_t *bit_no);  

/**
 * @brief Unsubscribes mouse interrupts
 * 
 * @return returns 0 if successfull and -1 if an error occured
 */
int (mouse_unsubscribe)(); 

/**
 * @brief Reads output buffer if there was not any error
 */
void (mouse_ih)();


/**
 * @brief Stores in the array the bytes from the packet
 * 
 * @param bytes Base address of the array of bytes to store the packet
 * @param size Pointer to number of the current size of the array
 */
void (store_packet)(uint8_t *bytes, uint8_t *size);

/**
 * @brief Reads the status register to check for errors and writes to port specified in first argument the command in the second argument
 * 
 * @param port Address of the port to write command to
 * @param command Command to write
 */
int (issueCommand)(port_t port, u32_t command);

/**
 * @brief Writes the command specified in its argument to the mouse and checks the acknowledgement
 * 
 * @param command Command that is going to be written
 */
int (write_mouse_command)(u32_t command);

/** @} end of Mouse */
#endif /* MOUSE_H */
