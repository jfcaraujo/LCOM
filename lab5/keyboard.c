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

void (kbc_ph)()
{
  uint32_t stat;

  if(sys_inb_cnt(STAT_REG, &stat)) return;
  /* loop while 8042 output buffer is empty*/

  if( stat & OBF ) {
    if( (stat & AUX_ERR) == 0) { 
      if(sys_inb_cnt(OUT_BUF, &scanCode)) return;

      if ( (stat & (PAR_ERR | TO_ERR)) != 0) //Checks if there was any error or if the output comes from the mouse
        scanCode = 0;  
    }
  } 
  else scanCode = 0;

}

int (scanCodeHandler)(bool *make, uint8_t *size, uint8_t *bytes)//simplified function to read the scanCode
{
  int handler = 0;

  if(scanCode != 0) //Means that there wasn't any error while reading the output buffer
  {
    if(escReleased()) handler = 2; //Loop Ends

      if(scanCode != BYTES_MSB)  {
        if(*size) {
            (*size)++;
            bytes[1] = scanCode;
        }
        else {
          (*size)++;
          bytes[0] = scanCode;
        }

        if(scanCode & MSB) *make = false;
        else *make = true;

        if(handler == 2) return handler; //Means that the ESC key breakcode should be printed and loop should end
        else return 1; //Means that the scancode should be printed and size should be put at zero and jump to the next iteration
      }
      else {
        bytes[0] = scanCode;
        (*size)++;
        return 0; // Means that the second byte of the scancode should be read in the next iteration
      }
  }
  return 0; //There was an error or the OUT_BUF was empty. Should proceed to the nest iteration
}

void (issueCommand)(port_t port, u32_t cmd)
{
  uint32_t stat;

  while( true ) {
    sys_inb(STAT_REG, &stat); /* assuming it returns OK */
    /* loop while 8042 input buffer is not empty */
    if( (stat & IBF) == 0 ) {
      sys_outb(port, cmd); /* no args command */
      return;
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
}

bool (escReleased)()
{
  if (scanCode == ESC_BREAKCODE) return true;
  else return false;
}
