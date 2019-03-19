// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/timer.h>
// Any header files below this line should have been created by you
#include "i8042.h"
#include "keyboard.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

extern uint32_t interruptsCounter;
extern uint32_t inb_calls;

int (kbd_test_scan)(bool assembly) {

    int ipc_status, r, handler;
    message msg;
    uint8_t bit_no;
    uint32_t irq_set;
    uint8_t bytes[2], size = 0;

    bool make, receiving = true;

    if(keyboard_subscribe_int(&bit_no)) return 1; 
    irq_set = (uint32_t) (BIT(bit_no));

    while (receiving) //End when esc key is released
    {
      //Get a request message
      if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("driver_receive failes with: %d", r);
        continue;
      }

     if(is_ipc_notify(ipc_status)) { //received notification
      switch(_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: //hardware interrupt notification
          if(msg.m_notify.interrupts & irq_set) { //subscribed keyboard interrupt

            if(assembly) kbc_asm_ih();
            else kbc_ih();

            handler = scanCodeHandler(&make, &size, bytes);

            if(handler == 1) {
              if(kbd_print_scancode(make, size, bytes)) return 1;
              size = 0;
            }
            else if(handler == 2) {
              receiving = false;
              if(kbd_print_scancode(make, size, bytes)) return 1;
            }

          break;

        default:
          break; 
        }
      }
    }
  }

  if(keyboard_unsubscribe_int()) return 1;
  if(kbd_print_no_sysinb(inb_calls)) return 1;

  return 0;
}

int (kbd_test_poll)() 
{
   bool make, polling = true;
   uint8_t bytes[2], size = 0;
   int handler;

   while (polling)
   {
      kbc_ph();

      handler = scanCodeHandler(&make, &size, bytes);

      if(handler == 1) {
        if(kbd_print_scancode(make, size, bytes)) return 1;
        size = 0;
      }
      else if(handler == 2) {
        polling = false;//ends while
        if(kbd_print_scancode(make, size, bytes)) return 1;
      }
   }

    if(kbd_print_no_sysinb(inb_calls)) return 1;
    issueCommand(KBC_CMD_REG, INT_ENABLE);
    return 0;
}

int (kbd_test_timed_scan)(uint8_t n) {
  
  if(n < 0) return 1;
  
  int ipc_status, r, handler;
  message msg;
  uint32_t kbd_irq_set, timer_irq_set;
  uint8_t bytes[2], size = 0, elapsedTime = 0, bit_no;
  bool make, receiving = true;

  if(timer_subscribe_int(&bit_no)) return 1; 
  timer_irq_set = (uint32_t) (BIT(bit_no));

  if(keyboard_subscribe_int(&bit_no)) return 1; 
  kbd_irq_set = (uint32_t) (BIT(bit_no));

  while (receiving && (elapsedTime < n)) //Ends when esc key is released or when n seconds have passed
  {
    //Get a request message
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("driver_receive failes with: %d", r);
        continue;
      }

    if(is_ipc_notify(ipc_status)) { //received notification
      switch(_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: //hardware interrupt notification
          if(msg.m_notify.interrupts & kbd_irq_set) { //subscribed keyboard interrupts
            
            kbc_ih();
            handler = scanCodeHandler(&make, &size, bytes);
            interruptsCounter=0;//resets interrupts passed to 0
            elapsedTime=0;//resets seconds passed to 0
            if(handler == 1) {
              if(kbd_print_scancode(make, size, bytes)) return 1;
              size = 0;
            }
            else if(handler == 2) {
              receiving = false;
              if(kbd_print_scancode(make, size, bytes)) return 1;
            }
          }

          if(msg.m_notify.interrupts & timer_irq_set) { //subscribed timer interrupts
            timer_int_handler();
            if(( interruptsCounter % INTERRUPT_FREQ ) == 0) {
              elapsedTime++;//increases variable representing numbers of seconds passed
            }
          }
          break;

        default:
          break; 
      
      }
    }
  }
  if(keyboard_unsubscribe_int()) return 1;
  if(timer_unsubscribe_int()) return 1;

  return 0;
}

