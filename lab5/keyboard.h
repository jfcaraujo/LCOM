#pragma once

#include <lcom/lcf.h>
#include <minix/syslib.h>
#include <stdint.h>
#include <minix/sysutil.h>

#include "i8042.h"

#ifdef LAB3
int sys_inb_cnt(port_t port, uint32_t *byte);
#else
#define sys_inb_cnt(p,q) sys_inb(p,q)
#endif

int (keyboard_subscribe_int)(uint8_t *bit_no);
int (keyboard_unsubscribe_int)();
void (kbc_ih)();
void (kbc_ph)();
void (issueCommand)(port_t port, u32_t cmd);
bool (escReleased)();
int (scanCodeHandler)(bool *make, uint8_t *size, uint8_t *bytes);
