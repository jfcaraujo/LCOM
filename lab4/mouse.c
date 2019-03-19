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
        if(sys_inb(OUT_BUF, &output)) return;

        if ( (stat & (PAR_ERR | TO_ERR)) != 0) //Check if there was any error
            byte = 0;  
        else byte = (uint8_t) output;
   } 
   else byte = 0;
}

void (store_packet)(uint8_t *bytes, uint8_t *size) //Stores in the array the bytes from the packet
{
    bytes[*size] = byte;
    (*size)++;
}

void (packet_handler)(uint8_t *bytes, struct packet *pp) //Parses the correct values to the struct packet
{
    pp->bytes[0] = bytes[0];
    pp->bytes[1] = bytes[1];
    pp->bytes[2] = bytes[2];

    uint8_t byte1 = bytes[0];

    pp->rb = (byte1 & RB);//stores 1 if the rigth button was pressed
    pp->mb = (byte1 & MB);//stores 1 if the middle button was pressed
    pp->lb = (byte1 & LB);//stores 1 if the left button was pressed
    pp->x_ov = (byte1 & X_OV);//stores 1 if there was an overflow to the x coordinates
    pp->y_ov = (byte1 & Y_OV);//stores 1 if there was an overflow to the y coordinates

    pp->delta_x = (int16_t) bytes[1];
    pp->delta_y = (int16_t) bytes[2];
    
    //adds 1 to the left of the number (if it's negative) to complete the 16 bits
    if(byte1 & X_SIGN) pp->delta_x |= (0xFF<<8);
    if(byte1 & Y_SIGN) pp->delta_y |= (0xFF<<8);
}

void (mouse_poll)()//Reads the output buffer if there was not any error and if the data came from the mouse
{
    uint32_t stat, output;

    if(sys_inb(STAT_REG, &stat)) return;
    /* loop while 8042 output buffer is empty*/
    if( stat & OBF ) { 
        if(stat & AUX) { //Checks if the data came from the mouse
            if(sys_inb(OUT_BUF, &output)) return;

            if ( (stat & (PAR_ERR | TO_ERR)) != 0) //Check if there was any error
                byte = 0;  
            else byte = (uint8_t) output;
        }
   } 
   else byte = 0;
}

int (issueCommand)(port_t port, u32_t cmd) //Reads the status register to check for errors and writes to port specified in first argument the command in the second argument 
{
  uint32_t stat;

  while( true ) {
    if(sys_inb(STAT_REG, &stat)) return 1; 
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

int (resetKBC)() //Resets the KBC command byte to Minix's default value
{
    uint32_t response;
    uint8_t defaultKBC_command = minix_get_dflt_kbc_cmd_byte(); //Gets the Minix's default command byte

    while (true) { //If the response to the command is a NACK the function tries to write it again
        if(issueCommand(KBC_CMD_REG, WRT_CMD_BYTE)) return 1; //Command to write a command byte
        if(issueCommand(IN_BUF, defaultKBC_command)) return 1; 

        if( sys_inb(OUT_BUF, &response)) return 1;

        if(response == ERROR) return 1; //If the response is an error, the function returns 1
        else if(response == ACK) return 0; //If the response is OK the function return 0

        tickdelay(micros_to_ticks(DELAY_US));
    } 
}
//STATE MACHINE
//In the beginning, the lengths are 0 and it isn't reading any line
int length_x = 0, length_y = 0;
bool reading_first_line = false;
bool reading_second_line = false;
bool first_line_complete=false;
state_t state = INIT;

void (reset_line_data)()//Resets the lengths measured
{
    length_x = 0;
    length_y = 0;
    return;
}

bool (first_line_completed)(uint8_t x_len, uint8_t tolerance, struct packet *pp)//Checks if the first line was completed
{
    //Returns true if the line fulfills the minimum x length, the slope is greater than 1 and if the mouse movement in this packet wasn't bigger than the tolerance
    return (length_x >= x_len && length_y > length_x &&  abs(pp->delta_x) <= tolerance && abs(pp->delta_y) <= tolerance);
}

bool (second_line_completed)(uint8_t x_len, uint8_t tolerance, struct packet *pp)//Checks if the second line was completed
{
    //Returns true if the line fulfills the minimum x length, the slope absolute value is greater than 1 and if the mouse movement in this packet wasn't bigger than the tolerance
    return (length_x >= x_len && abs(length_y) > length_x &&  abs(pp->delta_x) <= tolerance && abs(pp->delta_y) <= tolerance);
}

void (first_line_handler)(int negative_tolerance, struct packet *pp)//Handles packets for the first line
{
    if (pp->delta_x >= negative_tolerance && pp->delta_y >= negative_tolerance){//If the movement is within the tolerance increments the lengths measured
        length_x += pp->delta_x;
        length_y += pp->delta_y; 
    }
    else 
        reset_line_data();    
}

void (second_line_handler)(int x_tol, uint8_t y_tol, struct packet *pp) //Handles packets for the first line
{
    if (pp->delta_x >= x_tol && pp->delta_y <= y_tol) {//If the movement is within the tolerance increments the lengths measured
        length_x += pp->delta_x;
        length_y += pp->delta_y;  
    }
    else {
        reset_line_data();
        reading_second_line=false;//In order to reset the drawing to the beginning
    }
}

void (event_detector)(struct packet *pp, ev_type_t *evt) //Reads the events of the mouse
{
    bool left_button_pressed = (pp->lb && !pp->rb && !pp->mb);//If only the left button is pressed
    bool right_button_pressed = (pp->rb && !pp->lb && !pp->mb);//If only the rigth button is pressed
    bool no_button_pressed = (!pp->rb && !pp->lb && !pp->mb);//If no button is pressed
    
         
    if (left_button_pressed) { //Event: Left buton pressed
        *evt = LDOWN;
        if(!reading_first_line)
            reading_first_line = true; //Starts reading the first line
    }                  
    else if(no_button_pressed && reading_first_line) { //Event: Left button released
        *evt = LUP;
        reading_first_line = false; //Finishes reading the first line
    }          
    else if(right_button_pressed) { //Event: Right button pressed
        *evt = RDOWN;
        if(!reading_second_line && first_line_complete) 
            reading_second_line = true; //Starts reading the second line
    }
    else if(no_button_pressed && reading_second_line) { //Event: Right button released
        *evt = RUP;
        reading_second_line = false; //Finishes reading the second line
    } 
    else if (!no_button_pressed) 
        *evt = WRONGKEY; //Event: middle key is pressed, or if more than 1 key are pressed
    
        
    return;   
}

//Handles the evnts of the mouse, returns false if the movement was completed
bool (event_handler)(ev_type_t evt, struct packet *pp, uint8_t x_len, int negative_tolerance, uint8_t tolerance)
{
    switch(state) {
    case INIT:
        reset_line_data();
        if( evt == LDOWN )//If the left button is pressed, starts the first line
            state = DRAW1;
        break;
    case DRAW1:
        if( evt == LDOWN ) //While the left button is pressed, keeps drawing the line
            first_line_handler(negative_tolerance, pp);
        else if( evt == LUP ){//If the left button is lifted, ends the first line
            if(first_line_completed(x_len, tolerance, pp)) {//Checks if the first line is valid
                state = VERTEX;//Goes to the vertex
                first_line_complete=true;}}
        else //Else resets the drawing
            state = INIT;
        break;
    case VERTEX:
        reset_line_data();//Resets the lengths
        if( evt == RDOWN )//If the rigth key is pressed
            state = DRAW2;//Starts drawing the 2 line
        else if (evt == LDOWN) { //If the left key is pressed
            state = DRAW1;//starts drawing the second line
            reset_line_data(); //Resets the lengths
        }
        else if(evt == WRONGKEY)//If the middle key is pressed, or if more than 1 keys are pressed
            state = INIT;//Resets the drawing
        break; 
    case DRAW2:
        if( evt == RDOWN )//If the rigth key is pressed
            second_line_handler(negative_tolerance, tolerance, pp);
        else if( evt == RUP ) {//If the rigth key is lifted
            if(second_line_completed(x_len, tolerance, pp))//Checks if the second line is valid
                return false;}
        else if (evt == LDOWN){ //If the left key is pressed
            state = DRAW1;//starts drawing the second line
            reset_line_data();}
        else//Else resets the drawing
            state = INIT;
        break;  
    default:
        break;    
    }

    return true;
}   

