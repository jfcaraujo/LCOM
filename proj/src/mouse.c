#include "mouse.h"

int ms_hook_id = 2; //hook_id 0 is reserved for the timer, and 1 for the keyboard
uint8_t byte;

int (mouse_subscribe)(uint8_t *bit_no) //Subscribed mouse interrupts
{   
    *bit_no = (uint8_t) ms_hook_id;

    if(sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &ms_hook_id)) return 1;

    return 0;
}

int (mouse_unsubscribe)()  //Unsubscribed mouse interrupts
{
    if(sys_irqrmpolicy(&ms_hook_id)) return 1;

    return 0;
}

void (mouse_ih)() //Reads output buffer if there was not any error
{
    uint32_t stat, output;

    if(sys_inb(STAT_REG, &stat)) return;
    /* loop while 8042 output buffer is empty*/
    if( stat & OBF ) {
        if(stat & AUX_ERR) {
            if(sys_inb(OUT_BUF, &output)) return;

            if ( (stat & (PAR_ERR | TO_ERR)) != 0) //Check if there was any error
                byte = 0;  
            else byte = (uint8_t) output;
        }

   } 
   else byte = 0;
}

void (store_packet)(uint8_t *bytes, uint8_t *size) //Stores in the array the bytes from the packet
{
    bytes[*size] = byte;
    (*size)++;
}

int (issueCommand)(port_t port, u32_t cmd) //Reads the status register to check for errors and writes to port specified in first argument the command in the second argument 
{
  uint32_t stat, data;

  while( true ) {
    if(sys_inb(STAT_REG, &stat)) return 1; 

    //Discard output buffer
	if (stat & OBF) 
		sys_inb(OUT_BUF, &data); /* assuming it returns OK */
	
    /* loop while 8042 input buffer is not empty */
    if( (stat & IBF) == 0 ) {
      if(sys_outb(port, cmd)) return 1; 
      return 0;
    }
    tickdelay(micros_to_ticks(DELAY_US)); 
  }
}

int (write_mouse_command)(u32_t command) //Writes the command specified in its argument to the mouse and checks the acknowledgement
{
    uint32_t response;

    while(true) { //If the response to the command is a NACK the function tries to write it again
        if( issueCommand(KBC_CMD_REG, WRT_BYTE_MS)) return 1;
        if( issueCommand(IN_BUF, command)) return 1;

        if( sys_inb(OUT_BUF, &response)) return 1;

        if(response == ERROR) return 1; //If the response is an error, the function returns 1
        else if (response == ACK) return 0; //If the response is OK the function return 0

        tickdelay(micros_to_ticks(DELAY_US));
    }
}


