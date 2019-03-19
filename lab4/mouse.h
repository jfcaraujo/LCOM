#pragma once

#include <lcom/lcf.h>
#include <minix/syslib.h>
#include <stdint.h>
#include <minix/sysutil.h>

#include "i8042.h"

typedef enum {INIT, DRAW1, VERTEX, DRAW2} state_t; //Indicates which part of the drawing is currently being worked on
typedef enum {LDOWN, LUP, RDOWN, RUP, WRONGKEY} ev_type_t; //Indicates which event the mouse is in (left down, left up, rigth down, rigth up, middle key or more than 1 keys pressed)

int (mouse_subscribe)(uint8_t *bit_no);  //Subscribed mouse interrupts
int (mouse_unsubscribe)();  //Unsubscribed mouse interrupts
void (mouse_ih)(); //Reads output buffer if there was not any error
void (store_packet)(uint8_t *bytes, uint8_t *size); //Stores in the array the bytes from the packet
void (packet_handler)(uint8_t *bytes, struct packet *pp); //Parses the correct values to the struct packet
void (mouse_poll)(); //Reads the output buffer if there was not any error and if the information came from the mouse
int (issueCommand)(port_t port, u32_t command);
int (write_mouse_command)(u32_t command);
int (resetKBC)(); //Resets the KBC command byte to Minix's default value

//STATE MACHINE
void (reset_line_data)(); //Resets the lengths mesured
bool (first_line_completed)(uint8_t x_len, uint8_t tolerance, struct packet *pp); //Checks if the first line was completed
bool (second_line_completed)(uint8_t x_len, uint8_t tolerance, struct packet *pp);//Checks if the second line was completed
void (first_line_handler)(int negative_tolerance, struct packet *pp); //Handles packets for the first line
void (second_line_handler)(int x_tol, uint8_t y_tol, struct packet *pp); //Handles packets for the second line
void (event_detector)(struct packet *pp, ev_type_t *evt); //Reads the events of the mouse
bool (event_handler)(ev_type_t evt, struct packet *pp, uint8_t x_len, int negative_tolerance, uint8_t tolerance);//Handles the events of the mouse, returns false if the movement was completed

