// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "mouse.h"
#include "i8042.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

extern uint8_t byte;

int (mouse_test_packet)(uint32_t cnt) {

    int ipc_status, r;
    message msg;
    uint8_t bit_no;
    uint32_t irq_set;

    uint32_t packetCounter = 0; //Counter of the number of packets read and printed
    uint8_t bytes[3], size = 0; //Stores the bytes of each packet and saves its current size

    struct packet pp;

    if(write_mouse_command(EN_DATA_RP)) return 1; //Enables data reporting

    if(mouse_subscribe(&bit_no)) return 1;  //Subcribed mouse interrupts
    irq_set = (uint32_t) (BIT(bit_no));

    while (packetCounter < cnt) //Ends after processing the given number of packets
    {

      //Get a request message
      if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("driver_receive failes with: %d", r);
        continue;
      }

     if(is_ipc_notify(ipc_status)) { //received notification
      switch(_ENDPOINT_P(msg.m_source)) { 
        case HARDWARE: //hardware interrupt notification
          if(msg.m_notify.interrupts & irq_set) { //subscribed mouse interrupt

            mouse_ih(); //Reads output buffer if there was not any error
            store_packet(bytes, &size); //Stores in the array the bytes from the packet
            
            if(size != 3) continue;

            packet_handler(bytes, &pp); //Parses the correct values to the struct packet
            mouse_print_packet(&pp); //Prints the struct packet in a user friendly way
            size = 0; //Resets the size so it can start reading a new packet in the next iteration
            packetCounter++; //Increments the number of packets read and printed
      
           }
        default:
          break; 
       }
      }
     }

    if(mouse_unsubscribe()) return 1; //Unsubscribed mouse interrupts
    if(write_mouse_command(DIS_DATA_RP)) return 1; //Disables data reporting
    
    return 0;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {

  struct packet pp;

  uint32_t packetCounter = 0; //Counter of the number of packets read and printed
  uint8_t bytes[3], size = 0; //Stores the bytes of each packet and saves its current size

  uint32_t period_micros = period * 1000; //Stores the period value in micro seconds

  while (packetCounter < cnt) 
  {
    if(write_mouse_command(READ_DATA)) return 1; //Writes a read data instruction to the mouse 

    while(size < 3) { //Reads the output buffer 3 times. One for each of the packet bytes
      mouse_poll(); //Reads the output buffer if there was not any error and if the information came from the mouse
      store_packet(bytes, &size); //Stores in the array the bytes from the packet
    }

    packet_handler(bytes, &pp); //Parses the correct values to the struct packet
    mouse_print_packet(&pp); //Prints the struct packet in a user friendly way
    size = 0; //Resets the size so it can start reading a new packet in the next iteration
    packetCounter++; //Increments the number of packets read an printed
    
    tickdelay(micros_to_ticks(period_micros)); //Waits for a response for a period of time specified in period_micros in micro seconds
  }
  
  if(write_mouse_command(SET_STREAM_MODE )) return 1; //Sets stream mode
  if(write_mouse_command(DIS_DATA_RP)) return 1; //Disables data reporting
  if(resetKBC()) return 1; //Resets the KBC command byte to Minix's default value

  return 0;
}

extern uint32_t interruptsCounter; //Counter for mouse interrupts
int (mouse_test_async)(uint8_t idle_time) {

    int ipc_status, r;
    message msg;
    uint8_t bit_no;
    uint32_t mouse_irq_set, timer_irq_set;

    uint8_t elapsedTime = 0; //Counts the elapsed time since the last mouse interrupt
    uint8_t bytes[3], size = 0; //Stores the bytes of each packet and saves its current size

    struct packet pp;

    if(write_mouse_command(EN_DATA_RP)) return 1; //Enables data reporting

    if(mouse_subscribe(&bit_no)) return 1; //Subscribes mouse interrupts
    mouse_irq_set = (uint32_t) (BIT(bit_no)); 

    if(timer_subscribe_int(&bit_no)) return 1; //Subscribes timer interrupts
    timer_irq_set = (uint32_t) (BIT(bit_no));

    while (elapsedTime < idle_time) //Ends if it receives no packets for idle_time seconds
    {

      //Get a request message
      if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("driver_receive failes with: %d", r);
        continue;
      }

     if(is_ipc_notify(ipc_status)) { //received notification
      switch(_ENDPOINT_P(msg.m_source)) { 
        case HARDWARE: //hardware interrupt notification
          if(msg.m_notify.interrupts & mouse_irq_set) { //subscribed mouse interrupt

            mouse_ih(); //Reads output buffer if there was not any error
            interruptsCounter = 0; //Resets the counter of mouse interrupts

            store_packet(bytes, &size); //Parses the correct values to the struct packet
            
            if(size != 3) continue;

            packet_handler(bytes, &pp); //Parses the correct values to the struct packet
            mouse_print_packet(&pp); //Prints the struct packet in a user friendly way
            size = 0; //Resets the size so it can start reading a new packet in the next iteration
      
           }

          if(msg.m_notify.interrupts & timer_irq_set) {
            timer_int_handler(); //Increments the number of timer 0 interrupts
            if(interruptsCounter % sys_hz() == 0) //If a second has passed
              elapsedTime++;
          }
        default:
          break; 
       }
      }
     }

    if(mouse_unsubscribe()) return 1; //Unsubscribed mouse interrupts
    if(timer_unsubscribe_int()) return 1; //Unsubscribed timer interrupts
    if(write_mouse_command(DIS_DATA_RP)) return 1; //Disables data reporting
    
    return 0;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {

    int ipc_status, r;
    message msg;
    uint8_t bit_no;
    uint32_t irq_set;
     
    uint8_t bytes[3], size = 0; //Stores the bytes of each packet and saves its current size

    struct packet pp;

    int negative_tolerance = tolerance; 
    negative_tolerance *= -1; //Cast to an integer to compare with delta x and delta y
    
    ev_type_t evt;
    bool running = true;

    if(write_mouse_command(EN_DATA_RP)) return 1; //Enables data reporting 

    if(mouse_subscribe(&bit_no)) return 1; //Subscribes mouse interrupts
    irq_set = (uint32_t) (BIT(bit_no));

    while (running) //Ends after the shape is drawn
    {

      //Get a request message
      if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("driver_receive failes with: %d", r);
        continue;
      }

     if(is_ipc_notify(ipc_status)) { //received notification
      switch(_ENDPOINT_P(msg.m_source)) { 
        case HARDWARE: //hardware interrupt notification
          if(msg.m_notify.interrupts & irq_set) { //subscribed mouse interrupt

            mouse_ih(); //Reads output buffer if there was not any error
            store_packet(bytes, &size); //Stores in the array the bytes from the packet
            if(size != 3) continue;
            packet_handler(bytes, &pp); //Parses the correct values to the struct packet          
            mouse_print_packet(&pp); //Prints the struct packet in a user friendly way
            size = 0; //Resets the size so it can start reading a new packet in the next iteration

            event_detector(&pp, &evt); //Reads the events of the mouse
            running = event_handler(evt, &pp, x_len, negative_tolerance, tolerance); //Handles the events of the mouse, returns false if the movement was completed
          }

        default:
          break; 
       }
      }
     }

    if(mouse_unsubscribe()) return 1; //Unsubscribed mouse interrupts
    if(write_mouse_command(DIS_DATA_RP)) return 1; //Disables data reporting

  return 0;
}
