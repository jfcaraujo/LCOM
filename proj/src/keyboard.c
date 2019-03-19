#include "keyboard.h"

int kbd_hook_id = 1;//if it was 0 it would conflit with the timer
uint32_t inb_calls = 0;

#ifdef LAB3
int sys_inb_cnt(port_t port, uint32_t *byte)
{
  if(sys_inb(port, byte)) return 1;
  inb_calls++;//increments a global counter variable.
  return 0;
}
#endif

int (keyboard_subscribe_int)(uint8_t *bit_no) 
{
  *bit_no = (uint8_t) kbd_hook_id; //bits_no = 0

  if(sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id)) return 1;

  return 0;
}

int (keyboard_unsubscribe_int)() {

  if(sys_irqrmpolicy(&kbd_hook_id)) return 1;

  return 0;
}

uint32_t scanCode = 0; 

void (kbc_ih)()
{
  uint32_t stat;

  if(sys_inb_cnt(STAT_REG, &stat)) return;
  /* loop while 8042 output buffer is empty*/
  if( stat & OBF ) { 
    if(sys_inb_cnt(OUT_BUF, &scanCode)) return;

    if ( (stat & (PAR_ERR | TO_ERR)) != 0) //Check if there was any error
      scanCode = 0;  
  } 
  else scanCode = 0;
}

